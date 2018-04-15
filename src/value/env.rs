use super::{MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby::State;
use mruby_sys::REnv;
use std::collections::HashMap;
use std::convert::{TryFrom, TryInto};
use std::ptr::NonNull;
/// A type representing mruby Env
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbEnv<'cxt> {
    pub(super) data: NonNull<REnv>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbEnv<'cxt> {}
