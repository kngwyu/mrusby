Personal notes about mruby config

# `MRB_32BIT`, `MRB_64BIT`

## value

One of the two is declared

## description

pointer size

## default

depends on arch

# `MRB_USE_FLOAT`

## value

declared or not

## description
f64 -> f32

## default

None

# `MRB_WITHOUT_FLOAT`

## value
declared or not
## description
use only integers
## default
None

# `MRB_METHOD_CACHE`
## value
declared or not
## description
use method cache or not
## default
None

# `MRB_METHOD_CACHE_SIZE`

## value
integer(only 1 << n is accepted)
## description
method cache size
## default
None

# `MRB_METHOD_TABLE_INLINE`

## value
declared or not
## description
TODO
## default
- linux => declared
- other => None

# `MRB_INT_16`, `MRB_INT_32` `MRB_INT_64`

## value
one of 3 is declared
## description
int size
## default
64bit arch => 64
32bit => 32

# `MRB_NAN_BOXING`

## value
declared or not
## description
use NAN boxing(pack `mrb_value` into 64 bit using double)
## default
None
## note
can't use 64bit arch

# `MRB_ENDIAN_BIG`

## value
declared or
## description
as it looks
## default
None

# `MRB_WORD_`BOXING

## value
declared or
## description
use word for `mrb_value`
## default
None

# `MRB_UTF8_STRING`

## value
decalred or
## description
use utf8 string
## default
None


# `MRB_UTF8_STRING`

## value
decalred or
## description
use utf8 string
## default
None

# `MRB_FUNCALL_ARGC_MAX`

## value 
int
## description
argv max in `mrb_funcall`
## default
16

# `MRB_HEAP_PAGE_SIZE`

## value
int
## description
number of object per heap page
## default
1024

# `MRB_USE_ETEXT_EDATA`

## value
declared or
## description
TODO
## default
None

# `MRB_NO_INIT_ARRAY_START`

## value
declared or not
## description
TODO
## default
None

# `MRB_GC_TURN_OFF_GENERATIONAL`

## value
declared or not
## description
don't use generational gc
## default
None

# `KHASH_DEFAULT_SIZE`

## value
int
## description
khash table bucket size
## default
32

# `POOL_ALIGNMENT`

## value
int
## description
allocated memory address alignment
## default
4

# `POOL_PAGE_SIZE`

## value
int 
## description
paze size of memory pool
## default
16000

# `MRB_STR_BUF_MIN_SIZE`

## value
int
## description
initial minimum size of string buffer
## default
128

# `MRB_GC_ARENA_SIZE`

## value 
int
## description
size of gc arena
## default
100

# `MRB_GC_FIXED_ARENA`

## value
declared or not
## description
use fixed size gc arena
## default
None


# `MRB_FIXED_STATE_ATEXIT_STACK_SIZE`

## value
int 
## description
state atexit stack size
## default
5

# `MRB_FIXED_STATE_ATEXIT_STACK`

## value
delared or not
## description
use fixed stack size
## default
None

# `MRB_DISABLE_STDIO`

## value
declared or not
## description
don't use stdio
## default
None

# `ENABLE_DEBUG`

## value
declared or not
## description
use debug hook
## default
None




