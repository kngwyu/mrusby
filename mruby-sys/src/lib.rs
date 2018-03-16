#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/mruby_bindings.rs"));

#[cfg(test)]
mod api_call_test {
    fn mruby_open() -> *mut mrb_state {
        unsafe { mrb_open() }
    }

    #[test]
    fn open_test() {
        mruby_open();
    }
}
