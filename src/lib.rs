extern crate mruby_sys;

#[macro_use]
mod error;
#[allow(non_upper_case_globals)]
mod value;
mod vm;

#[cfg(test)]
mod rawapi_test {
    use mruby_sys::*;
    use std::ffi::CStr;
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
    fn get_string(mrb: *mut mrb_state, val: mrb_value) -> String {
        unsafe {
            let c_str = mrb_str_to_cstr(mrb, val);
            CStr::from_ptr(c_str).to_str().unwrap().to_owned()
        }
    }
    fn cstr(s: &str) -> &CStr {
        CStr::from_bytes_with_nul(s.as_bytes()).unwrap()
    }
    fn load_str_cxt(mrb: *mut mrb_state, s: &str, ctx: *mut mrbc_context) -> mrb_value {
        let len = s.as_bytes().len();
        let ptr = s.as_ptr() as *const c_char;
        unsafe { mrb_load_nstring_cxt(mrb, ptr, len, ctx) }
    }

    #[test]
    fn read_hash() {
        let mrb = mruby_open();
        assert!(!mrb.is_null());
        let cxt = compiler_context(mrb);
        let s = r#"
        config = {}
        config[:a] = 100
        config[:b] = "Hello"
        "#;
        unsafe {
            load_str_cxt(mrb, s, cxt);
            let config = mrb_load_string_cxt(mrb, "config".as_ptr() as *const c_char, cxt);
            assert_eq!(config.tt, mrb_vtype_MRB_TT_HASH);
            let a = load_str_cxt(mrb, "config[:a]", cxt);
            assert_eq!(a.tt, mrb_vtype_MRB_TT_FIXNUM);
            assert_eq!(a.value.i, 100);
            let size = mrb_funcall(mrb, config, cstr("size\0").as_ptr(), 0, 0);
            assert_eq!(size.tt, mrb_vtype_MRB_TT_FIXNUM);
            assert_eq!(size.value.i, 2);
        }
        mruby_close(mrb);
    }
    #[test]
    fn read_class() {
        let mrb = mruby_open();
        assert!(!mrb.is_null());
        let cxt = compiler_context(mrb);
        let s = r#"
class Me
  def my_name
    'Me'
  end
end
Me
"#;
        let s2 = "Me.new.my_name";
        let me = load_str_cxt(mrb, s, cxt);
        assert_eq!(me.tt, mrb_vtype_MRB_TT_CLASS);
        let my_name = load_str_cxt(mrb, s2, cxt);
        assert_eq!(my_name.tt, mrb_vtype_MRB_TT_STRING);
        let my_name = get_string(mrb, my_name);
        assert_eq!(my_name, "Me");
        mruby_close(mrb);
    }
}
