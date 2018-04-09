use super::{MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::RClass;
use std::collections::HashMap;
use std::convert::{TryFrom, TryInto};
use vm::State;

/// A type representing mruby class
pub struct MrbClass<'cxt> {
    pub(super) data: &'cxt mut RClass,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbClass<'cxt> {}
