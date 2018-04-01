use mruby_sys::RException;

/// A type representing mruby Exception
pub struct MrbException<'cxt>(pub(in value) &'cxt mut RException);
