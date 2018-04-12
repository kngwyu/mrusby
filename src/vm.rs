use error::{ErrorKind, MrbResult};
use mruby_sys::{mrb_close, mrb_load_nstring_cxt, mrb_open, mrb_state, mrbc_context,
                mrbc_context_new};
use std::marker::PhantomData;
use std::mem::transmute;
use std::os::raw::c_char;
use std::ptr::{self, NonNull};
use value::MrbValue;

/// wrapper of mrb_state
#[derive(Clone, Copy, Debug, Hash, Eq, PartialEq)]
pub struct State<'vm> {
    data: NonNull<mrb_state>,
    __marker: PhantomData<&'vm !>,
}

impl<'vm> State<'vm> {
    fn new(state: *mut mrb_state) -> MrbResult<Self> {
        let data = non_null!(state, "[State::new] mrb_state is Null");
        Ok(State {
            data: data,
            __marker: PhantomData,
        })
    }
    pub(crate) fn as_ptr(&self) -> *mut mrb_state {
        self.data.as_ptr()
    }
}

/// entry point of mrusby
pub struct MrbVm<'vm> {
    state: State<'vm>,
    main_cxt: MrbContext<'vm>,
}

impl<'vm> MrbVm<'vm> {
    pub fn new() -> MrbResult<MrbVm<'vm>> {
        unsafe {
            let state = chain!(State::new(mrb_open()), "[MrbVM::new]");
            let cxt = chain!(MrbContext::new(state), "[MrbVM::new]");
            Ok(MrbVm {
                state: state,
                main_cxt: cxt,
            })
        }
    }
    pub fn state(&self) -> State<'vm> {
        self.state.clone()
    }
    pub fn main(&mut self) -> &mut MrbContext<'vm> {
        &mut self.main_cxt
    }
    pub fn named(&self) -> MrbResult<MrbContext<'vm>> {
        MrbContext::new(self.state.clone())
    }
    pub fn unnamed(&self) -> MrbContext<'vm> {
        MrbContext::empty(self.state.clone())
    }
}

impl<'vm> Drop for MrbVm<'vm> {
    fn drop(&mut self) {
        unsafe {
            mrb_close(self.state.as_ptr());
        }
    }
}

/// Wrapper of mrbc_context
pub struct MrbContext<'vm> {
    state: State<'vm>,
    pub(crate) cxt: Option<&'vm mut mrbc_context>,
}

impl<'vm> MrbContext<'vm> {
    fn empty(state: State<'vm>) -> Self {
        MrbContext {
            state: state,
            cxt: None,
        }
    }
    fn new(state: State<'vm>) -> MrbResult<Self> {
        let cxt = unsafe {
            let cxt = mrbc_context_new(state.as_ptr());
            get_ref!(cxt, "[MrbContext::new] mrbc_context_new returned Null")
        };
        Ok(MrbContext {
            state: state,
            cxt: Some(cxt),
        })
    }

    pub fn reset(&mut self) -> MrbResult<&mut Self> {
        let cxt = unsafe {
            let cxt = mrbc_context_new(self.state.as_ptr());
            get_ref!(cxt, "[MrbContext::new] mrbc_context_new returned Null")
        };
        self.cxt = Some(cxt);
        Ok(self)
    }

    pub fn exec_str<'cxt>(&'cxt mut self, s: &str) -> MrbResult<MrbValue<'cxt>> {
        let str_len = s.as_bytes().len();
        let str_p = s.as_ptr() as *const c_char;
        let val = unsafe {
            let cxt_p = if let Some(ref mut cxt) = self.cxt {
                *cxt as *mut mrbc_context
            } else {
                ptr::null_mut::<mrbc_context>()
            };
            mrb_load_nstring_cxt(self.state.as_ptr(), str_p, str_len, cxt_p)
        };
        MrbValue::from_raw(self.state, val)
    }
}
