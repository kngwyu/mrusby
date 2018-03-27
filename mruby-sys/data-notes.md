# mruby types known by Rust

## in `value.h`

### `mrb_sym`

```c
typedef uint32_t mrb_sym;
```

```rust
pub type mrb_sym = u32;
```
### `mrb_bool`

```c
typedef uint8_t mrb_bool;
```

```rust
pub type mrb_bool = u8;
```

### `mrb_int`

```c
#if defined(MRB_INT64)
  typedef int64_t mrb_int;
# define MRB_INT_BIT 64
# define MRB_INT_MIN (INT64_MIN>>MRB_FIXNUM_SHIFT)
# define MRB_INT_MAX (INT64_MAX>>MRB_FIXNUM_SHIFT)
# define MRB_PRIo PRIo64
# define MRB_PRId PRId64
# define MRB_PRIx PRIx64
#elif defined(MRB_INT16)
  typedef int16_t mrb_int;
# define MRB_INT_BIT 16
# define MRB_INT_MIN (INT16_MIN>>MRB_FIXNUM_SHIFT)
# define MRB_INT_MAX (INT16_MAX>>MRB_FIXNUM_SHIFT)
# define MRB_PRIo PRIo16
# define MRB_PRId PRId16
# define MRB_PRIx PRIx16
#else
  typedef int32_t mrb_int;
# define MRB_INT_BIT 32
# define MRB_INT_MIN (INT32_MIN>>MRB_FIXNUM_SHIFT)
# define MRB_INT_MAX (INT32_MAX>>MRB_FIXNUM_SHIFT)
# define MRB_PRIo PRIo32
# define MRB_PRId PRId32
# define MRB_PRIx PRIx32
#endif
```

```rust
pub type mrb_int = i64;
```

### `mrb_float`
```c
#ifdef MRB_USE_FLOAT
  typedef float mrb_float;
#else
  typedef double mrb_float;
#endif
```

```rust
pub type mrb_float = f64;
```

### `mrb_vtype`

```c
enum mrb_vtype {
  MRB_TT_FALSE = 0,   /*   0 */
  MRB_TT_FREE,        /*   1 */
  MRB_TT_TRUE,        /*   2 */
  MRB_TT_FIXNUM,      /*   3 */
  MRB_TT_SYMBOL,      /*   4 */
  MRB_TT_UNDEF,       /*   5 */
  MRB_TT_FLOAT,       /*   6 */
  MRB_TT_CPTR,        /*   7 */
  MRB_TT_OBJECT,      /*   8 */
  MRB_TT_CLASS,       /*   9 */
  MRB_TT_MODULE,      /*  10 */
  MRB_TT_ICLASS,      /*  11 */
  MRB_TT_SCLASS,      /*  12 */
  MRB_TT_PROC,        /*  13 */
  MRB_TT_ARRAY,       /*  14 */
  MRB_TT_HASH,        /*  15 */
  MRB_TT_STRING,      /*  16 */
  MRB_TT_RANGE,       /*  17 */
  MRB_TT_EXCEPTION,   /*  18 */
  MRB_TT_FILE,        /*  19 */
  MRB_TT_ENV,         /*  20 */
  MRB_TT_DATA,        /*  21 */
  MRB_TT_FIBER,       /*  22 */
  MRB_TT_ISTRUCT,     /*  23 */
  MRB_TT_BREAK,       /*  24 */
  MRB_TT_MAXDEFINE    /*  25 */
};
```

```rust
pub const mrb_vtype_MRB_TT_FALSE: mrb_vtype = 0;
pub const mrb_vtype_MRB_TT_FREE: mrb_vtype = 1;
pub const mrb_vtype_MRB_TT_TRUE: mrb_vtype = 2;
pub const mrb_vtype_MRB_TT_FIXNUM: mrb_vtype = 3;
pub const mrb_vtype_MRB_TT_SYMBOL: mrb_vtype = 4;
pub const mrb_vtype_MRB_TT_UNDEF: mrb_vtype = 5;
pub const mrb_vtype_MRB_TT_FLOAT: mrb_vtype = 6;
pub const mrb_vtype_MRB_TT_CPTR: mrb_vtype = 7;
pub const mrb_vtype_MRB_TT_OBJECT: mrb_vtype = 8;
pub const mrb_vtype_MRB_TT_CLASS: mrb_vtype = 9;
pub const mrb_vtype_MRB_TT_MODULE: mrb_vtype = 10;
pub const mrb_vtype_MRB_TT_ICLASS: mrb_vtype = 11;
pub const mrb_vtype_MRB_TT_SCLASS: mrb_vtype = 12;
pub const mrb_vtype_MRB_TT_PROC: mrb_vtype = 13;
pub const mrb_vtype_MRB_TT_ARRAY: mrb_vtype = 14;
pub const mrb_vtype_MRB_TT_HASH: mrb_vtype = 15;
pub const mrb_vtype_MRB_TT_STRING: mrb_vtype = 16;
pub const mrb_vtype_MRB_TT_RANGE: mrb_vtype = 17;
pub const mrb_vtype_MRB_TT_EXCEPTION: mrb_vtype = 18;
pub const mrb_vtype_MRB_TT_FILE: mrb_vtype = 19;
pub const mrb_vtype_MRB_TT_ENV: mrb_vtype = 20;
pub const mrb_vtype_MRB_TT_DATA: mrb_vtype = 21;
pub const mrb_vtype_MRB_TT_FIBER: mrb_vtype = 22;
pub const mrb_vtype_MRB_TT_ISTRUCT: mrb_vtype = 23;
pub const mrb_vtype_MRB_TT_BREAK: mrb_vtype = 24;
pub const mrb_vtype_MRB_TT_MAXDEFINE: mrb_vtype = 25;
pub type mrb_vtype = u32;
```

