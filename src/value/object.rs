use super::MrbPtrType;
use mruby_sys::RObject;
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby Object
pub struct MrbObject<'cxt> {
    pub(super) data: NonNull<RObject>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbObject<'cxt> {}
