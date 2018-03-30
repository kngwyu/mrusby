use std::os::raw::c_void;

pub trait MrbPtr {
    type Pointer;
    fn into_raw(self) -> *mut Self::Pointer;
}