## in `object.h`

### `RBasic`

```c
#define MRB_OBJECT_HEADER \
  enum mrb_vtype tt:8;\
  uint32_t color:3;\
  uint32_t flags:21;\
  struct RClass *c;\
  struct RBasic *gcnext

struct RBasic {
  MRB_OBJECT_HEADER;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RBasic {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
}
```
3 Bitfields
```c 
  enum mrb_vtype tt:8;
  uint32_t color:3;
  uint32_t flags:21;
```
are packed into 1 `__BindgenBitfieldUnit` and we can access them by 
```rust 
impl RBasic {
    #[inline]
    pub fn tt(&self) -> mrb_vtype {
        unsafe { ::std::mem::transmute(self._bitfield_1.get(0usize, 8u8) as u32) }
    }
    #[inline]
    pub fn set_tt(&mut self, val: mrb_vtype) {
        unsafe {
            let val: u32 = ::std::mem::transmute(val);
            self._bitfield_1.set(0usize, 8u8, val as u64)
        }
    }
    #[inline]
    pub fn color(&self) -> u32 {
        unsafe { ::std::mem::transmute(self._bitfield_1.get(8usize, 3u8) as u32) }
    }
    ...
```

### `RObject`
RBasic + vtable id

```c
struct RObject {
  enum mrb_vtype tt:8;
  uint32_t color:3;
  uint32_t flags:21;
  struct RClass *c;
  struct RBasic *gcnext;
  struct iv_tbl *iv;
};
```

```rust 
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RObject {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub iv: *mut iv_tbl,
}
```


### `RFiber`
`RBasic` + `mrb_context`, though I don't know what fiber mean..(TODO)
```c
struct RFiber {
  enum mrb_vtype tt:8;
  uint32_t color:3;
  uint32_t flags:21;
  struct RClass *c;
  struct RBasic *gcnext;
  struct mrb_context *cxt;
};
```

```rust
pub struct RFiber {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub cxt: *mut mrb_context,
}
```

## in `boxing_no.h` or `boxing_nan.h` 

### `mrb_value`
in `boxing_no.h`

```c
typedef struct mrb_value {
  union {
#ifndef MRB_WITHOUT_FLOAT
    mrb_float f;
#endif
    void *p;
    mrb_int i;
    mrb_sym sym;
  } value;
  enum mrb_vtype tt;
} mrb_value;
```

and in `boxing_nan.h`

```c
typedef struct mrb_value {
  union {
    mrb_float f;
    union {
      void *p;
      struct {
        MRB_ENDIAN_LOHI(
          uint32_t ttt;
          ,union {
            mrb_int i;
            mrb_sym sym;
          };
        )
      };
    } value;
  };
} mrb_value;
```

Now we consider only `no` version.

``` rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct mrb_value {
    pub value: mrb_value__bindgen_ty_1,
    pub tt: mrb_vtype,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union mrb_value__bindgen_ty_1 {
    pub f: mrb_float,
    pub p: *mut ::std::os::raw::c_void,
    pub i: mrb_int,
    pub sym: mrb_sym,
    _bindgen_union_align: u64,
}
``` 

## in `gc.h`

### `mrb_each_object_callback`
```c
typedef int (mrb_each_object_callback)(struct mrb_state *mrb, struct RBasic *obj, void *data);
```

```rust 
pub type mrb_each_object_callback = ::std::option::Option<
    unsafe extern "C" fn(mrb: *mut mrb_state, obj: *mut RBasic, data: *mut ::std::os::raw::c_void)
        -> ::std::os::raw::c_int,
>;
```

### `mrb_gc_state`

```c
typedef enum {
  MRB_GC_STATE_ROOT = 0,
  MRB_GC_STATE_MARK,
  MRB_GC_STATE_SWEEP
} mrb_gc_state;
```

``` rust
pub const mrb_gc_state_MRB_GC_STATE_ROOT: mrb_gc_state = 0;
pub const mrb_gc_state_MRB_GC_STATE_MARK: mrb_gc_state = 1;
pub const mrb_gc_state_MRB_GC_STATE_SWEEP: mrb_gc_state = 2;
pub type mrb_gc_state = u32;
```

### `mrb_heap_page`

```c
typedef struct mrb_heap_page {
  struct RBasic *freelist;
  struct mrb_heap_page *prev;
  struct mrb_heap_page *next;
  struct mrb_heap_page *free_next;
  struct mrb_heap_page *free_prev;
  mrb_bool old:1;
  void *objects[];
} mrb_heap_page;
```

```rust
#[repr(C)]
#[derive(Debug)]
pub struct mrb_heap_page {
    pub freelist: *mut RBasic,
    pub prev: *mut mrb_heap_page,
    pub next: *mut mrb_heap_page,
    pub free_next: *mut mrb_heap_page,
    pub free_prev: *mut mrb_heap_page,
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub objects: __IncompleteArrayField<*mut ::std::os::raw::c_void>,
    pub __bindgen_padding_0: [u8; 7usize],
}
```

