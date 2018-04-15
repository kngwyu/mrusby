use error::{ErrorKind, MrbResult};
use mruby::State;
use mruby_sys::RData;
use std::os::raw::c_void;
use std::ptr::NonNull;
/// A type representing mruby User Data
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbData<'cxt> {
    pub(super) data: NonNull<RData>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbData<'cxt> {}
