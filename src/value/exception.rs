use mruby_sys::RException;
use vm::State;
/// A type representing mruby Exception
pub struct MrbException<'cxt> {
    pub(super) data: &'cxt mut RException,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbException<'cxt> {}
