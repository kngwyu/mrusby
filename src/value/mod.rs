use mruby_sys::*;
use std::marker::PhantomData;
use std::os::raw::c_void;
pub mod ptr;
pub use self::ptr::MrbPtr;
use error::{ErrorKind, MrbError, MrbResult};
/// MrbInt is same as pointer size (usually it's 64bit)
pub type MrbInt = mrb_int;
/// MrbFloat is f64 in default setting, but you can change by feature
pub type MrbFloat = mrb_float;

/// A type representing mruby Symbol
#[derive(Clone, Copy, Debug, Eq, PartialEq, Hash)]
pub struct MrbSymbol<'cxt> {
    inner: mrb_sym,
    __marker: PhantomData<&'cxt ()>,
}

impl<'cxt> MrbSymbol<'cxt> {
    fn new(sym: mrb_sym) -> MrbSymbol<'cxt> {
        MrbSymbol {
            inner: sym,
            __marker: PhantomData,
        }
    }
}

/// rust representation of void*
pub type MrbCptr = *mut c_void;

// FreeとUndefはまとめてエラー型に分離してもいいかも？
pub enum MrbValue<'cxt> {
    Bool(bool),
    Integer(MrbInt),
    Symbol(MrbSymbol<'cxt>),
    Float(MrbFloat),
    Cptr(MrbCptr),
    Object(MrbObject<'cxt>),
    Class(MrbClass<'cxt>),
    Module,
    Proc(MrbProc<'cxt>),
    Array(MrbArray<'cxt>),
    Hash,
    String,
    Range,
    Exception,
    File,
    Env,
    Data,
    Fiber,
}

impl<'cxt> MrbValue<'cxt> {
    pub fn from_raw(val: mrb_value) -> MrbResult<MrbValue<'cxt>> {
        let ty = val.tt;
        let make_err = |e: ErrorKind| {
            let err_str = format!("[MrbValue::from_raw] tt: {}", ty);
            Err(e.into_with(err_str))
        };
        macro_rules! ref_cast {
            ($t:ty) => {{
                unsafe {
                    let obj = val.value.p as *mut $t;
                    get_ref!(obj, "[MrbValue::from_raw] tt: {}", ty)
                }
            }};
        }
        match ty {
            mrb_vtype_MRB_TT_FALSE => Ok(MrbValue::Bool(false)),
            mrb_vtype_MRB_TT_FREE => make_err(ErrorKind::FreeVal),
            mrb_vtype_MRB_TT_TRUE => Ok(MrbValue::Bool(true)),
            mrb_vtype_MRB_TT_FIXNUM => unsafe {
                let i = val.value.i;
                Ok(MrbValue::Integer(i))
            },
            mrb_vtype_MRB_TT_SYMBOL => unsafe {
                let sym = val.value.sym;
                let sym = MrbSymbol::new(sym);
                Ok(MrbValue::Symbol(sym))
            },
            mrb_vtype_MRB_TT_UNDEF => make_err(ErrorKind::Undefined),
            mrb_vtype_MRB_TT_FLOAT => unsafe {
                let float = val.value.f;
                Ok(MrbValue::Float(float))
            },
            mrb_vtype_MRB_TT_CPTR => unsafe {
                let c_ptr = val.value.p;
                Ok(MrbValue::Cptr(c_ptr))
            },
            mrb_vtype_MRB_TT_OBJECT => {
                let obj = ref_cast!(RObject);
                Ok(MrbValue::Object(MrbObject(obj)))
            }
            mrb_vtype_MRB_TT_CLASS => {
                let class = ref_cast!(RClass);
                Ok(MrbValue::Class(MrbClass(class)))
            }
            // mrb_vtype_MRB_TT_MODULE => {}
            // mrb_vtype_MRB_TT_ICLASS => {}
            // mrb_vtype_MRB_TT_SCLASS => {}
            mrb_vtype_MRB_TT_PROC => {
                let process = ref_cast!(RProc);
                Ok(MrbValue::Proc(MrbProc(process)))
            }

            mrb_vtype_MRB_TT_ARRAY => {
                let array = ref_cast!(RArray);
                Ok(MrbValue::Array(MrbArray(array)))
            }
            // mrb_vtype_MRB_TT_HASH => {}
            // mrb_vtype_MRB_TT_STRING => {}
            // mrb_vtype_MRB_TT_RANGE => {}
            // mrb_vtype_MRB_TT_EXCEPTION => {}
            // mrb_vtype_MRB_TT_FILE => {}
            // mrb_vtype_MRB_TT_ENV => {}
            // mrb_vtype_MRB_TT_DATA => {}
            // mrb_vtype_MRB_TT_FIBER => {}
            // mrb_vtype_MRB_TT_ISTRUCT => {}
            // mrb_vtype_MRB_TT_BREAK => {}
            // mrb_vtype_MRB_TT_MAXDEFINE => {}
            _ => unreachable!("unknown mruby value {}", ty),
        }
    }
}

/// A type representing mruby Object
pub struct MrbObject<'cxt>(&'cxt mut RObject);

/// A type representing mruby Class
pub struct MrbClass<'cxt>(&'cxt mut RClass);

/// A type representing mruby Array
pub struct MrbArray<'cxt>(&'cxt mut RArray);

/// A type representing mruby User Data
pub struct MrbData<'cxt>(&'cxt mut RData);

/// A type representing mruby Hash
pub struct MrbHash<'cxt>(&'cxt mut RHash);

/// A type representing mruby Env
// TODO: more description
pub struct MrbEnv<'cxt>(&'cxt mut REnv);

/// A type representing mruby Proc
pub struct MrbProc<'cxt>(&'cxt mut RProc);

/// A type representing mruby Exception
pub struct MrbException<'cxt>(&'cxt mut RException);