### `mrb_gc`
To make matters simple, we suppose Arena Size isn't defined.
```c
typedef struct mrb_gc {
  mrb_heap_page *heaps;                /* heaps for GC */
  mrb_heap_page *sweeps;
  mrb_heap_page *free_heaps;
  size_t live; /* count of live objects */
  struct RBasic **arena;                   /* GC protection array */
  int arena_capa;
  int arena_idx;
  mrb_gc_state state; /* state of gc */
  int current_white_part; /* make white object by white_part */
  struct RBasic *gray_list; /* list of gray objects to be traversed incrementally */
  struct RBasic *atomic_gray_list; /* list of objects to be traversed atomically */
  size_t live_after_mark;
  size_t threshold;
  int interval_ratio;
  int step_ratio;
  mrb_bool iterating     :1;
  mrb_bool disabled      :1;
  mrb_bool full          :1;
  mrb_bool generational  :1;
  mrb_bool out_of_memory :1;
  size_t majorgc_old_threshold;
} mrb_gc;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_gc {
    pub heaps: *mut mrb_heap_page,
    pub sweeps: *mut mrb_heap_page,
    pub free_heaps: *mut mrb_heap_page,
    pub live: usize,
    pub arena: *mut *mut RBasic,
    pub arena_capa: ::std::os::raw::c_int,
    pub arena_idx: ::std::os::raw::c_int,
    pub state: mrb_gc_state,
    pub current_white_part: ::std::os::raw::c_int,
    pub gray_list: *mut RBasic,
    pub atomic_gray_list: *mut RBasic,
    pub live_after_mark: usize,
    pub threshold: usize,
    pub interval_ratio: ::std::os::raw::c_int,
    pub step_ratio: ::std::os::raw::c_int,
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub majorgc_old_threshold: usize,
}
```

```c
  mrb_bool iterating     :1;
  mrb_bool disabled      :1;
  mrb_bool full          :1;
  mrb_bool generational  :1;
  mrb_bool out_of_memory :1;
```
are packed into `_bitfield_1`


## in `mruby.h`

### `mrb_code`
```c
typedef uint32_t mrb_code;
```

```rust
pub type mrb_code = u32;
```

### `mrb_aspec`
```c
/**
 * Required arguments signature type.
 */
typedef uint32_t mrb_aspec;
```

```rust
pub type mrb_aspec = u32;
```

### `mrb_allocf`

```c
/**
 * Function pointer type of custom allocator used in @see mrb_open_allocf.
 *
 * The function pointing it must behave similarly as realloc except:
 * - If ptr is NULL it must allocate new space.
 * - If s is NULL, ptr must be freed.
 *
 * See @see mrb_default_allocf for the default implementation.
 */
typedef void* (*mrb_allocf) (struct mrb_state *mrb, void*, size_t, void *ud);
```

```rust
pub type mrb_allocf = ::std::option::Option<
    unsafe extern "C" fn(
        mrb: *mut mrb_state,
        arg1: *mut ::std::os::raw::c_void,
        arg2: usize,
        ud: *mut ::std::os::raw::c_void,
    ) -> *mut ::std::os::raw::c_void,
>;
```

### `mrb_call_info`

```c
typedef struct {
  mrb_sym mid;
  struct RProc *proc;
  mrb_value *stackent;
  int nregs;
  int ridx;
  int epos;
  struct REnv *env;
  mrb_code *pc;                 /* return address */
  mrb_code *err;                /* error position */
  int argc;
  int acc;
  struct RClass *target_class;
} mrb_callinfo;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_callinfo {
    pub mid: mrb_sym,
    pub proc_: *mut RProc,
    pub stackent: *mut mrb_value,
    pub nregs: ::std::os::raw::c_int,
    pub ridx: ::std::os::raw::c_int,
    pub epos: ::std::os::raw::c_int,
    pub env: *mut REnv,
    pub pc: *mut mrb_code,
    pub err: *mut mrb_code,
    pub argc: ::std::os::raw::c_int,
    pub acc: ::std::os::raw::c_int,
    pub target_class: *mut RClass,
}
```

### `mrb_fiber_state`

```c
enum mrb_fiber_state {
  MRB_FIBER_CREATED = 0,
  MRB_FIBER_RUNNING,
  MRB_FIBER_RESUMED,
  MRB_FIBER_SUSPENDED,
  MRB_FIBER_TRANSFERRED,
  MRB_FIBER_TERMINATED,
};
```

```rust
pub const mrb_fiber_state_MRB_FIBER_CREATED: mrb_fiber_state = 0;
pub const mrb_fiber_state_MRB_FIBER_RUNNING: mrb_fiber_state = 1;
pub const mrb_fiber_state_MRB_FIBER_RESUMED: mrb_fiber_state = 2;
pub const mrb_fiber_state_MRB_FIBER_SUSPENDED: mrb_fiber_state = 3;
pub const mrb_fiber_state_MRB_FIBER_TRANSFERRED: mrb_fiber_state = 4;
pub const mrb_fiber_state_MRB_FIBER_TERMINATED: mrb_fiber_state = 5;
pub type mrb_fiber_state = u32;
```

### `mrb_context`

```c
struct mrb_context {
  struct mrb_context *prev;

  mrb_value *stack;                       /* stack of virtual machine */
  mrb_value *stbase, *stend;

  mrb_callinfo *ci;
  mrb_callinfo *cibase, *ciend;

  mrb_code **rescue;                      /* exception handler stack */
  int rsize;
  struct RProc **ensure;                  /* ensure handler stack */
  int esize, eidx;

  enum mrb_fiber_state status;
  mrb_bool vmexec;
  struct RFiber *fib;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_context {
    pub prev: *mut mrb_context,
    pub stack: *mut mrb_value,
    pub stbase: *mut mrb_value,
    pub stend: *mut mrb_value,
    pub ci: *mut mrb_callinfo,
    pub cibase: *mut mrb_callinfo,
    pub ciend: *mut mrb_callinfo,
    pub rescue: *mut *mut mrb_code,
    pub rsize: ::std::os::raw::c_int,
    pub ensure: *mut *mut RProc,
    pub esize: ::std::os::raw::c_int,
    pub eidx: ::std::os::raw::c_int,
    pub status: mrb_fiber_state,
    pub vmexec: mrb_bool,
    pub fib: *mut RFiber,
}
```

