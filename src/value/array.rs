use mruby_sys::RArray;
use error::{ErrorKind, MrbResult};
use vm::State;
use super::MrbPtrType;

/// A type representing mruby Array
pub struct MrbArray<'cxt> {
    pub(super) data: &'cxt mut RArray,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbArray<'cxt> {}
