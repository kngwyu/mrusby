use error::MrbResult;
use mruby::Mruby;

pub(crate) fn with_vm(f: impl FnOnce(Mruby) -> MrbResult<()>) {
    let vm = Mruby::new().unwrap();
    f(vm).unwrap();
}