### `mrb_func_t`

```c
typedef mrb_value (*mrb_func_t)(struct mrb_state *mrb, mrb_value);
```

```rust
pub type mrb_func_t =
::std::option::Option<unsafe extern "C" fn(mrb: *mut mrb_state, arg1: mrb_value) -> mrb_value>;
```

### `mrb_method_t`

```c
#ifdef MRB_METHOD_TABLE_INLINE
typedef uintptr_t mrb_method_t;
#else
typedef struct {
  mrb_bool func_p;
  union {
    struct RProc *proc;
    mrb_func_t func;
  };
} mrb_method_t;
#endif
```

```rust
pub type mrb_method_t = usize;
```

### `mrb_atexit_func`

```c
typedef void (*mrb_atexit_func)(struct mrb_state*);
```

```rust
pub type mrb_atexit_func = ::std::option::Option<unsafe extern "C" fn(arg1: *mut mrb_state)>;
```

### `mrb_state`
To make matters simple, we assume that
- `MRB_WITHOUT_FLOAT` isn't defined
- `MRB_METHOD_CACHE` isn't defined
- `MRB_ENABLE_DEBUG_HOOK` isn't defined
- `MRB_BYTECODE_DECODE_OPTION` isn't defined
- `MRB_GC_FIXED_ARENA` isn't defined
- `MRB_FIXED_STATE_ATEXIT_STACK` isn't defined

```c
typedef struct mrb_state {
  struct mrb_jmpbuf *jmp;

  uint32_t flags;
  mrb_allocf allocf;                      /* memory allocation function */
  void *allocf_ud;                        /* auxiliary data of allocf */

  struct mrb_context *c;
  struct mrb_context *root_c;
  struct iv_tbl *globals;                 /* global variable table */

  struct RObject *exc;                    /* exception */

  struct RObject *top_self;
  struct RClass *object_class;            /* Object class */
  struct RClass *class_class;
  struct RClass *module_class;
  struct RClass *proc_class;
  struct RClass *string_class;
  struct RClass *array_class;
  struct RClass *hash_class;
  struct RClass *range_class;
  struct RClass *float_class;
  struct RClass *fixnum_class;
  struct RClass *true_class;
  struct RClass *false_class;
  struct RClass *nil_class;
  struct RClass *symbol_class;
  struct RClass *kernel_module;

  struct alloca_header *mems;
  mrb_gc gc;

  mrb_sym symidx;
  struct kh_n2s *name2sym;      /* symbol hash */
  struct symbol_name *symtbl;   /* symbol table */
  size_t symcapa;

  struct RClass *eException_class;
  struct RClass *eStandardError_class;
  struct RObject *nomem_err;              /* pre-allocated NoMemoryError */
  struct RObject *stack_err;              /* pre-allocated SysStackError */
  void *ud; /* auxiliary data */
  
  mrb_atexit_func *atexit_stack;

  mrb_int atexit_stack_len;
} mrb_state;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_state {
    pub jmp: *mut mrb_jmpbuf,
    pub flags: u32,
    pub allocf: mrb_allocf,
    pub allocf_ud: *mut ::std::os::raw::c_void,
    pub c: *mut mrb_context,
    pub root_c: *mut mrb_context,
    pub globals: *mut iv_tbl,
    pub exc: *mut RObject,
    pub top_self: *mut RObject,
    pub object_class: *mut RClass,
    pub class_class: *mut RClass,
    pub module_class: *mut RClass,
    pub proc_class: *mut RClass,
    pub string_class: *mut RClass,
    pub array_class: *mut RClass,
    pub hash_class: *mut RClass,
    pub range_class: *mut RClass,
    pub float_class: *mut RClass,
    pub fixnum_class: *mut RClass,
    pub true_class: *mut RClass,
    pub false_class: *mut RClass,
    pub nil_class: *mut RClass,
    pub symbol_class: *mut RClass,
    pub kernel_module: *mut RClass,
    pub mems: *mut alloca_header,
    pub gc: mrb_gc,
    pub symidx: mrb_sym,
    pub name2sym: *mut kh_n2s,
    pub symtbl: *mut symbol_name,
    pub symcapa: usize,
    pub eException_class: *mut RClass,
    pub eStandardError_class: *mut RClass,
    pub nomem_err: *mut RObject,
    pub stack_err: *mut RObject,
    pub ud: *mut ::std::os::raw::c_void,
    pub atexit_stack: *mut mrb_atexit_func,
    pub atexit_stack_len: mrb_int,
}
```

### `call_type`

```c
typedef enum call_type {
  CALL_PUBLIC,
  CALL_FCALL,
  CALL_VCALL,
  CALL_TYPE_MAX
} call_type;
```

```rust
pub const call_type_CALL_PUBLIC: call_type = 0;
pub const call_type_CALL_FCALL: call_type = 1;
pub const call_type_CALL_VCALL: call_type = 2;
pub const call_type_CALL_TYPE_MAX: call_type = 3;
pub type call_type = u32;
```

## in `compile.h`

### `mrbc_context`

