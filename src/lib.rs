#![feature(try_from)]

extern crate mruby_sys;

mod error;
mod value;

#[cfg(test)]
mod rawapi_test {
    use mruby_sys::*;
    use std::os::raw::c_char;
    use std::ffi::CStr;

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
            let s = s.as_ptr() as *const c_char;
            mrb_load_string_cxt(mrb, s, cxt);
            let config = mrb_load_string_cxt(mrb, "config".as_ptr() as *const c_char, cxt);
            assert_eq!(config.tt, mrb_vtype_MRB_TT_HASH);
            let a = mrb_load_string_cxt(mrb, "config[:a]".as_ptr() as *const c_char, cxt);
            assert_eq!(a.tt, mrb_vtype_MRB_TT_FIXNUM);
            assert_eq!(a.value.i, 100);
            let size = mrb_funcall(mrb, config, "size".as_ptr() as *const c_char, 0, 0);
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
        unsafe {
            let s = s.as_ptr() as *const c_char;
            let me = mrb_load_string_cxt(mrb, s, cxt);
            println!("{}", me.tt);
            let my_name = mrb_load_string_cxt(mrb, "Me.new.my_name".as_ptr() as *const c_char, cxt);
            println!("{}", my_name.tt);
            let my_name = {
                let c_str = mrb_str_to_cstr(mrb, my_name);
                CStr::from_ptr(c_str).to_str().unwrap()
            };
            // assert_eq!(my_name, "Me");
            println!("{}", my_name);
        }
        mruby_close(mrb);
    }
}
