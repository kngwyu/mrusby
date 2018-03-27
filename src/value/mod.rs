use mruby_sys::*;
use std::os::raw::c_void;
pub mod ptr;
/// MrbInt is same as pointer size (usually it's 64bit)
pub type MrbInt = mrb_int;
/// MrbFloat is f64 in default setting, but you can change by feature
pub type MrbFloat = mrb_float;

/// A type representing mruby Symbol
#[derive(Clone, Copy, Debug, Eq, PartialEq, Hash)]
pub struct MrbSymbol(mrb_sym);

/// rust representation of void*
pub type MrbCptr = *mut c_void;

/// A type representing mruby Object
#[derive(Debug)]
pub struct MrbObject<'cxt>(&'cxt mut RObject);

/// A type representing mruby Class
#[derive(Clone, Copy, Debug, Eq, PartialEq, Hash)]
pub struct MrbClass(*mut RClass);

// FreeとUndefはまとめてエラー型に分離してもいいかも？
pub enum MrbValue {
    Bool(bool),
    Integer(MrbInt),
    Symbol(MrbSymbol),
    Float(MrbFloat),
    Cptr(MrbCptr),
    Object,
    Class(MrbClass),
    Module,
    Proc,
    Array,
    Hash,
    String,
    Range,
    Exception,
    File,
    Env,
    Data,
    Fiber,
}
