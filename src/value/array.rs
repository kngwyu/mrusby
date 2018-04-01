use mruby_sys::RArray;
use std::os::raw::c_void;
use error::{ErrorKind, MrbResult};
use vm::State;

/// A type representing mruby Array
pub struct MrbArray<'cxt> {
    data: &'cxt mut RArray,
    state: &'cxt State,
}

impl<'cxt> MrbArray<'cxt> {
    pub(in value) fn new(s: &'cxt State, p: *mut c_void) -> MrbResult<Self> {
        let data = unsafe { get_ref!(p as *mut RArray, "MrbArray::new") };
        Ok(MrbArray {
            data: data,
            state: s,
        })
    }
    fn f(&mut self) {
        let p = self.data as *mut RArray;
    }
}
