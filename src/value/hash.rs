use super::{MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::RHash;
use std::collections::HashMap;
use std::convert::{TryFrom, TryInto};
use vm::State;

/// A type representing mruby Hash
pub struct MrbHash<'cxt> {
    pub(super) data: &'cxt mut RHash,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbHash<'cxt> {}

// impl<'cxt, K, V> TryFrom<MrbHash<'cxt>> for HashMap<K, V>
// where
//     K: TryFrom<MrbValue<'cxt>>,
//     V: TryFrom<MrbValue<'cxt>>,
// {
//     type Error = MrbError;
//     fn try_from(value: MrbHash<'cxt>) {}
// }

// impl<'cxt, K, V> TryInto<HashMap<K, V>> for MrbHash<'cxt>
// where
//     K: TryFrom<MrbValue<'cxt>>,
//     V: TryFrom<MrbValue<'cxt>>,
// {
//     type Error = MrbError;
//     fn try_into(self) -> MrbResult<HashMap<K, V>> {

//     }
// }
