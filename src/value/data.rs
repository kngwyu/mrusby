use mruby_sys::RData;
use std::os::raw::c_void;
use error::{ErrorKind, MrbResult};
use vm::State;
/// A type representing mruby User Data
pub struct MrbData<'cxt> {
    pub(super) data: &'cxt mut RData,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbData<'cxt> {}
