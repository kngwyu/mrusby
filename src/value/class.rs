use super::{MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::RClass;
use std::collections::HashMap;
use std::convert::{TryFrom, TryInto};
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby class
pub struct MrbClass<'cxt> {
    pub(super) data: NonNull<RClass>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbClass<'cxt> {}
