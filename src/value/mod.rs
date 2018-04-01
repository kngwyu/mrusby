use mruby_sys::*;
use std::os::raw::c_void;
mod array;
mod data;
mod exception;
mod hash;
mod object;
mod symbol;

pub use self::array::MrbArray;
pub use self::data::MrbData;
pub use self::exception::MrbException;
pub use self::hash::MrbHash;
pub use self::object::MrbObject;
pub use self::symbol::MrbSymbol;

use vm::State;
use error::{ErrorKind, MrbResult};

/// MrbInt is same as pointer size (usually it's 64bit)
pub type MrbInt = mrb_int;
/// MrbFloat is f64 in default setting, but you can change by feature
pub type MrbFloat = mrb_float;

/// rust representation of void*
pub type MrbCptr = *mut c_void;

// FreeとUndefはエラー型
// Exceptionは後でエラー型にいれるけど操作方法がわからんからとりあえずここ
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
    Exception(MrbException<'cxt>),
    File,
    Env,
    Data(MrbData<'cxt>),
    Fiber,
}

impl<'cxt> MrbValue<'cxt> {
    pub(crate) fn from_raw(state: &'cxt State, val: mrb_value) -> MrbResult<MrbValue<'cxt>> {
        let ty = val.tt;
        let make_err = |e: ErrorKind| {
            let err_str = format!("[MrbValue::from_raw] tt: {}", ty);
            Err(e.into_with(err_str))
        };
        let p = || unsafe { val.value.p };
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
            // mrb_vtype_MRB_TT_OBJECT => {
            //     let obj = ref_cast!(RObject);
            //     Ok(MrbValue::Object(MrbObject(obj)))
            // }
            // mrb_vtype_MRB_TT_CLASS => {
            //     let class = ref_cast!(RClass);
            //     Ok(MrbValue::Class(MrbClass(class)))
            // }
            // mrb_vtype_MRB_TT_MODULE => {}
            // mrb_vtype_MRB_TT_ICLASS => {}
            // mrb_vtype_MRB_TT_SCLASS => {}
            // mrb_vtype_MRB_TT_PROC => {
            //     let process = ref_cast!(RProc);
            //     Ok(MrbValue::Proc(MrbProc(process)))
            // }
            // mrb_vtype_MRB_TT_ARRAY => {
            //     let array = ref_cast!(RArray);
            //     Ok(MrbValue::Array(MrbArray(array)))
            // }
            // mrb_vtype_MRB_TT_HASH => {}
            // mrb_vtype_MRB_TT_STRING => {}
            // mrb_vtype_MRB_TT_RANGE => {}
            // mrb_vtype_MRB_TT_EXCEPTION => {
            //     let exc = ref_cast!(RException);
            //     Ok(MrbValue::Exception(MrbException(exc)))
            // }
            // mrb_vtype_MRB_TT_FILE => {}
            // mrb_vtype_MRB_TT_ENV => {}
            mrb_vtype_MRB_TT_DATA => {
                let res = chain!(MrbData::new(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Data(res))
            }
            // mrb_vtype_MRB_TT_FIBER => {}
            // mrb_vtype_MRB_TT_ISTRUCT => {}
            // mrb_vtype_MRB_TT_BREAK => {}
            // mrb_vtype_MRB_TT_MAXDEFINE => {}
            _ => unreachable!("unknown mruby value {}", ty),
        }
    }
}

/// A type representing mruby Class
pub struct MrbClass<'cxt>(&'cxt mut RClass);

/// A type representing mruby Env
// TODO: more description
pub struct MrbEnv<'cxt>(&'cxt mut REnv);

/// A type representing mruby Proc
pub struct MrbProc<'cxt>(&'cxt mut RProc);

trait MrbPtrType<'cxt>: Sized {
    fn from_ptr(s: &'cxt State, p: *mut c_void) -> MrbResult<Self>;
}
