use super::{MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::REnv;
use std::collections::HashMap;
use std::convert::{TryFrom, TryInto};
use std::ptr::NonNull;
use vm::State;
/// A type representing mruby Env
pub struct MrbEnv<'cxt> {
    pub(super) data: NonNull<REnv>,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbEnv<'cxt> {}
