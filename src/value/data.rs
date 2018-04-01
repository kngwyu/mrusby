use mruby_sys::RData;
use std::os::raw::c_void;
use error::{ErrorKind, MrbResult};
use vm::State;
/// A type representing mruby User Data
pub struct MrbData<'cxt> {
    data: &'cxt mut RData,
    state: &'cxt State,
}

impl<'cxt> MrbData<'cxt> {
    pub(in value) fn new(s: &'cxt State, p: *mut c_void) -> MrbResult<Self> {
        let data = unsafe { get_ref!(p as *mut RData, "MrbData::new") };
        Ok(MrbData {
            data: data,
            state: s,
        })
    }
}
