use error::{ErrorKind, MrbResult};
use mruby_sys::{mrb_close, mrb_load_nstring_cxt, mrb_open, mrb_state, mrbc_context,
                mrbc_context_new};
use std::mem::transmute;
use std::os::raw::c_char;
use std::ptr::{self, NonNull};
use value::MrbValue;

/// wrapper of mrb_state
#[derive(Clone, Copy, Debug)]
pub struct State(NonNull<mrb_state>);

impl State {
    fn new(state: *mut mrb_state) -> MrbResult<State> {
        if let Some(s) = NonNull::new(state) {
            Ok(State(s))
        } else {
            Err(ErrorKind::Null.into_with("[State::new] mrb_state is Null"))
        }
    }
    pub(crate) fn as_ptr(&self) -> *mut mrb_state {
        self.0.as_ptr()
    }
    pub(crate) fn as_ref<'a>(&self) -> &'a Self {
        unsafe { transmute(self) }
    }
}

/// entry point of mrusby
pub struct MrbVm<'vm> {
    pub(crate) state: State,
    main_cxt: MrbContext<'vm>,
}

impl<'vm> MrbVm<'vm> {
    pub fn new() -> MrbResult<MrbVm<'vm>> {
        unsafe {
            let state = chain!(State::new(mrb_open()), "[MrbVM::new]");
            let cxt = chain!(MrbContext::new(state.as_ref()), "[MrbVM::new]");
            Ok(MrbVm {
                state: state,
                main_cxt: cxt,
            })
        }
    }
    pub fn cxt(&mut self) -> &mut MrbContext<'vm> {
        &mut self.main_cxt
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
    state: &'vm State,
    cxt: Option<&'vm mut mrbc_context>,
}

impl<'vm> MrbContext<'vm> {
    fn new(state: &'vm State) -> MrbResult<Self> {
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

    pub fn exec_str<'cxt>(&mut self, s: &str) -> MrbResult<MrbValue<'cxt>> {
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
        MrbValue::from_raw(self.state.as_ref(), val)
    }
}
