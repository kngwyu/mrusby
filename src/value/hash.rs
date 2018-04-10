use super::{FromMrb, IntoMrb, MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::RHash;
use std::collections::HashMap;

use std::hash::Hash;
use vm::{MrbVm, State};

/// A type representing mruby Hash
pub struct MrbHash<'cxt> {
    pub(super) data: &'cxt mut RHash,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbHash<'cxt> {}

// impl<'cxt, K, V> IntoMrb<'cxt> for HashMap<K, V>
// where
//     K: FromMrb<'cxt> + Hash + Eq,
//     V: FromMrb<'cxt>,
// {
//     fn into_mrb(self, vm: &'cxt MrbVm) -> MrbResult<MrbValue<'cxt>> {

//     }
// }
