use super::{FromMrb, FromMrbRaw, IntoMrb, MrbPtrType, MrbValue};
use error::{ErrorKind, MrbError, MrbResult};
use mruby::{Mruby, State};
use mruby_sys::{mrb_hash_delete_key, mrb_hash_empty_p, mrb_hash_get, mrb_hash_keys, mrb_hash_new,
                mrb_hash_set, RHash};
use std::collections::HashMap;
use std::hash::Hash;
use std::ptr::NonNull;

/// A type representing mruby Hash
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbHash<'cxt> {
    pub(super) data: NonNull<RHash>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbHash<'cxt> {
    /// Delete key&value by the specified key from mruby hash class.
    /// Equivalent to
    /// ```ruby
    /// hash.delete(key)
    /// ```
    pub fn delete<K, V>(&self, key: K) -> MrbResult<V>
    where
        K: IntoMrb<'cxt>,
        V: FromMrb<'cxt>,
    {
        let hash_raw = self.as_val();
        let key_raw = chain!(key.into_mrb(self.state), "[MrbHash::delete]").as_raw();
        let val = unsafe { mrb_hash_delete_key(self.state.as_ptr(), hash_raw, key_raw) };
        V::from_mrb_raw(val, self.state).map_err(|e| e.chain("[MrbHash::delete]"))
    }

    /// Get a value from mruy hash class.
    /// Equivalent to
    /// ```ruby
    /// hash[key]
    /// ```
    // TODO: mruby returns Bool(false) instead of nil, so how we handle empty value?
    // If it's possible, return MrbResult<Option<V>> is better.
    pub fn get<K, V>(&self, key: K) -> MrbResult<V>
    where
        K: IntoMrb<'cxt>,
        V: FromMrb<'cxt>,
    {
        let hash_raw = self.as_val();
        let key_raw = chain!(key.into_mrb(self.state), "[MrbHash::get]").as_raw();
        let val = unsafe { mrb_hash_get(self.state.as_ptr(), hash_raw, key_raw) };
        V::from_mrb_raw(val, self.state).map_err(|e| e.chain("[Mrbhash::get]"))
    }

    /// Check if the mruby hash class is empty or not
    /// Equivalent to
    /// ```ruby
    /// hash.empty?
    /// ```
    pub fn is_empty(&self) -> MrbResult<bool> {
        let hash_raw = self.as_val();
        let val = unsafe { mrb_hash_empty_p(self.state.as_ptr(), hash_raw) };
        bool::from_mrb_raw(val, self.state).map_err(|e| e.chain("[MrbHash::is_empty]"))
    }
    /// Make mruby hash class (just make, not register it to mruby state).
    /// Equivalent to
    /// ```ruby
    /// Hash.new
    /// ```
    pub fn new(vm: &'cxt Mruby) -> MrbResult<Self> {
        let raw_val = unsafe { mrb_hash_new(vm.state().as_ptr()) };
        Self::from_mrb_raw(raw_val, vm.state()).map_err(|e| e.chain("[MrbHash::new]"))
    }

    /// Set a value to mruy hash class.
    /// Equivalent to
    /// ```ruby
    /// hash[key] = val
    /// ```
    pub fn set<K, V>(&mut self, key: K, val: V) -> MrbResult<()>
    where
        K: IntoMrb<'cxt>,
        V: IntoMrb<'cxt>,
    {
        let hash_raw = self.as_val();
        let key_raw = chain!(key.into_mrb(self.state), "[MrbHash::set]").as_raw();
        let val_raw = chain!(val.into_mrb(self.state), "[MrbHash::set]").as_raw();
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
//     fn into_mrb(self, vm: &'cxt Mruby) -> MrbResult<MrbValue<'cxt>> {

//     }
// }

#[cfg(test)]
mod hash_test {
    use super::*;
    use test_utils::with_vm;
    #[test]
    fn set_get() {
        with_vm(|vm| {
            let mut hash = MrbHash::new(&vm)?;
            hash.set(1000, -500)?;
            hash.set(200, -3.14)?;
            let got: i64 = hash.get(1000)?;
            assert_eq!(got, -500);
            let got: f64 = hash.get(200)?;
            assert!(got <= -3.13 && got >= -3.15);
            let got: MrbValue = hash.get(3)?;
            assert_eq!(got, MrbValue::Bool(false));
            Ok(())
        });
    }
    #[test]
    fn set_delete() {
        with_vm(|vm| {
            let mut hash = MrbHash::new(&vm)?;
            assert!(hash.is_empty()?);
            hash.set(200, 50)?;
            assert!(!hash.is_empty()?);
            let got: i64 = hash.delete(200)?;
            assert_eq!(got, 50);
            assert!(hash.is_empty()?);
            Ok(())
        });
    }
}
