use super::{FromMrb, FromMrbRaw, IntoMrb, MrbInt, MrbPtrType, MrbValue};
use error::{ErrorKind, MrbResult};
use mruby_sys::{mrb_ary_concat, mrb_ary_entry, mrb_ary_new, mrb_ary_pop, mrb_ary_push, RArray};
use std::ptr::NonNull;
use tuple_map::TupleMap2;
use vm::{MrbVm, State};
/// A type representing mruby Array
#[derive(Clone, Eq, PartialEq, Hash)]
pub struct MrbArray<'cxt> {
    pub(super) data: NonNull<RArray>,
    pub(super) state: State<'cxt>,
}

impl<'cxt> MrbArray<'cxt> {
    /// Make mruby array class (just make, not register it to mruby state).
    /// Equivalent to
    /// ```ruby
    /// Array.new
    /// ```
    pub fn new(vm: &'cxt MrbVm) -> MrbResult<Self> {
        let raw_val = unsafe { mrb_ary_new(vm.state().as_ptr()) };
        Self::from_mrb_raw(raw_val, vm.state()).map_err(|e| e.chain("[MrbArray::new]"))
    }

    /// Push an element to mruby array class
    /// Equivalent to
    /// ```ruby
    /// ary << val
    /// ```
    pub fn push<V: IntoMrb<'cxt>>(&mut self, val: V) -> MrbResult<()> {
        let ary_raw = self.as_val();
        let val_raw = chain!(val.into_mrb(self.state), "[MrbArray::push]").as_raw();
        unsafe { mrb_ary_push(self.state.as_ptr(), ary_raw, val_raw) };
        Ok(())
    }

    /// Consume other and append its contains to Self's tail
    /// Equivalent to
    /// ```ruby
    /// ary.concat(other)
    /// ```
    pub fn append(&self, other: Self) {
        let (s_raw, o_raw) = (self, &other).map(|x| x.as_val());
        unsafe { mrb_ary_concat(self.state.as_ptr(), s_raw, o_raw) }
    }

    /// get n-th value of mruby array
    /// Equivalent to
    /// ```ruby
    /// ary[idx]
    /// ```
    pub fn get<V: FromMrb<'cxt>>(&self, idx: isize) -> MrbResult<V> {
        let ary_raw = self.as_val();
        let got = unsafe { mrb_ary_entry(ary_raw, idx as MrbInt) };
        V::from_mrb_raw(got, self.state)
    }
}

#[cfg(test)]
mod array_test {
    use super::*;
    #[test]
    fn push_get() {
        let f = || -> MrbResult<()> {
            let vm = MrbVm::new()?;
            let mut ary = MrbArray::new(&vm)?;
            ary.push(3)?;
            ary.push(4)?;
            assert_eq!(ary.get::<i64>(0)?, 3);
            assert_eq!(ary.get::<i64>(1)?, 4);
            Ok(())
        };
        f().unwrap();
    }
}
