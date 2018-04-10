use mruby_sys::RException;
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby Exception
pub struct MrbException<'cxt> {
    pub(super) data: NonNull<RException>,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbException<'cxt> {}
