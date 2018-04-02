use mruby_sys::RHash;
use error::{ErrorKind, MrbResult};
use vm::State;
use super::MrbPtrType;
/// A type representing mruby Hash
pub struct MrbHash<'cxt> {
    pub(super) data: &'cxt mut RHash,
    pub(super) state: &'cxt State,
}