```c
typedef struct mrbc_context {
  mrb_sym *syms;
  int slen;
  char *filename;
  short lineno;
  int (*partial_hook)(struct mrb_parser_state*);
  void *partial_data;
  struct RClass *target_class;
  mrb_bool capture_errors:1;
  mrb_bool dump_result:1;
  mrb_bool no_exec:1;
  mrb_bool keep_lv:1;
  mrb_bool no_optimize:1;

  size_t parser_nerr;
} mrbc_context;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrbc_context {
    pub syms: *mut mrb_sym,
    pub slen: ::std::os::raw::c_int,
    pub filename: *mut ::std::os::raw::c_char,
    pub lineno: ::std::os::raw::c_short,
    pub partial_hook: ::std::option::Option<
        unsafe extern "C" fn(arg1: *mut mrb_parser_state) -> ::std::os::raw::c_int,
    >,
    pub partial_data: *mut ::std::os::raw::c_void,
    pub target_class: *mut RClass,
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub parser_nerr: usize,
}
```

### `mrb_ast_node`

```c
/* AST node structure */
typedef struct mrb_ast_node {
  struct mrb_ast_node *car, *cdr;
  uint16_t lineno, filename_index;
} mrb_ast_node;

```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_ast_node {
    pub car: *mut mrb_ast_node,
    pub cdr: *mut mrb_ast_node,
    pub lineno: u16,
    pub filename_index: u16,
}
```

### `mrb_lex_state_enum`

```c
/* lexer states */
enum mrb_lex_state_enum {
  EXPR_BEG,                   /* ignore newline, +/- is a sign. */
  EXPR_END,                   /* newline significant, +/- is an operator. */
  EXPR_ENDARG,                /* ditto, and unbound braces. */
  EXPR_ENDFN,                 /* ditto, and unbound braces. */
  EXPR_ARG,                   /* newline significant, +/- is an operator. */
  EXPR_CMDARG,                /* newline significant, +/- is an operator. */
  EXPR_MID,                   /* newline significant, +/- is an operator. */
  EXPR_FNAME,                 /* ignore newline, no reserved words. */
  EXPR_DOT,                   /* right after '.' or '::', no reserved words. */
  EXPR_CLASS,                 /* immediate after 'class', no here document. */
  EXPR_VALUE,                 /* alike EXPR_BEG but label is disallowed. */
  EXPR_MAX_STATE
};
```

```rust
pub const mrb_lex_state_enum_EXPR_BEG: mrb_lex_state_enum = 0;
pub const mrb_lex_state_enum_EXPR_END: mrb_lex_state_enum = 1;
pub const mrb_lex_state_enum_EXPR_ENDARG: mrb_lex_state_enum = 2;
pub const mrb_lex_state_enum_EXPR_ENDFN: mrb_lex_state_enum = 3;
pub const mrb_lex_state_enum_EXPR_ARG: mrb_lex_state_enum = 4;
pub const mrb_lex_state_enum_EXPR_CMDARG: mrb_lex_state_enum = 5;
pub const mrb_lex_state_enum_EXPR_MID: mrb_lex_state_enum = 6;
pub const mrb_lex_state_enum_EXPR_FNAME: mrb_lex_state_enum = 7;
pub const mrb_lex_state_enum_EXPR_DOT: mrb_lex_state_enum = 8;
pub const mrb_lex_state_enum_EXPR_CLASS: mrb_lex_state_enum = 9;
pub const mrb_lex_state_enum_EXPR_VALUE: mrb_lex_state_enum = 10;
pub const mrb_lex_state_enum_EXPR_MAX_STATE: mrb_lex_state_enum = 11;
pub type mrb_lex_state_enum = u32;
```

### `mrb_parser_message`

```c
/* saved error message */
struct mrb_parser_message {
  int lineno;
  int column;
  char* message;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_parser_message {
    pub lineno: ::std::os::raw::c_int,
    pub column: ::std::os::raw::c_int,
    pub message: *mut ::std::os::raw::c_char,
}
```

### `mrb_string_type`

```c
enum mrb_string_type {
  str_not_parsing  = (0),
  str_squote   = (STR_FUNC_PARSING),
  str_dquote   = (STR_FUNC_PARSING|STR_FUNC_EXPAND),
  str_regexp   = (STR_FUNC_PARSING|STR_FUNC_REGEXP|STR_FUNC_EXPAND),
  str_sword    = (STR_FUNC_PARSING|STR_FUNC_WORD|STR_FUNC_ARRAY),
  str_dword    = (STR_FUNC_PARSING|STR_FUNC_WORD|STR_FUNC_ARRAY|STR_FUNC_EXPAND),
  str_ssym     = (STR_FUNC_PARSING|STR_FUNC_SYMBOL),
  str_ssymbols = (STR_FUNC_PARSING|STR_FUNC_SYMBOL|STR_FUNC_ARRAY),
  str_dsymbols = (STR_FUNC_PARSING|STR_FUNC_SYMBOL|STR_FUNC_ARRAY|STR_FUNC_EXPAND),
  str_heredoc  = (STR_FUNC_PARSING|STR_FUNC_HEREDOC),
  str_xquote   = (STR_FUNC_PARSING|STR_FUNC_XQUOTE|STR_FUNC_EXPAND),
};
```

```rust
pub const mrb_string_type_str_not_parsing: mrb_string_type = 0;
pub const mrb_string_type_str_squote: mrb_string_type = 1;
pub const mrb_string_type_str_dquote: mrb_string_type = 3;
pub const mrb_string_type_str_regexp: mrb_string_type = 7;
pub const mrb_string_type_str_sword: mrb_string_type = 41;
pub const mrb_string_type_str_dword: mrb_string_type = 43;
pub const mrb_string_type_str_ssym: mrb_string_type = 17;
pub const mrb_string_type_str_ssymbols: mrb_string_type = 49;
pub const mrb_string_type_str_dsymbols: mrb_string_type = 51;
pub const mrb_string_type_str_heredoc: mrb_string_type = 65;
pub const mrb_string_type_str_xquote: mrb_string_type = 131;
pub type mrb_string_type = u32;
```

### `mrb_parser_heredoc_info`

```c
/* heredoc structure */
struct mrb_parser_heredoc_info {
  mrb_bool allow_indent:1;
  mrb_bool line_head:1;
  enum mrb_string_type type;
  const char *term;
  int term_len;
  mrb_ast_node *doc;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_parser_heredoc_info {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub type_: mrb_string_type,
    pub term: *const ::std::os::raw::c_char,
    pub term_len: ::std::os::raw::c_int,
    pub doc: *mut mrb_ast_node,
}
```

### `mrb_parser_state`

```c
/* parser structure */
struct mrb_parser_state {
  mrb_state *mrb;
  struct mrb_pool *pool;
  mrb_ast_node *cells;
  const char *s, *send;
#ifndef MRB_DISABLE_STDIO
  FILE *f;
#endif
  mrbc_context *cxt;
  char const *filename;
  int lineno;
  int column;

