use error::{ErrorKind, MrbResult};
use mruby_sys::{mrb_close, mrb_load_nstring_cxt, mrb_open, mrb_state, mrbc_context,
                mrbc_context_new};
use std::marker::PhantomData;
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
pub struct Mruby<'vm> {
    state: State<'vm>,
    main_cxt: MrbContext<'vm>,
    null_cxt: MrbContext<'vm>,
}

impl<'vm> Mruby<'vm> {
    pub fn new() -> MrbResult<Mruby<'vm>> {
        let state = unsafe { mrb_open() };
        let state = chain!(State::new(state), "[MrbVM::new]");
        let main = chain!(MrbContext::new(state), "[MrbVM::new]");
        let nul = MrbContext::empty(state);
        Ok(Mruby {
            state: state,
            main_cxt: main,
            null_cxt: nul,
        })
    }
    pub fn state(&self) -> State<'vm> {
        self.state.clone()
    }
    pub fn main(&mut self) -> &MrbContext<'vm> {
        &self.main_cxt
    }
    pub fn disposable(&mut self) -> &MrbContext<'vm> {
        &self.null_cxt
    }
}

impl<'vm> Drop for Mruby<'vm> {
    fn drop(&mut self) {
        unsafe {
            mrb_close(self.state.as_ptr());
        }
    }
}

/// Wrapper of mrbc_context
pub struct MrbContext<'vm> {
    state: State<'vm>,
    cxt: Option<NonNull<mrbc_context>>,
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
            non_null!(cxt, "[MrbContext::new]")
        };
        Ok(MrbContext {
            state: state,
            cxt: Some(cxt),
        })
    }

    /// Reset compiler status context
    pub fn reset(&mut self) -> MrbResult<&mut Self> {
        let cxt = unsafe {
            let cxt = mrbc_context_new(self.state.as_ptr());
            non_null!(cxt, "[MrbContext::new]")
        };
        self.cxt = Some(cxt);
        Ok(self)
    }

    /// Exec mruby code from string
    pub fn exec_str<'cxt>(&'cxt self, s: &str) -> MrbResult<MrbValue<'cxt>> {
        let str_len = s.as_bytes().len();
        let str_p = s.as_ptr() as *const c_char;
        let val = unsafe {
            let cxt_p = if let Some(ref cxt) = self.cxt {
                cxt.as_ptr()
            } else {
                ptr::null_mut::<mrbc_context>()
            };
            mrb_load_nstring_cxt(self.state.as_ptr(), str_p, str_len, cxt_p)
        };
        MrbValue::from_raw(self.state, val)
    }
}
