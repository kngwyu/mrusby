#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/hash.h"
#include "mruby/array.h"

int main() {
    mrb_state* mrb = mrb_open();
    mrbc_context* cxt = mrbc_context_new(mrb);
#define LOAD(s) mrb_load_string_cxt(mrb, s, cxt);
    LOAD("config = {}");
    LOAD("config[:a] = 100");
    LOAD("config[:b] = 'Hello'");
    mrb_value a = LOAD("config[:a]");
    printf("%ld\n", a.value.i);
    mrb_value config = LOAD("config");
    printf("%d\n", config.tt);
    struct Rhash* hash = config.value.p;
    mrb_value keys = mrb_hash_keys(mrb, config);
    printf("%d\n", keys.tt);
    struct RArray* array = keys.value.p;
    printf("%d\n", ARY_LEN(array));
    mrb_close(mrb);
    return 0;
}