  enum mrb_lex_state_enum lstate;
  mrb_ast_node *lex_strterm; /* (type nest_level beg . end) */

  unsigned int cond_stack;
  unsigned int cmdarg_stack;
  int paren_nest;
  int lpar_beg;
  int in_def, in_single;
  mrb_bool cmd_start:1;
  mrb_ast_node *locals;

  mrb_ast_node *pb;
  char *tokbuf;
  char buf[MRB_PARSER_TOKBUF_SIZE];
  int tidx;
  int tsiz;

  mrb_ast_node *all_heredocs; /* list of mrb_parser_heredoc_info* */
  mrb_ast_node *heredocs_from_nextline;
  mrb_ast_node *parsing_heredoc;
  mrb_ast_node *lex_strterm_before_heredoc;
  mrb_bool heredoc_end_now:1; /* for mirb */

  void *ylval;

  size_t nerr;
  size_t nwarn;
  mrb_ast_node *tree;

  mrb_bool no_optimize:1;
  mrb_bool capture_errors:1;
  struct mrb_parser_message error_buffer[10];
  struct mrb_parser_message warn_buffer[10];

  mrb_sym* filename_table;
  size_t filename_table_length;
  int current_filename_index;

  struct mrb_jmpbuf* jmp;
};
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct mrb_parser_state {
    pub mrb: *mut mrb_state,
    pub pool: *mut mrb_pool,
    pub cells: *mut mrb_ast_node,
    pub s: *const ::std::os::raw::c_char,
    pub send: *const ::std::os::raw::c_char,
    pub f: *mut FILE,
    pub cxt: *mut mrbc_context,
    pub filename: *const ::std::os::raw::c_char,
    pub lineno: ::std::os::raw::c_int,
    pub column: ::std::os::raw::c_int,
    pub lstate: mrb_lex_state_enum,
    pub lex_strterm: *mut mrb_ast_node,
    pub cond_stack: ::std::os::raw::c_uint,
    pub cmdarg_stack: ::std::os::raw::c_uint,
    pub paren_nest: ::std::os::raw::c_int,
    pub lpar_beg: ::std::os::raw::c_int,
    pub in_def: ::std::os::raw::c_int,
    pub in_single: ::std::os::raw::c_int,
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub locals: *mut mrb_ast_node,
    pub pb: *mut mrb_ast_node,
    pub tokbuf: *mut ::std::os::raw::c_char,
    pub buf: [::std::os::raw::c_char; 256usize],
    pub tidx: ::std::os::raw::c_int,
    pub tsiz: ::std::os::raw::c_int,
    pub all_heredocs: *mut mrb_ast_node,
    pub heredocs_from_nextline: *mut mrb_ast_node,
    pub parsing_heredoc: *mut mrb_ast_node,
    pub lex_strterm_before_heredoc: *mut mrb_ast_node,
    pub _bitfield_2: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub ylval: *mut ::std::os::raw::c_void,
    pub nerr: usize,
    pub nwarn: usize,
    pub tree: *mut mrb_ast_node,
    pub _bitfield_3: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub error_buffer: [mrb_parser_message; 10usize],
    pub warn_buffer: [mrb_parser_message; 10usize],
    pub filename_table: *mut mrb_sym,
    pub filename_table_length: usize,
    pub current_filename_index: ::std::os::raw::c_int,
    pub jmp: *mut mrb_jmpbuf,
}
```

## in `khash.h`

### `khint_t`

```c
typedef uint32_t khint_t;
```

```rust
pub type khint_t = u32;
```

### `khiter_t`

```c
typedef khint_t khiter_t;
```

```rust
pub type khiter_t = khint_t;
```

### `kh_cstr_t`

```c
typedef const char *kh_cstr_t;
```

```rust
pub type kh_cstr_t = *const ::std::os::raw::c_char;
```

## in `hash.h`

**Note** Some of hash types are declared by macros in `khash.h` but I don't note them here.

### `Rhash`

```c
struct RHash {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  struct kh_ht *ht;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RHash {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub iv: *mut iv_tbl,
    pub ht: *mut kh_ht,
}
```

### `mrb_hash_value`

```c
typedef struct {
  mrb_value v;
  mrb_int n;
} mrb_hash_value;
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct mrb_hash_value {
    pub v: mrb_value,
    pub n: mrb_int,
}
```

## in `error.h`

### `RException`

```c
struct RException {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RException {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub iv: *mut iv_tbl,
}
```

### `RBreak`

```c
struct RBreak {
  MRB_OBJECT_HEADER;
  struct RProc *proc;
  mrb_value val;
};
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RBreak {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub proc_: *mut RProc,
    pub val: mrb_value,
}
```

## in `string.h`

### `RString`

```c
struct RString {
  MRB_OBJECT_HEADER;
  union {
    struct {
      mrb_int len;
      union {
        mrb_int capa;
        struct mrb_shared_string *shared;
        struct RString *fshared;
      } aux;
      char *ptr;
    } heap;
    char ary[RSTRING_EMBED_LEN_MAX + 1];
  } as;
};
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RString {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub as_: RString__bindgen_ty_1,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RString__bindgen_ty_1 {
    pub heap: RString__bindgen_ty_1__bindgen_ty_1,
    pub ary: [::std::os::raw::c_char; 24usize],
    _bindgen_union_align: [u64; 3usize],
}
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RString__bindgen_ty_1__bindgen_ty_1 {
    pub len: mrb_int,
    pub aux: RString__bindgen_ty_1__bindgen_ty_1__bindgen_ty_1,
    pub ptr: *mut ::std::os::raw::c_char,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RString__bindgen_ty_1__bindgen_ty_1__bindgen_ty_1 {
    pub capa: mrb_int,
    pub shared: *mut mrb_shared_string,
    pub fshared: *mut RString,
    _bindgen_union_align: u64,
}
```

## in `array.h`

### `mrb_shared_array`

```c
typedef struct mrb_shared_array {
  int refcnt;
  mrb_int len;
  mrb_value *ptr;
} mrb_shared_array;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_shared_array {
    pub refcnt: ::std::os::raw::c_int,
    pub len: mrb_int,
    pub ptr: *mut mrb_value,
}
```

### `RArray`

```c
#define MRB_ARY_EMBED_LEN_MAX ((mrb_int)(sizeof(void*)*3/sizeof(mrb_value)))
struct RArray {
  MRB_OBJECT_HEADER;
  union {
    struct {
      mrb_int len;
      union {
        mrb_int capa;
        mrb_shared_array *shared;
      } aux;
      mrb_value *ptr;
    } heap;
    mrb_value embed[MRB_ARY_EMBED_LEN_MAX];
  }
};
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RArray {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub as_: RArray__bindgen_ty_1,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RArray__bindgen_ty_1 {
    pub heap: RArray__bindgen_ty_1__bindgen_ty_1,
    pub embed: [mrb_value; 1usize],
    _bindgen_union_align: [u64; 3usize],
}
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RArray__bindgen_ty_1__bindgen_ty_1 {
    pub len: mrb_int,
    pub aux: RArray__bindgen_ty_1__bindgen_ty_1__bindgen_ty_1,
    pub ptr: *mut mrb_value,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RArray__bindgen_ty_1__bindgen_ty_1__bindgen_ty_1 {
    pub capa: mrb_int,
    pub shared: *mut mrb_shared_array,
    _bindgen_union_align: u64,
}
```

## in `class.h`

### `RClass`

```c
struct RClass {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  struct kh_mt *mt;
  struct RClass *super;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RClass {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub iv: *mut iv_tbl,
    pub mt: *mut kh_mt,
    pub super_: *mut RClass,
}
```

## in `range.h`

### `mrb_range_edges`

```c
typedef struct mrb_range_edges {
  mrb_value beg;
  mrb_value end;
} mrb_range_edges;
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct mrb_range_edges {
    pub beg: mrb_value,
    pub end: mrb_value,
}
```

### `RRange`

```c
struct RRange {
  MRB_OBJECT_HEADER;
  mrb_range_edges *edges;
  mrb_bool excl : 1;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RRange {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub edges: *mut mrb_range_edges,
    pub _bitfield_2: __BindgenBitfieldUnit<[u8; 1usize], u8>,
    pub __bindgen_padding_0: [u8; 7usize],
}
```

## in `irep.h`

### `irep_pool_type`

```c
enum irep_pool_type {
  IREP_TT_STRING,
  IREP_TT_FIXNUM,
  IREP_TT_FLOAT,
};
```

```rust
pub const irep_pool_type_IREP_TT_STRING: irep_pool_type = 0;
pub const irep_pool_type_IREP_TT_FIXNUM: irep_pool_type = 1;
pub const irep_pool_type_IREP_TT_FLOAT: irep_pool_type = 2;
pub type irep_pool_type = u32;
```

### `mrb_locals`

```c
struct mrb_locals {
  mrb_sym name;
  uint16_t r;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_locals {
    pub name: mrb_sym,
    pub r: u16,
}
```

### `mrb_irep`

```c
/* Program data array struct */
typedef struct mrb_irep {
  uint16_t nlocals;        /* Number of local variables */
  uint16_t nregs;          /* Number of register variables */
  uint8_t flags;

  mrb_code *iseq;
  mrb_value *pool;
  mrb_sym *syms;
  struct mrb_irep **reps;

  struct mrb_locals *lv;
  /* debug info */
  mrb_bool own_filename;
  const char *filename;
  uint16_t *lines;
  struct mrb_irep_debug_info* debug_info;

  int ilen, plen, slen, rlen, refcnt;
} mrb_irep;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_irep {
    pub nlocals: u16,
    pub nregs: u16,
    pub flags: u8,
    pub iseq: *mut mrb_code,
    pub pool: *mut mrb_value,
    pub syms: *mut mrb_sym,
    pub reps: *mut *mut mrb_irep,
    pub lv: *mut mrb_locals,
    pub own_filename: mrb_bool,
    pub filename: *const ::std::os::raw::c_char,
    pub lines: *mut u16,
    pub debug_info: *mut mrb_irep_debug_info,
    pub ilen: ::std::os::raw::c_int,
    pub plen: ::std::os::raw::c_int,
    pub slen: ::std::os::raw::c_int,
    pub rlen: ::std::os::raw::c_int,
    pub refcnt: ::std::os::raw::c_int,
}
```

## in `proc.h`

### `REnv`

```c
struct REnv {
  MRB_OBJECT_HEADER;
  mrb_value *stack;
  struct mrb_context *cxt;
  mrb_sym mid;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct REnv {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub stack: *mut mrb_value,
    pub cxt: *mut mrb_context,
    pub mid: mrb_sym,
}
```

### `RProc`

```c
struct RProc {
  MRB_OBJECT_HEADER;
  union {
    mrb_irep *irep;
    mrb_func_t func;
  } body;
  struct RProc *upper;
  union {
    struct RClass *target_class;
    struct REnv *env;
  } e;
};
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct RProc {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub body: RProc__bindgen_ty_1,
    pub upper: *mut RProc,
    pub e: RProc__bindgen_ty_2,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RProc__bindgen_ty_1 {
    pub irep: *mut mrb_irep,
    pub func: mrb_func_t,
    _bindgen_union_align: u64,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union RProc__bindgen_ty_2 {
    pub target_class: *mut RClass,
    pub env: *mut REnv,
    _bindgen_union_align: u64,
}
```

## in `variable.h`

### `global_variable`

```c
typedef struct global_variable {
  int   counter;
  mrb_value *data;
  mrb_value (*getter)(void);
  void  (*setter)(void);
  /* void  (*marker)(); */
  /* int block_trace; */
  /* struct trace_var *trace; */
} global_variable;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct global_variable {
    pub counter: ::std::os::raw::c_int,
    pub data: *mut mrb_value,
    pub getter: ::std::option::Option<unsafe extern "C" fn() -> mrb_value>,
    pub setter: ::std::option::Option<unsafe extern "C" fn()>,
}
```

### `global_entry`

```c
struct global_entry {
  global_variable *var;
  mrb_sym id;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct global_entry {
    pub var: *mut global_variable,
    pub id: mrb_sym,
}
```

## in `data.h`

### `mrb_data_type`

```c
typedef struct mrb_data_type {
  /** data type name */
  const char *struct_name;

  /** data type release function pointer */
  void (*dfree)(mrb_state *mrb, void*);
} mrb_data_type;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_data_type {
    /// data type name
    pub struct_name: *const ::std::os::raw::c_char,
    /// data type release function pointer
    pub dfree: ::std::option::Option<
        unsafe extern "C" fn(mrb: *mut mrb_state, arg1: *mut ::std::os::raw::c_void),
    >,
}
```

### `RData`

```c
struct RData {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  const mrb_data_type *type;
  void *data;
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RData {
    pub _bitfield_1: __BindgenBitfieldUnit<[u8; 4usize], u32>,
    pub c: *mut RClass,
    pub gcnext: *mut RBasic,
    pub iv: *mut iv_tbl,
    pub type_: *const mrb_data_type,
    pub data: *mut ::std::os::raw::c_void,
}
```

## in `debug.h`

### `mrb_debug_line_type`

```c
typedef enum mrb_debug_line_type {
  mrb_debug_line_ary = 0,
  mrb_debug_line_flat_map = 1
} mrb_debug_line_type;
```

```rust
pub const mrb_debug_line_type_mrb_debug_line_ary: mrb_debug_line_type = 0;
pub const mrb_debug_line_type_mrb_debug_line_flat_map: mrb_debug_line_type = 1;
pub type mrb_debug_line_type = u32;
```

### `mrb_irep_debug_info_line`

```c
typedef struct mrb_irep_debug_info_line {
  uint32_t start_pos;
  uint16_t line;
} mrb_irep_debug_info_line;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_irep_debug_info_line {
    pub start_pos: u32,
    pub line: u16,
}
```

### `mrb_irep_debug_info_file`

```c
typedef struct mrb_irep_debug_info_file {
  uint32_t start_pos;
  const char *filename;
  mrb_sym filename_sym;
  uint32_t line_entry_count;
  mrb_debug_line_type line_type;
  union {
    void *ptr;
    mrb_irep_debug_info_line *flat_map;
    uint16_t *ary;
  } lines;
} mrb_irep_debug_info_file;
```

```rust
#[repr(C)]
#[derive(Copy, Clone)]
pub struct mrb_irep_debug_info_file {
    pub start_pos: u32,
    pub filename: *const ::std::os::raw::c_char,
    pub filename_sym: mrb_sym,
    pub line_entry_count: u32,
    pub line_type: mrb_debug_line_type,
    pub lines: mrb_irep_debug_info_file__bindgen_ty_1,
}
#[repr(C)]
#[derive(Copy, Clone)]
pub union mrb_irep_debug_info_file__bindgen_ty_1 {
    pub ptr: *mut ::std::os::raw::c_void,
    pub flat_map: *mut mrb_irep_debug_info_line,
    pub ary: *mut u16,
    _bindgen_union_align: u64,
}
```

### `mrb_irep_debug_info`

```c
typedef struct mrb_irep_debug_info {
  uint32_t pc_count;
  uint16_t flen;
  mrb_irep_debug_info_file **files;
} mrb_irep_debug_info;
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_irep_debug_info {
    pub pc_count: u32,
    pub flen: u16,
    pub files: *mut *mut mrb_irep_debug_info_file,
}
```
