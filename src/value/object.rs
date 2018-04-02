use mruby_sys::RObject;
use vm::State;
use super::MrbPtrType;

/// A type representing mruby Object
pub struct MrbObject<'cxt> {
    pub(super) data: &'cxt mut RObject,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbObject<'cxt> {}
