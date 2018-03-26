extern crate bindgen;

use std::error::Error;
use std::env;
use std::fmt;
use std::path::PathBuf;
use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufReader, BufWriter};
use std::str;
use std::process::Command;

fn main() {
    let lib_path = PathBuf::from(env::var("OUT_DIR").unwrap()).join("host/lib");
    println!("cargo:rustc-link-search={}", lib_path.to_str().unwrap());
    println!("cargo:rustc-link-lib=mruby");
    build_mruby().unwrap();
    gen_bindings().unwrap();
}

const HEADER: &str = "mruby-wrapper.h";
fn gen_bindings() -> Result<(), BuildError> {
    let out_path = PathBuf::from(env::var("OUT_DIR")?);
    bindgen::Builder::default()
        .header(out_path.join(HEADER).to_str().unwrap())
        .blacklist_type("max_align_t")
        .generate()?
        .write_to_file(out_path.join("mruby_bindings.rs"))?;
    Ok(())
}

#[derive(Debug, Clone)]
struct Define {
    name: String,
    value: Option<u64>,
}

impl Define {
    fn new(name: &str) -> Self {
        Define {
            name: name.to_owned(),
            value: None,
        }
    }
    #[allow(unused)]
    fn value(mut self, v: u64) -> Self {
        self.value = Some(v);
        self
    }
    fn float32() -> Self {
        Self::new("MRB_USE_FLOAT")
    }
    fn method_cache() -> Self {
        Self::new("MRB_METHOD_CACHE")
    }
    fn utf8() -> Self {
        Self::new("MRB_UTF8_STRING")
    }
    fn enable_debug() -> Self {
        Self::new("MRB_ENABLE_DEBUG")
    }
}

const BUILD_CONFIG: &str = r#"
MRuby::Build.new do |conf|
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :clang
  end
  conf.gembox 'default'
  conf.bins = []
  conf.cc.defines = %w("#;

struct MrubyConfig {
    defines: Vec<Define>,
}

impl MrubyConfig {
    fn add(&mut self, d: Define) {
        self.defines.push(d);
    }
    fn new() -> Self {
        let mut builder = Self { defines: vec![] };
        if cfg!(feature = "float-32") {
            builder.add(Define::float32());
        }
        if cfg!(feature = "utf8-string") {
            builder.add(Define::utf8());
        }
        if cfg!(feature = "method-cache") {
            builder.add(Define::method_cache());
        }
        if cfg!(debug_assertions) {
            builder.add(Define::enable_debug());
        }
        builder
    }
    fn write(self, out_path: PathBuf) -> Result<PathBuf, io::Error> {
        let header_path = out_path.clone().join(HEADER);
        let config_path = out_path.clone().join("config.rb");
        let mut header_writer = BufWriter::new(File::create(&header_path)?);
        let mut config_writer = BufWriter::new(File::create(&config_path)?);
        config_writer.write_all(BUILD_CONFIG.as_bytes())?;
        for def in self.defines {
            write!(header_writer, "#define {}", def.name)?;
            write!(config_writer, " {}", def.name)?;
            if let Some(value) = def.value {
                writeln!(header_writer, "{}", value)?;
            } else {
                writeln!(header_writer, "")?;
            }
        }
        write!(config_writer, ")\nend")?;
        let mut reader = BufReader::new(File::open(HEADER)?);
        let mut buf = Vec::new();
        reader.read_to_end(&mut buf)?;
        header_writer.write_all(&buf)?;
        Ok(config_path)
    }
}

fn build_mruby() -> Result<(), BuildError> {
    let out_path = PathBuf::from(env::var("OUT_DIR")?);
    let config_file = if let Ok(f) = env::var("MRUSBY_CONFIG") {
        PathBuf::from(&f)
    } else {
        let conf = MrubyConfig::new();
        conf.write(out_path.clone())?
    };
    let status = Command::new("ruby")
        .current_dir("mruby/")
        .arg("./minirake")
        .env("MRUBY_BUILD_DIR", &out_path)
        .env("MRUBY_CONFIG", &config_file)
        .status()?;
    if !status.success() {
        panic!("failed to build mruby");
    }
    Ok(())
}

#[derive(Debug)]
enum BuildError {
    Env(String),
    BindGen,
    Io(String),
}

impl fmt::Display for BuildError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        use BuildError::*;
        match *self {
            Env(ref s) => write!(f, "Environment Variable error: {}", s),
            BindGen => write!(f, "bindgen error"),
            Io(ref s) => write!(f, "IO error: {}", s),
        }
    }
}

impl Error for BuildError {
    fn description(&self) -> &str {
        "mruby-sys Build Error"
    }
}

impl From<env::VarError> for BuildError {
    fn from(e: env::VarError) -> Self {
        BuildError::Env(e.description().to_owned())
    }
}

impl From<()> for BuildError {
    fn from(_: ()) -> Self {
        BuildError::BindGen
    }
}

impl From<io::Error> for BuildError {
    fn from(e: io::Error) -> Self {
        BuildError::Io(e.description().to_owned())
    }
}
