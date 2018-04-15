use mruby_sys::mrb_sym;
use std::marker::PhantomData;

use mruby::State;
/// A type representing mruby Symbol
#[derive(Clone, Copy, Debug, Eq, PartialEq, Hash)]
pub struct MrbSymbol<'cxt> {
    inner: mrb_sym,
    state: State<'cxt>,
}

impl<'cxt> MrbSymbol<'cxt> {
    pub(super) fn new(sym: mrb_sym, state: State<'cxt>) -> MrbSymbol<'cxt> {
        MrbSymbol {
            inner: sym,
            state: state,
        }
    }
    pub(super) fn raw(&self) -> mrb_sym {
        self.inner
    }
}
