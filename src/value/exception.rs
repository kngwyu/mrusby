use mruby::State;
use mruby_sys::RException;
use std::ptr::NonNull;
/// A type representing mruby Exception
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbException<'cxt> {
    pub(super) data: NonNull<RException>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbException<'cxt> {}
