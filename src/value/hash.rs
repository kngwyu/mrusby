use super::{FromMrb, IntoMrb, MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby_sys::{mrb_hash_get, mrb_hash_new, mrb_hash_set, RHash};
use std::collections::HashMap;
use std::hash::Hash;
use std::ptr::NonNull;
use vm::{MrbVm, State};

/// A type representing mruby Hash
pub struct MrbHash<'cxt> {
    pub(super) data: NonNull<RHash>,
    pub(super) state: &'cxt State,
}

impl<'cxt> MrbHash<'cxt> {
    pub fn new(vm: &'cxt MrbVm) -> MrbResult<Self> {
        let raw_val = unsafe { mrb_hash_new(vm.state.as_ptr()) };
        let val = chain!(MrbValue::from_raw(&vm.state, raw_val), "MrbHash::new");
        Self::from_mrb(val)
    }
    pub fn get<K, V>(&self, key: K) -> MrbResult<V>
    where
        K: IntoMrb<'cxt>,
        V: FromMrb<'cxt>,
    {
        let hash_raw = self.as_val();
        let key_raw = chain!(key.into_mrb(&self.state), "[MrbHash::get]").as_raw();
        let val = unsafe { mrb_hash_get(self.state.as_ptr(), hash_raw, key_raw) };
        let val = chain!(MrbValue::from_raw(&self.state, val), "[MrbHash::get]");
        V::from_mrb(val)
    }
    pub fn set<K, V>(&mut self, key: K, val: V) -> MrbResult<()>
    where
        K: IntoMrb<'cxt>,
        V: IntoMrb<'cxt>,
    {
        let hash_raw = self.as_val();
        let key_raw = chain!(key.into_mrb(&self.state), "[MrbHash::get]").as_raw();
        let val_raw = chain!(val.into_mrb(&self.state), "[MrbHash::get]").as_raw();
        unsafe {
            mrb_hash_set(self.state.as_ptr(), hash_raw, key_raw, val_raw);
        }
        Ok(())
    }
}

// impl<'cxt, K, V> IntoMrb<'cxt> for HashMap<K, V>
// where
//     K: FromMrb<'cxt> + Hash + Eq,
//     V: FromMrb<'cxt>,
// {
//     fn into_mrb(self, vm: &'cxt MrbVm) -> MrbResult<MrbValue<'cxt>> {

//     }
// }

#[cfg(test)]
mod hash_test {
    use super::*;
    #[test]
    fn get_set() {
        let vm = MrbVm::new().unwrap();
        let mut hash = MrbHash::new(&vm).unwrap();
        hash.set(1000, -500).unwrap();
        hash.set(200, -3.14).unwrap();
        let got: i64 = hash.get(1000).unwrap();
        assert_eq!(got, -500);
        let got: f64 = hash.get(200).unwrap();
        assert!(got <= -3.13 && got >= -3.15);
    }
}
