extern crate bindgen;

use std::error::Error;
use std::env;
use std::fmt;
use std::path::PathBuf;
use std::fs::File;
use std::io::prelude::*;
use std::str;
use std::process::Command;

fn main() {
    let lib_path = PathBuf::from(env::var("OUT_DIR").unwrap()).join("host/lib");
    println!("cargo:rustc-link-search={}", lib_path.to_str().unwrap());
    println!("cargo:rustc-link-lib=mruby");
    build_mruby().unwrap();
    gen_bindings().unwrap();
}

fn gen_bindings() -> Result<(), BuildError> {
    let out_path = PathBuf::from(env::var("OUT_DIR")?);
    bindgen::Builder::default()
        .header("mruby-wrapper.h")
        .blacklist_type("max_align_t")
        .generate()?
        .write_to_file(out_path.join("mruby_bindings.rs"))?;
    Ok(())
}

const BUILD_CONFIG_RELEASE: &str = r#"
MRuby::Build.new do |conf|
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end
  conf.gembox 'default'
  conf.bins = []
end
"#;

const BUILD_CONFIG_DEBUG: &str = r#"
MRuby::Build.new do |conf|
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end
  enable_debug
  conf.gembox 'default'
  conf.bins = []
end
"#;

fn build_mruby() -> Result<(), BuildError> {
    let out_path = PathBuf::from(env::var("OUT_DIR")?);
    let config_file = if let Ok(f) = env::var("MRUSBY_CONFIG") {
        PathBuf::from(&f)
    } else {
        let (s, path) = if cfg!(debug_assertions) {
            (
                BUILD_CONFIG_DEBUG.to_owned(),
                out_path.clone().join("config_debug.rs"),
            )
        } else {
            (
                BUILD_CONFIG_RELEASE.to_owned(),
                out_path.clone().join("config_release.rs"),
            )
        };
        let mut f = File::create(&path)?;
        f.write_all(s.as_bytes())?;
        path
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

impl From<std::io::Error> for BuildError {
    fn from(e: std::io::Error) -> Self {
        BuildError::Io(e.description().to_owned())
    }
}
