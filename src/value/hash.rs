use mruby_sys::RHash;

/// A type representing mruby Hash
pub struct MrbHash<'cxt>(pub(in value) &'cxt mut RHash);
