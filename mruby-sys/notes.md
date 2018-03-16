This is my notes about mruby bindings @_@

# mruby types known by Rust

## in `Object.h`

### `RBasic`
Hmm... I don't think, Bitfield is good, but, it's mruby developpers' decision, so I just follow it.
```c
struct RBasic {
  enum mrb_vtype tt:8;
  uint32_t color:3;
  uint32_t flags:21;
  struct RClass *c;
  struct RBasic *gcnext;
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
/// Required arguments signature type.
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
/// Function pointer type of custom allocator used in @see mrb_open_allocf.
///
/// The function pointing it must behave similarly as realloc except:
/// - If ptr is NULL it must allocate new space.
/// - If s is NULL, ptr must be freed.
///
/// See @see mrb_default_allocf for the default implementation.
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
# mruby types not known by Rust(incomplete)

## in `irep.h`

```c
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
    _unused: [u8; 0],
}
```

## in `throw.h`

```c
struct mrb_jmpbuf {
  mrb_jmpbuf_impl impl;

#if defined(MRB_ENABLE_CXX_EXCEPTION) && defined(__cplusplus)
  static mrb_int jmpbuf_id;
  mrb_jmpbuf() : impl(jmpbuf_id++) {}
#endif
};
```

```rust
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_jmpbuf {
    _unused: [u8; 0],
}
```


# mruby functions known by Rust

## in `value.h`

### [API] `mrb_regexp_p`
```c
MRB_API mrb_bool mrb_regexp_p(struct mrb_state*, mrb_value);
```

```rust
extern "C" {
    pub fn mrb_regexp_p(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_bool;
}
```

## in 'gc.h'

### `mrb_objspace_each_objects`
```c
void mrb_objspace_each_objects(struct mrb_state *mrb, mrb_each_object_callback *callback, void *data);
```

```rust 
extern "C" {
    pub fn mrb_objspace_each_objects(
        mrb: *mut mrb_state,
        callback: mrb_each_object_callback,
        data: *mut ::std::os::raw::c_void,
    );
}
```

### [API] `mrb_free_context`

```c
MRB_API void mrb_free_context(struct mrb_state *mrb, struct mrb_context *c);
```

```rust
extern "C" {
    pub fn mrb_free_context(mrb: *mut mrb_state, c: *mut mrb_context);
}
```

### [API] `mrb_object_dead_p`

```c
MRB_API mrb_bool
mrb_object_dead_p(struct mrb_state *mrb, struct RBasic *object);
```

```rust
extern "C" {
    pub fn mrb_object_dead_p(mrb: *mut mrb_state, object: *mut RBasic) -> mrb_bool;
}
```

