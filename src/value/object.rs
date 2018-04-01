use mruby_sys::RObject;

/// A type representing mruby Object
pub struct MrbObject<'cxt>(pub(in value) &'cxt mut RObject);
