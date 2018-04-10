mod array;
mod class;
mod data;
mod env;
mod exception;
mod hash;
mod object;
// mod proc;
mod symbol;

use mruby_sys::*;
use num_traits::NumCast;
use std::fmt;
use std::os::raw::c_void;

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
use vm::{MrbVm, State};

/// MrbInt is same as pointer size (usually it's 64bit)
pub type MrbInt = mrb_int;
/// MrbFloat is f64 in default setting, but you can change by feature
pub type MrbFloat = mrb_float;

/// rust representation of void*
// TODO: what is the use case?
pub type MrbCptr = *mut c_void;

/// Value user can use in Mrb VM or context
// Free & Undef are defined in error type(also exception?)
#[derive(Debug)]
pub enum MrbValue<'cxt> {
    Bool(bool),
    Integer(MrbInt),
    Symbol(MrbSymbol<'cxt>),
    Float(MrbFloat),
    Cptr(MrbCptr),
    Object(MrbObject<'cxt>),
    Class(MrbClass<'cxt>),
    Module,
    Proc,
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
    pub(crate) fn as_raw(&mut self) -> mrb_value {
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
            MrbValue::Object(ref mut obj) => obj.as_val(),
            MrbValue::Class(ref mut class) => class.as_val(),
            MrbValue::Array(ref mut array) => array.as_val(),
            MrbValue::Hash(ref mut hash) => hash.as_val(),
            MrbValue::Exception(ref mut exc) => exc.as_val(),
            MrbValue::Env(ref mut env) => env.as_val(),
            MrbValue::Data(ref mut dat) => dat.as_val(),
            _ => unimplemented!(),
        }
    }
}

/// A type representing mruby Proc
pub struct MrbProc<'cxt>(&'cxt mut RProc);

/// Types which can be converted into MrbValue.
/// When using MrbContext::exec_str or other methods which eval mruby codes,
/// values have lifetime same as context, but, values generated by IntoMrb have
/// lifetime same as Vm. But, here we use 'cxt' for ergonomics.
pub trait IntoMrb<'cxt> {
    fn into_mrb(self, vm: &'cxt MrbVm) -> MrbResult<MrbValue<'cxt>>;
}

/// Type which can be converted from MrbValue.
pub trait FromMrb<'cxt>: Sized {
    fn from_mrb(val: MrbValue<'cxt>) -> MrbResult<Self>;
}

/// Type conversion trait(for derive)
pub trait MrbConvert<'cxt>: IntoMrb<'cxt> + FromMrb<'cxt> {}

impl<'cxt, T> MrbConvert<'cxt> for T
where
    T: IntoMrb<'cxt> + FromMrb<'cxt>,
{
}

/// utility trait to convert None to MrbError in various contexts
pub(crate) trait TryCast<T> {
    fn cast(self) -> MrbResult<T>;
}

impl<T, U> TryCast<T> for U
where
    T: NumCast,
    U: NumCast,
{
    fn cast(self) -> MrbResult<T> {
        if let Some(res) = T::from(self) {
            Ok(res)
        } else {
            Err(ErrorKind::NumCast.into())
        }
    }
}

macro_rules! impl_conv {
    ($t:ty, $var:ident) => {
        impl<'cxt> IntoMrb<'cxt> for $t {
            fn into_mrb(self, _vm: &'cxt MrbVm) -> MrbResult<MrbValue<'cxt>> {
                Ok(MrbValue::$var(self.cast()?))
            }
        }
        impl<'cxt> FromMrb<'cxt> for $t {
            fn from_mrb(val: MrbValue<'cxt>) -> MrbResult<Self> {
                if let MrbValue::$var(i) = val {
                    i.cast()
                } else {
                    let err_msg = format!("{:?} => {}", val, stringify!($t));
                    Err(ErrorKind::From.into_with(err_msg))
                }
            }
        }
    };
}

impl_conv!(u8, Integer);
impl_conv!(u16, Integer);
impl_conv!(u32, Integer);
impl_conv!(u64, Integer);
impl_conv!(usize, Integer);
impl_conv!(i8, Integer);
impl_conv!(i16, Integer);
impl_conv!(i32, Integer);
impl_conv!(i64, Integer);
impl_conv!(isize, Integer);
impl_conv!(f32, Float);
impl_conv!(f64, Float);

trait MrbPtrType<'cxt>: Sized {
    fn from_ptr(s: &'cxt State, p: *mut c_void) -> MrbResult<Self>;
    fn as_val(&mut self) -> mrb_value;
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
            fn as_val(&mut self) -> mrb_value {
                mrb_value {
                    tt: $tt,
                    value: mrb_value__bindgen_ty_1 {
                        p: (self.data as *mut $rbname) as *mut c_void,
                    },
                }
            }
        }
        // TODO: more detailed debug info
        impl<'cxt> fmt::Debug for $rsname<'cxt> {
            fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
                write!(f, "{}", stringify!($rsname))
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
