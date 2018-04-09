use mruby_sys::mrb_sym;
use std::marker::PhantomData;

/// A type representing mruby Symbol
#[derive(Clone, Copy, Debug, Eq, PartialEq, Hash)]
pub struct MrbSymbol<'cxt> {
    inner: mrb_sym,
    __marker: PhantomData<&'cxt ()>,
}

impl<'cxt> MrbSymbol<'cxt> {
    pub(super) fn new(sym: mrb_sym) -> MrbSymbol<'cxt> {
        MrbSymbol {
            inner: sym,
            __marker: PhantomData,
        }
    }
    pub(super) fn raw(&self) -> mrb_sym {
        self.inner
    }
}
