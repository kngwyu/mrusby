use mruby_sys::*;
use std::os::raw::c_void;

mod array;
mod class;
mod data;
mod env;
mod exception;
mod hash;
mod object;
// mod proc;
mod symbol;

pub use self::array::MrbArray;
pub use self::class::MrbClass;
pub use self::data::MrbData;
pub use self::env::MrbEnv;
pub use self::exception::MrbException;
pub use self::hash::MrbHash;
pub use self::object::MrbObject;
// pub use self::proc::MrbProc;
pub use self::symbol::MrbSymbol;

use error::{ErrorKind, MrbResult};
use std::convert::{TryFrom, TryInto};
use vm::State;

/// MrbInt is same as pointer size (usually it's 64bit)
pub type MrbInt = mrb_int;
/// MrbFloat is f64 in default setting, but you can change by feature
pub type MrbFloat = mrb_float;

/// rust representation of void*
// TODO: what is the use case?
pub type MrbCptr = *mut c_void;

/// Value user can use in Mrb VM or context
// Free & Undef are defined in error type(also exception?)
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
    Hash(MrbHash<'cxt>),
    String,
    Range,
    Exception(MrbException<'cxt>),
    File,
    Env(MrbEnv<'cxt>),
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
            mrb_vtype_MRB_TT_OBJECT => {
                let res = chain!(MrbObject::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Object(res))
            }
            mrb_vtype_MRB_TT_CLASS => {
                let res = chain!(MrbClass::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Class(res))
            }
            // mrb_vtype_MRB_TT_MODULE => {}
            // mrb_vtype_MRB_TT_ICLASS => {}
            // mrb_vtype_MRB_TT_SCLASS => {}
            // mrb_vtype_MRB_TT_PROC => {
            //     let process = ref_cast!(RProc);
            //     Ok(MrbValue::Proc(MrbProc(process)))
            // }
            mrb_vtype_MRB_TT_ARRAY => {
                let res = chain!(MrbArray::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Array(res))
            }
            mrb_vtype_MRB_TT_HASH => {
                let res = chain!(MrbHash::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Hash(res))
            }
            // mrb_vtype_MRB_TT_STRING => {}
            // mrb_vtype_MRB_TT_RANGE => {}
            mrb_vtype_MRB_TT_EXCEPTION => {
                let res = chain!(MrbException::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Exception(res))
            }
            // mrb_vtype_MRB_TT_FILE => {}
            mrb_vtype_MRB_TT_ENV => {
                let res = chain!(MrbEnv::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Env(res))
            }
            mrb_vtype_MRB_TT_DATA => {
                let res = chain!(MrbData::from_ptr(state, p()), "MrbValue::from_raw");
                Ok(MrbValue::Data(res))
            }
            // mrb_vtype_MRB_TT_FIBER => {}
            // mrb_vtype_MRB_TT_ISTRUCT => {}
            // mrb_vtype_MRB_TT_BREAK => {}
            // mrb_vtype_MRB_TT_MAXDEFINE => {}
            _ => unreachable!("unknown mruby value {}", ty),
        }
    }
    pub(crate) fn get_raw(&mut self) -> mrb_value {
        match *self {
            MrbValue::Bool(b) => {
                let tt = if b {
                    mrb_vtype_MRB_TT_TRUE
                } else {
                    mrb_vtype_MRB_TT_FALSE
                };
                mrb_value {
                    tt: tt,
                    value: mrb_value__bindgen_ty_1 { i: 1 },
                }
            }
            MrbValue::Integer(i) => mrb_value {
                tt: mrb_vtype_MRB_TT_FIXNUM,
                value: mrb_value__bindgen_ty_1 { i: i },
            },
            MrbValue::Symbol(ref symbol) => mrb_value {
                tt: mrb_vtype_MRB_TT_SYMBOL,
                value: mrb_value__bindgen_ty_1 { sym: symbol.raw() },
            },
            MrbValue::Float(f) => mrb_value {
                tt: mrb_vtype_MRB_TT_FLOAT,
                value: mrb_value__bindgen_ty_1 { f: f },
            },
            MrbValue::Cptr(ptr) => mrb_value {
                tt: mrb_vtype_MRB_TT_CPTR,
                value: mrb_value__bindgen_ty_1 { p: ptr },
            },
            MrbValue::Object(ref mut obj) => obj.get_val(),
            MrbValue::Class(ref mut class) => class.get_val(),
            MrbValue::Array(ref mut array) => array.get_val(),
            MrbValue::Hash(ref mut hash) => hash.get_val(),
            MrbValue::Exception(ref mut exc) => exc.get_val(),
            MrbValue::Env(ref mut env) => env.get_val(),
            MrbValue::Data(ref mut dat) => dat.get_val(),
            _ => unimplemented!(),
        }
    }
}

trait MrbPtrType<'cxt>: Sized {
    fn from_ptr(s: &'cxt State, p: *mut c_void) -> MrbResult<Self>;
    fn get_val(&mut self) -> mrb_value;
}

macro_rules! impl_ptr_type {
    ($rbname:ident, $rsname:ident, $tt:expr) => {
        impl<'cxt> MrbPtrType<'cxt> for $rsname<'cxt> {
            fn from_ptr(s: &'cxt State, p: *mut c_void) -> MrbResult<Self> {
                let data =
                    unsafe { get_ref!(p as *mut $rbname, concat!(stringify!($rsname), "::new")) };
                Ok(Self {
                    data: data,
                    state: s,
                })
            }
            fn get_val(&mut self) -> mrb_value {
                mrb_value {
                    tt: $tt,
                    value: mrb_value__bindgen_ty_1 {
                        p: (self.data as *mut $rbname) as *mut c_void,
                    },
                }
            }
        }
    };
}

impl_ptr_type!(RArray, MrbArray, mrb_vtype_MRB_TT_ARRAY);
impl_ptr_type!(RClass, MrbClass, mrb_vtype_MRB_TT_CLASS);
impl_ptr_type!(RData, MrbData, mrb_vtype_MRB_TT_DATA);
impl_ptr_type!(REnv, MrbEnv, mrb_vtype_MRB_TT_ENV);
impl_ptr_type!(RException, MrbException, mrb_vtype_MRB_TT_EXCEPTION);
impl_ptr_type!(RHash, MrbHash, mrb_vtype_MRB_TT_HASH);
impl_ptr_type!(RObject, MrbObject, mrb_vtype_MRB_TT_OBJECT);
// impl_ptr_type!(RProc, MrbProc, mrb_vtype_MRB_TT_PROC);

/// A type representing mruby Proc
pub struct MrbProc<'cxt>(&'cxt mut RProc);

pub trait MrbConvertible<'cxt>: TryInto<MrbValue<'cxt>> + TryFrom<MrbValue<'cxt>> {}
