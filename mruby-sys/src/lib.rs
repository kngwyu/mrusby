#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/mruby_bindings.rs"));

#[cfg(test)]
mod api_call_test {
    use super::*;
    use std::ptr;
    use std::os::raw::c_char;
    fn mruby_open() -> *mut mrb_state {
        unsafe { mrb_open() }
    }
    fn mruby_close(mrb: *mut mrb_state) {
        unsafe { mrb_close(mrb) }
    }
    fn compiler_context(mrb: *mut mrb_state) -> *mut mrbc_context {
        unsafe { mrbc_context_new(mrb) }
    }

    #[test]
    fn open_test() {
        let mrb = mruby_open();
        assert!(mrb != ptr::null_mut());
        mruby_close(mrb);
    }

    #[test]
    fn hello_world() {
        let mrb = mruby_open();
        assert!(mrb != ptr::null_mut());
        let s = r"
s = 'あいうえお'
puts s[1]
";
        unsafe {
            let s = s.as_bytes().as_ptr() as *const c_char;
            mrb_load_string(mrb, s);
        }
        mruby_close(mrb);
    }

    #[test]
    fn read_hash() {
        let mrb = mruby_open();
        assert!(mrb != ptr::null_mut());
        let cxt = compiler_context(mrb);
        let s = r#"
    config = {}
    config[:a] = 100
    config[:b] = "Hello"
    "#;
        unsafe {
            let s = s.as_ptr() as *const c_char;
            mrb_load_string_cxt(mrb, s, cxt);
            let config = mrb_load_string_cxt(mrb, "config".as_ptr() as *const c_char, cxt);
            assert_eq!(config.tt, mrb_vtype_MRB_TT_HASH);
            let size = mrb_funcall(mrb, config, "size".as_ptr() as *const c_char, 0, 0);
            assert_eq!(size.tt, mrb_vtype_MRB_TT_FIXNUM);
            let size = size.value.i;
            assert_eq!(size, 2);
        }
        mruby_close(mrb);
    }
}
