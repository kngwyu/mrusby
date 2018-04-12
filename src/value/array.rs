use super::MrbPtrType;
use error::{ErrorKind, MrbResult};
use mruby_sys::RArray;
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby Array
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbArray<'cxt> {
    pub(super) data: NonNull<RArray>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbArray<'cxt> {}
