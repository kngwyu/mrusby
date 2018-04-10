use super::MrbPtrType;
use error::{ErrorKind, MrbResult};
use mruby_sys::RArray;
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby Array
pub struct MrbArray<'cxt> {
    pub(super) data: NonNull<RArray>,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbArray<'cxt> {}
