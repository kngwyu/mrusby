/*
** mruby - An embeddable Ruby implementation
**
** Copyright (c) mruby developers 2010-2018
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

/*
** This file is combined wrapper file for bindgen.
** handmade by kngwyu and maybe includes unnecesarry parts
 */

/* start mruby.h */
#include <stdint.h>
#include <stddef.h>
#include <limits.h>


#ifdef MRB_DEBUG
#include <assert.h>
#define mrb_assert(p) assert(p)
#define mrb_assert_int_fit(t1,n,t2,max) assert((n)>=0 && ((sizeof(n)<=sizeof(t2))||(n<=(t1)(max))))
#else
#define mrb_assert(p) ((void)0)
#define mrb_assert_int_fit(t1,n,t2,max) ((void)0)
#endif

#if __STDC_VERSION__ >= 201112L
#define mrb_static_assert(exp, str) _Static_assert(exp, str)
#else
#define mrb_static_assert(exp, str) mrb_assert(exp)
#endif

#include <limits.h>
#include <stdint.h>

/* architecture selection: */
/* specify -DMRB_32BIT or -DMRB_64BIT to override */
#if !defined(MRB_32BIT) && !defined(MRB_64BIT)
#if UINT64_MAX == SIZE_MAX
#define MRB_64BIT
#else
#define MRB_32BIT
#endif
#endif

/* start mrbconf.h */
#if defined(MRB_32BIT) && defined(MRB_64BIT)
#error Cannot build for 32 and 64 bit architecture at the same time
#endif

#if !defined(MRB_METHOD_TABLE_INLINE) && defined(__linux__)
# define MRB_METHOD_TABLE_INLINE
#endif

#ifdef DISABLE_STDIO
#define MRB_DISABLE_STDIO
#endif

#ifdef ENABLE_DEBUG
#define MRB_ENABLE_DEBUG_HOOK
#endif

#ifndef MRB_DISABLE_STDIO
# include <stdio.h>
#endif

#ifndef FALSE
# define FALSE 0
#endif

#ifndef TRUE
# define TRUE 1
#endif
/* end mrbconf.h */
#ifndef MRB_WITHOUT_FLOAT
#ifndef FLT_EPSILON
#define FLT_EPSILON (1.19209290e-07f)
#endif
#ifndef DBL_EPSILON
#define DBL_EPSILON ((double)2.22044604925031308085e-16L)
#endif
#ifndef LDBL_EPSILON
#define LDBL_EPSILON (1.08420217248550443401e-19L)
#endif

#ifdef MRB_USE_FLOAT
#define MRB_FLOAT_EPSILON FLT_EPSILON
#else
#define MRB_FLOAT_EPSILON DBL_EPSILON
#endif
#endif

/* start mruby/common.h */
#ifdef __cplusplus
#ifdef MRB_ENABLE_CXX_ABI
#define MRB_BEGIN_DECL
#define MRB_END_DECL
#else
# define MRB_BEGIN_DECL extern "C" {
# define MRB_END_DECL }
#endif
#else
/** Start declarations in C mode */
# define MRB_BEGIN_DECL
/** End declarations in C mode */
# define MRB_END_DECL
#endif

/**
 * Shared compiler macros
 */
MRB_BEGIN_DECL

/** Declare a function that never returns. */
#if __STDC_VERSION__ >= 201112L
# define mrb_noreturn _Noreturn
#elif defined __GNUC__ && !defined __STRICT_ANSI__
# define mrb_noreturn __attribute__((noreturn))
#elif defined _MSC_VER
# define mrb_noreturn __declspec(noreturn)
#else
# define mrb_noreturn
#endif

/** Mark a function as deprecated. */
#if defined __GNUC__ && !defined __STRICT_ANSI__
# define mrb_deprecated __attribute__((deprecated))
#elif defined _MSC_VER
# define mrb_deprecated __declspec(deprecated)
#else
# define mrb_deprecated
#endif

/** Declare a function as always inlined. */
#if defined(_MSC_VER)
# define MRB_INLINE static __inline
#else
# define MRB_INLINE static inline
#endif


/** Declare a public MRuby API function. */
#if defined(MRB_BUILD_AS_DLL)
#if defined(MRB_CORE) || defined(MRB_LIB)
# define MRB_API __declspec(dllexport)
#else
# define MRB_API __declspec(dllimport)
#endif
#else
# define MRB_API extern
#endif

MRB_END_DECL
/* end mruby/common.h */

/* start mruby/value.h */
/**
 * MRuby Value definition functions and macros.
 */
MRB_BEGIN_DECL

typedef uint32_t mrb_sym;
typedef uint8_t mrb_bool;
struct mrb_state;

#if defined(MRB_INT16) && defined(MRB_INT64)
# error "You can't define MRB_INT16 and MRB_INT64 at the same time."
#endif

#if defined _MSC_VER && _MSC_VER < 1800
# define PRIo64 "llo"
# define PRId64 "lld"
# define PRIx64 "llx"
# define PRIo16 "ho"
# define PRId16 "hd"
# define PRIx16 "hx"
# define PRIo32 "o"
# define PRId32 "d"
# define PRIx32 "x"
#else
# include <inttypes.h>
#endif

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


#ifndef MRB_WITHOUT_FLOAT
MRB_API double mrb_float_read(const char*, char**);
#ifdef MRB_USE_FLOAT
  typedef float mrb_float;
#else
  typedef double mrb_float;
#endif
#endif

#if defined _MSC_VER && _MSC_VER < 1900
# ifndef __cplusplus
#  define inline __inline
# endif
# include <stdarg.h>
MRB_API int mrb_msvc_vsnprintf(char *s, size_t n, const char *format, va_list arg);
MRB_API int mrb_msvc_snprintf(char *s, size_t n, const char *format, ...);
# define vsnprintf(s, n, format, arg) mrb_msvc_vsnprintf(s, n, format, arg)
# define snprintf(s, n, format, ...) mrb_msvc_snprintf(s, n, format, __VA_ARGS__)
# if _MSC_VER < 1800 && !defined MRB_WITHOUT_FLOAT
#  include <float.h>
#  define isfinite(n) _finite(n)
#  define isnan _isnan
#  define isinf(n) (!_finite(n) && !_isnan(n))
#  define signbit(n) (_copysign(1.0, (n)) < 0.0)
static const unsigned int IEEE754_INFINITY_BITS_SINGLE = 0x7F800000;
#  define INFINITY (*(float *)&IEEE754_INFINITY_BITS_SINGLE)
#  define NAN ((float)(INFINITY - INFINITY))
# endif
#endif

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

/* start mruby/object.h */
#define MRB_OBJECT_HEADER \
  enum mrb_vtype tt:8;\
  uint32_t color:3;\
  uint32_t flags:21;\
  struct RClass *c;\
  struct RBasic *gcnext
#define MRB_FLAG_TEST(obj, flag) ((obj)->flags & flag)
struct RBasic {
  MRB_OBJECT_HEADER;
};
#define mrb_basic_ptr(v) ((struct RBasic*)(mrb_ptr(v)))
/* flags bits >= 18 is reserved */
#define MRB_FLAG_IS_FROZEN (1 << 18)
#define MRB_FROZEN_P(o) ((o)->flags & MRB_FLAG_IS_FROZEN)
#define MRB_SET_FROZEN_FLAG(o) ((o)->flags |= MRB_FLAG_IS_FROZEN)
#define MRB_UNSET_FROZEN_FLAG(o) ((o)->flags &= ~MRB_FLAG_IS_FROZEN)
struct RObject {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
};
#define mrb_obj_ptr(v)   ((struct RObject*)(mrb_ptr(v)))
#define mrb_immediate_p(x) (mrb_type(x) < MRB_TT_HAS_BASIC)
#define mrb_special_const_p(x) mrb_immediate_p(x)
struct RFiber {
  MRB_OBJECT_HEADER;
  struct mrb_context *cxt;
};
/* end mruby/object.h */

#ifdef MRB_DOCUMENTATION_BLOCK

typedef void mrb_value;

#endif

#if defined(MRB_NAN_BOXING)
/* start mruby/boxing_nan.h */
#ifdef MRB_USE_FLOAT
# error ---->> MRB_NAN_BOXING and MRB_USE_FLOAT conflict <<----
#endif

#ifdef MRB_WITHOUT_FLOAT
# error ---->> MRB_NAN_BOXING and MRB_WITHOUT_FLOAT conflict <<----
#endif

#ifdef MRB_INT64
# error ---->> MRB_NAN_BOXING and MRB_INT64 conflict <<----
#endif

#define MRB_FIXNUM_SHIFT 0
#define MRB_TT_HAS_BASIC MRB_TT_OBJECT

#ifdef MRB_ENDIAN_BIG
#define MRB_ENDIAN_LOHI(a,b) a b
#else
#define MRB_ENDIAN_LOHI(a,b) b a
#endif

/* value representation by nan-boxing:
 *   float : FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF
 *   object: 111111111111TTTT TTPPPPPPPPPPPPPP PPPPPPPPPPPPPPPP PPPPPPPPPPPPPPPP
 *   int   : 1111111111110001 0000000000000000 IIIIIIIIIIIIIIII IIIIIIIIIIIIIIII
 *   sym   : 1111111111110001 0100000000000000 SSSSSSSSSSSSSSSS SSSSSSSSSSSSSSSS
 * In order to get enough bit size to save TT, all pointers are shifted 2 bits
 * in the right direction. Also, TTTTTT is the mrb_vtype + 1;
 */
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

#define mrb_float_pool(mrb,f) mrb_float_value(mrb,f)

#define mrb_tt(o)       ((enum mrb_vtype)(((o).value.ttt & 0xfc000)>>14)-1)
#define mrb_type(o)     (enum mrb_vtype)((uint32_t)0xfff00000 < (o).value.ttt ? mrb_tt(o) : MRB_TT_FLOAT)
#define mrb_ptr(o)      ((void*)((((uintptr_t)0x3fffffffffff)&((uintptr_t)((o).value.p)))<<2))
#define mrb_float(o)    (o).f
#define mrb_cptr(o)     mrb_ptr(o)
#define mrb_fixnum(o)   (o).value.i
#define mrb_symbol(o)   (o).value.sym

#ifdef MRB_64BIT
#define BOXNAN_SHIFT_LONG_POINTER(v) (((uintptr_t)(v)>>34)&0x3fff)
#else
#define BOXNAN_SHIFT_LONG_POINTER(v) 0
#endif

#define BOXNAN_SET_VALUE(o, tt, attr, v) do {\
  (o).attr = (v);\
  (o).value.ttt = 0xfff00000 | (((tt)+1)<<14);\
} while (0)

#define BOXNAN_SET_OBJ_VALUE(o, tt, v) do {\
  (o).value.p = (void*)((uintptr_t)(v)>>2);\
  (o).value.ttt = (0xfff00000|(((tt)+1)<<14)|BOXNAN_SHIFT_LONG_POINTER(v));\
} while (0)

#define SET_FLOAT_VALUE(mrb,r,v) do { \
  if (v != v) { \
    (r).value.ttt = 0x7ff80000; \
    (r).value.i = 0; \
  } \
  else { \
    (r).f = v; \
  }} while(0)

#define SET_NIL_VALUE(r) BOXNAN_SET_VALUE(r, MRB_TT_FALSE, value.i, 0)
#define SET_FALSE_VALUE(r) BOXNAN_SET_VALUE(r, MRB_TT_FALSE, value.i, 1)
#define SET_TRUE_VALUE(r) BOXNAN_SET_VALUE(r, MRB_TT_TRUE, value.i, 1)
#define SET_BOOL_VALUE(r,b) BOXNAN_SET_VALUE(r, b ? MRB_TT_TRUE : MRB_TT_FALSE, value.i, 1)
#define SET_INT_VALUE(r,n) BOXNAN_SET_VALUE(r, MRB_TT_FIXNUM, value.i, (n))
#define SET_SYM_VALUE(r,v) BOXNAN_SET_VALUE(r, MRB_TT_SYMBOL, value.sym, (v))
#define SET_OBJ_VALUE(r,v) BOXNAN_SET_OBJ_VALUE(r, (((struct RObject*)(v))->tt), (v))
#define SET_CPTR_VALUE(mrb,r,v) BOXNAN_SET_OBJ_VALUE(r, MRB_TT_CPTR, v)
#define SET_UNDEF_VALUE(r) BOXNAN_SET_VALUE(r, MRB_TT_UNDEF, value.i, 0)
/* end mruby/boxing_nan.h */
#elif defined(MRB_WORD_BOXING)
/* start mruby/boxing_word.h */
#if defined(MRB_INT16)
# error MRB_INT16 is too small for MRB_WORD_BOXING.
#endif

#if defined(MRB_INT64) && !defined(MRB_64BIT)
#error MRB_INT64 cannot be used with MRB_WORD_BOXING in 32-bit mode.
#endif

#ifndef MRB_WITHOUT_FLOAT
struct RFloat {
  MRB_OBJECT_HEADER;
  mrb_float f;
};
#endif

struct RCptr {
  MRB_OBJECT_HEADER;
  void *p;
};

#define MRB_FIXNUM_SHIFT 1
#ifdef MRB_WITHOUT_FLOAT
#define MRB_TT_HAS_BASIC MRB_TT_CPTR
#else
#define MRB_TT_HAS_BASIC MRB_TT_FLOAT
#endif

enum mrb_special_consts {
  MRB_Qnil    = 0,
  MRB_Qfalse  = 2,
  MRB_Qtrue   = 4,
  MRB_Qundef  = 6,
};

#define MRB_FIXNUM_FLAG   0x01
#define MRB_SYMBOL_FLAG   0x0e
#define MRB_SPECIAL_SHIFT 8

typedef union mrb_value {
  union {
    void *p;
    struct {
      unsigned int i_flag : MRB_FIXNUM_SHIFT;
      mrb_int i : (MRB_INT_BIT - MRB_FIXNUM_SHIFT);
    };
    struct {
      unsigned int sym_flag : MRB_SPECIAL_SHIFT;
      mrb_sym sym : (sizeof(mrb_sym) * CHAR_BIT);
    };
    struct RBasic *bp;
#ifndef MRB_WITHOUT_FLOAT
    struct RFloat *fp;
#endif
    struct RCptr *vp;
  } value;
  unsigned long w;
} mrb_value;

MRB_API mrb_value mrb_word_boxing_cptr_value(struct mrb_state*, void*);
#ifndef MRB_WITHOUT_FLOAT
MRB_API mrb_value mrb_word_boxing_float_value(struct mrb_state*, mrb_float);
MRB_API mrb_value mrb_word_boxing_float_pool(struct mrb_state*, mrb_float);
#endif

#ifndef MRB_WITHOUT_FLOAT
#define mrb_float_pool(mrb,f) mrb_word_boxing_float_pool(mrb,f)
#endif

#define mrb_ptr(o)     (o).value.p
#define mrb_cptr(o)    (o).value.vp->p
#ifndef MRB_WITHOUT_FLOAT
#define mrb_float(o)   (o).value.fp->f
#endif
#define mrb_fixnum(o)  ((mrb_int)(o).value.i)
#define mrb_symbol(o)  (o).value.sym

static inline enum mrb_vtype
mrb_type(mrb_value o)
{
  switch (o.w) {
  case MRB_Qfalse:
  case MRB_Qnil:
    return MRB_TT_FALSE;
  case MRB_Qtrue:
    return MRB_TT_TRUE;
  case MRB_Qundef:
    return MRB_TT_UNDEF;
  }
  if (o.value.i_flag == MRB_FIXNUM_FLAG) {
    return MRB_TT_FIXNUM;
  }
  if (o.value.sym_flag == MRB_SYMBOL_FLAG) {
    return MRB_TT_SYMBOL;
  }
  return o.value.bp->tt;
}

#define mrb_bool(o)    ((o).w != MRB_Qnil && (o).w != MRB_Qfalse)
#define mrb_fixnum_p(o) ((o).value.i_flag == MRB_FIXNUM_FLAG)
#define mrb_undef_p(o) ((o).w == MRB_Qundef)
#define mrb_nil_p(o)  ((o).w == MRB_Qnil)

#define BOXWORD_SET_VALUE(o, ttt, attr, v) do { \
  switch (ttt) {\
  case MRB_TT_FALSE:  (o).w = (v) ? MRB_Qfalse : MRB_Qnil; break;\
  case MRB_TT_TRUE:   (o).w = MRB_Qtrue; break;\
  case MRB_TT_UNDEF:  (o).w = MRB_Qundef; break;\
  case MRB_TT_FIXNUM: (o).w = 0;(o).value.i_flag = MRB_FIXNUM_FLAG; (o).attr = (v); break;\
  case MRB_TT_SYMBOL: (o).w = 0;(o).value.sym_flag = MRB_SYMBOL_FLAG; (o).attr = (v); break;\
  default:            (o).w = 0; (o).attr = (v); if ((o).value.bp) (o).value.bp->tt = ttt; break;\
  }\
} while (0)

#ifndef MRB_WITHOUT_FLOAT
#define SET_FLOAT_VALUE(mrb,r,v) r = mrb_word_boxing_float_value(mrb, v)
#endif
#define SET_CPTR_VALUE(mrb,r,v) r = mrb_word_boxing_cptr_value(mrb, v)
#define SET_NIL_VALUE(r) BOXWORD_SET_VALUE(r, MRB_TT_FALSE, value.i, 0)
#define SET_FALSE_VALUE(r) BOXWORD_SET_VALUE(r, MRB_TT_FALSE, value.i, 1)
#define SET_TRUE_VALUE(r) BOXWORD_SET_VALUE(r, MRB_TT_TRUE, value.i, 1)
#define SET_BOOL_VALUE(r,b) BOXWORD_SET_VALUE(r, b ? MRB_TT_TRUE : MRB_TT_FALSE, value.i, 1)
#define SET_INT_VALUE(r,n) BOXWORD_SET_VALUE(r, MRB_TT_FIXNUM, value.i, (n))
#define SET_SYM_VALUE(r,v) BOXWORD_SET_VALUE(r, MRB_TT_SYMBOL, value.sym, (v))
#define SET_OBJ_VALUE(r,v) BOXWORD_SET_VALUE(r, (((struct RObject*)(v))->tt), value.p, (v))
#define SET_UNDEF_VALUE(r) BOXWORD_SET_VALUE(r, MRB_TT_UNDEF, value.i, 0)
/* end mruby/boxing_word.h */
#else
/* start mruby/boxing_no.h */
#define MRB_FIXNUM_SHIFT 0
#define MRB_TT_HAS_BASIC MRB_TT_OBJECT

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

#ifndef MRB_WITHOUT_FLOAT
#define mrb_float_pool(mrb,f) mrb_float_value(mrb,f)
#endif

#define mrb_ptr(o)      (o).value.p
#define mrb_cptr(o)     mrb_ptr(o)
#ifndef MRB_WITHOUT_FLOAT
#define mrb_float(o)    (o).value.f
#endif
#define mrb_fixnum(o)   (o).value.i
#define mrb_symbol(o)   (o).value.sym
#define mrb_type(o)     (o).tt

#define BOXNIX_SET_VALUE(o, ttt, attr, v) do {\
  (o).tt = ttt;\
  (o).attr = v;\
} while (0)

#define SET_NIL_VALUE(r) BOXNIX_SET_VALUE(r, MRB_TT_FALSE, value.i, 0)
#define SET_FALSE_VALUE(r) BOXNIX_SET_VALUE(r, MRB_TT_FALSE, value.i, 1)
#define SET_TRUE_VALUE(r) BOXNIX_SET_VALUE(r, MRB_TT_TRUE, value.i, 1)
#define SET_BOOL_VALUE(r,b) BOXNIX_SET_VALUE(r, b ? MRB_TT_TRUE : MRB_TT_FALSE, value.i, 1)
#define SET_INT_VALUE(r,n) BOXNIX_SET_VALUE(r, MRB_TT_FIXNUM, value.i, (n))
#ifndef MRB_WITHOUT_FLOAT
#define SET_FLOAT_VALUE(mrb,r,v) BOXNIX_SET_VALUE(r, MRB_TT_FLOAT, value.f, (v))
#endif
#define SET_SYM_VALUE(r,v) BOXNIX_SET_VALUE(r, MRB_TT_SYMBOL, value.sym, (v))
#define SET_OBJ_VALUE(r,v) BOXNIX_SET_VALUE(r, (((struct RObject*)(v))->tt), value.p, (v))
#define SET_CPTR_VALUE(mrb,r,v) BOXNIX_SET_VALUE(r, MRB_TT_CPTR, value.p, v)
#define SET_UNDEF_VALUE(r) BOXNIX_SET_VALUE(r, MRB_TT_UNDEF, value.i, 0)
/* end mruby/boxing_no.h */
#endif

#ifndef mrb_fixnum_p
#define mrb_fixnum_p(o) (mrb_type(o) == MRB_TT_FIXNUM)
#endif
#ifndef mrb_undef_p
#define mrb_undef_p(o) (mrb_type(o) == MRB_TT_UNDEF)
#endif
#ifndef mrb_nil_p
#define mrb_nil_p(o)  (mrb_type(o) == MRB_TT_FALSE && !mrb_fixnum(o))
#endif
#ifndef mrb_bool
#define mrb_bool(o)   (mrb_type(o) != MRB_TT_FALSE)
#endif
#ifndef MRB_WITHOUT_FLOAT
#define mrb_float_p(o) (mrb_type(o) == MRB_TT_FLOAT)
#endif
#define mrb_symbol_p(o) (mrb_type(o) == MRB_TT_SYMBOL)
#define mrb_array_p(o) (mrb_type(o) == MRB_TT_ARRAY)
#define mrb_string_p(o) (mrb_type(o) == MRB_TT_STRING)
#define mrb_hash_p(o) (mrb_type(o) == MRB_TT_HASH)
#define mrb_cptr_p(o) (mrb_type(o) == MRB_TT_CPTR)
#define mrb_exception_p(o) (mrb_type(o) == MRB_TT_EXCEPTION)
#define mrb_test(o)   mrb_bool(o)
MRB_API mrb_bool mrb_regexp_p(struct mrb_state*, mrb_value);

/*
 * Returns a float in Ruby.
 */
#ifndef MRB_WITHOUT_FLOAT
MRB_INLINE mrb_value mrb_float_value(struct mrb_state *mrb, mrb_float f)
{
  mrb_value v;
  (void) mrb;
  SET_FLOAT_VALUE(mrb, v, f);
  return v;
}
#endif

static inline mrb_value
mrb_cptr_value(struct mrb_state *mrb, void *p)
{
  mrb_value v;
  (void) mrb;
  SET_CPTR_VALUE(mrb,v,p);
  return v;
}

/*
 * Returns a fixnum in Ruby.
 */
MRB_INLINE mrb_value mrb_fixnum_value(mrb_int i)
{
  mrb_value v;
  SET_INT_VALUE(v, i);
  return v;
}

static inline mrb_value
mrb_symbol_value(mrb_sym i)
{
  mrb_value v;
  SET_SYM_VALUE(v, i);
  return v;
}

static inline mrb_value
mrb_obj_value(void *p)
{
  mrb_value v;
  SET_OBJ_VALUE(v, (struct RBasic*)p);
  mrb_assert(p == mrb_ptr(v));
  mrb_assert(((struct RBasic*)p)->tt == mrb_type(v));
  return v;
}


/*
 * Get a nil mrb_value object.
 *
 * @return
 *      nil mrb_value object reference.
 */
MRB_INLINE mrb_value mrb_nil_value(void)
{
  mrb_value v;
  SET_NIL_VALUE(v);
  return v;
}

/*
 * Returns false in Ruby.
 */
MRB_INLINE mrb_value mrb_false_value(void)
{
  mrb_value v;
  SET_FALSE_VALUE(v);
  return v;
}

/*
 * Returns true in Ruby.
 */
MRB_INLINE mrb_value mrb_true_value(void)
{
  mrb_value v;
  SET_TRUE_VALUE(v);
  return v;
}

static inline mrb_value
mrb_bool_value(mrb_bool boolean)
{
  mrb_value v;
  SET_BOOL_VALUE(v, boolean);
  return v;
}

static inline mrb_value
mrb_undef_value(void)
{
  mrb_value v;
  SET_UNDEF_VALUE(v);
  return v;
}

#ifdef MRB_USE_ETEXT_EDATA
#if (defined(__APPLE__) && defined(__MACH__))
#include <mach-o/getsect.h>
static inline mrb_bool
mrb_ro_data_p(const char *p)
{
  return (const char*)get_etext() < p && p < (const char*)get_edata();
}
#else
extern char _etext[];
#ifdef MRB_NO_INIT_ARRAY_START
extern char _edata[];

static inline mrb_bool
mrb_ro_data_p(const char *p)
{
  return _etext < p && p < _edata;
}
#else
extern char __init_array_start[];

static inline mrb_bool
mrb_ro_data_p(const char *p)
{
  return _etext < p && p < (char*)&__init_array_start;
}
#endif
#endif
#else
# define mrb_ro_data_p(p) FALSE
#endif

MRB_END_DECL
/* end mruby/value.h */

/* start mruby/gc.h */
MRB_BEGIN_DECL


struct mrb_state;

#define MRB_EACH_OBJ_OK 0
#define MRB_EACH_OBJ_BREAK 1
typedef int (mrb_each_object_callback)(struct mrb_state *mrb, struct RBasic *obj, void *data);
void mrb_objspace_each_objects(struct mrb_state *mrb, mrb_each_object_callback *callback, void *data);
MRB_API void mrb_free_context(struct mrb_state *mrb, struct mrb_context *c);

#ifndef MRB_GC_ARENA_SIZE
#define MRB_GC_ARENA_SIZE 100
#endif

typedef enum {
  MRB_GC_STATE_ROOT = 0,
  MRB_GC_STATE_MARK,
  MRB_GC_STATE_SWEEP
} mrb_gc_state;

/* Disable MSVC warning "C4200: nonstandard extension used: zero-sized array
 * in struct/union" when in C++ mode */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4200)
#endif

typedef struct mrb_heap_page {
  struct RBasic *freelist;
  struct mrb_heap_page *prev;
  struct mrb_heap_page *next;
  struct mrb_heap_page *free_next;
  struct mrb_heap_page *free_prev;
  mrb_bool old:1;
  void *objects[];
} mrb_heap_page;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

typedef struct mrb_gc {
  mrb_heap_page *heaps;                /* heaps for GC */
  mrb_heap_page *sweeps;
  mrb_heap_page *free_heaps;
  size_t live; /* count of live objects */
#ifdef MRB_GC_FIXED_ARENA
  struct RBasic *arena[MRB_GC_ARENA_SIZE]; /* GC protection array */
#else
  struct RBasic **arena;                   /* GC protection array */
  int arena_capa;
#endif
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

MRB_API mrb_bool
mrb_object_dead_p(struct mrb_state *mrb, struct RBasic *object);

MRB_END_DECL
/* end mruby/gc.h */

/* start mruby/version.h */
/**
 * mruby version definition macros
 */
MRB_BEGIN_DECL

/*
 * A passed in expression.
 */
#define MRB_STRINGIZE0(expr) #expr

/*
 * Passes in an expression to MRB_STRINGIZE0.
 */
#define MRB_STRINGIZE(expr) MRB_STRINGIZE0(expr)

/*
 * The version of Ruby used by mruby.
 */
#define MRUBY_RUBY_VERSION "1.9"

/*
 * Ruby engine.
 */
#define MRUBY_RUBY_ENGINE  "mruby"

/*
 * Major release version number.
 */
#define MRUBY_RELEASE_MAJOR 1

/*
 * Minor release version number.
 */
#define MRUBY_RELEASE_MINOR 4

/*
 * Tiny release version number.
 */
#define MRUBY_RELEASE_TEENY 0

/*
 * The mruby version.
 */
#define MRUBY_VERSION MRB_STRINGIZE(MRUBY_RELEASE_MAJOR) "." MRB_STRINGIZE(MRUBY_RELEASE_MINOR) "." MRB_STRINGIZE(MRUBY_RELEASE_TEENY)

/*
 * Release number.
 */
#define MRUBY_RELEASE_NO (MRUBY_RELEASE_MAJOR * 100 * 100 + MRUBY_RELEASE_MINOR * 100 + MRUBY_RELEASE_TEENY)

/*
 * Release year.
 */
#define MRUBY_RELEASE_YEAR 2018

/*
 * Release month.
 */
#define MRUBY_RELEASE_MONTH 1

/*
 * Release day.
 */
#define MRUBY_RELEASE_DAY 16

/*
 * Release date as a string.
 */
#define MRUBY_RELEASE_DATE MRB_STRINGIZE(MRUBY_RELEASE_YEAR) "-" MRB_STRINGIZE(MRUBY_RELEASE_MONTH) "-" MRB_STRINGIZE(MRUBY_RELEASE_DAY)

/*
 * The year mruby was first created.
 */
#define MRUBY_BIRTH_YEAR 2010

/*
 * MRuby's authors.
 */
#define MRUBY_AUTHOR "mruby developers"

/*
 * mruby's version, and release date.
 */
#define MRUBY_DESCRIPTION      \
  "mruby " MRUBY_VERSION       \
  " (" MRUBY_RELEASE_DATE ") " \

/*
 * mruby's copyright information.
 */
#define MRUBY_COPYRIGHT                \
  "mruby - Copyright (c) "             \
  MRB_STRINGIZE(MRUBY_BIRTH_YEAR)"-"   \
  MRB_STRINGIZE(MRUBY_RELEASE_YEAR)" " \
  MRUBY_AUTHOR                         \

MRB_END_DECL
/* end mruby/version.h */

/**
 * MRuby C API entry point
 */
MRB_BEGIN_DECL

typedef uint32_t mrb_code;

/**
 * Required arguments signature type.
 */
typedef uint32_t mrb_aspec;


struct mrb_irep;
struct mrb_state;

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

#ifndef MRB_FIXED_STATE_ATEXIT_STACK_SIZE
#define MRB_FIXED_STATE_ATEXIT_STACK_SIZE 5
#endif

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

enum mrb_fiber_state {
  MRB_FIBER_CREATED = 0,
  MRB_FIBER_RUNNING,
  MRB_FIBER_RESUMED,
  MRB_FIBER_SUSPENDED,
  MRB_FIBER_TRANSFERRED,
  MRB_FIBER_TERMINATED,
};

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

#ifdef MRB_METHOD_CACHE_SIZE
# define MRB_METHOD_CACHE
#else
/* default method cache size: 128 */
/* cache size needs to be power of 2 */
# define MRB_METHOD_CACHE_SIZE (1<<7)
#endif

typedef mrb_value (*mrb_func_t)(struct mrb_state *mrb, mrb_value);

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

#ifdef MRB_METHOD_CACHE
struct mrb_cache_entry {
  struct RClass *c, *c0;
  mrb_sym mid;
  mrb_method_t m;
};
#endif

struct mrb_jmpbuf;

typedef void (*mrb_atexit_func)(struct mrb_state*);

#define MRB_STATE_NO_REGEXP 1
#define MRB_STATE_REGEXP    2

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

#ifndef MRB_WITHOUT_FLOAT
  struct RClass *float_class;
#endif
  struct RClass *fixnum_class;
  struct RClass *true_class;
  struct RClass *false_class;
  struct RClass *nil_class;
  struct RClass *symbol_class;
  struct RClass *kernel_module;

  struct alloca_header *mems;
  mrb_gc gc;

#ifdef MRB_METHOD_CACHE
  struct mrb_cache_entry cache[MRB_METHOD_CACHE_SIZE];
#endif

  mrb_sym symidx;
  struct kh_n2s *name2sym;      /* symbol hash */
  struct symbol_name *symtbl;   /* symbol table */
  size_t symcapa;

#ifdef MRB_ENABLE_DEBUG_HOOK
  void (*code_fetch_hook)(struct mrb_state* mrb, struct mrb_irep *irep, mrb_code *pc, mrb_value *regs);
  void (*debug_op_hook)(struct mrb_state* mrb, struct mrb_irep *irep, mrb_code *pc, mrb_value *regs);
#endif

#ifdef MRB_BYTECODE_DECODE_OPTION
  mrb_code (*bytecode_decoder)(struct mrb_state* mrb, mrb_code code);
#endif

  struct RClass *eException_class;
  struct RClass *eStandardError_class;
  struct RObject *nomem_err;              /* pre-allocated NoMemoryError */
  struct RObject *stack_err;              /* pre-allocated SysStackError */
#ifdef MRB_GC_FIXED_ARENA
  struct RObject *arena_err;              /* pre-allocated arena overfow error */
#endif

  void *ud; /* auxiliary data */

#ifdef MRB_FIXED_STATE_ATEXIT_STACK
  mrb_atexit_func atexit_stack[MRB_FIXED_STATE_ATEXIT_STACK_SIZE];
#else
  mrb_atexit_func *atexit_stack;
#endif
  mrb_int atexit_stack_len;
} mrb_state;

/**
 * Defines a new class.
 *
 * If you're creating a gem it may look something like this:
 *
 *      !!!c
 *      void mrb_example_gem_init(mrb_state* mrb) {
 *          struct RClass *example_class;
 *          example_class = mrb_define_class(mrb, "Example_Class", mrb->object_class);
 *      }
 *
 *      void mrb_example_gem_final(mrb_state* mrb) {
 *          //free(TheAnimals);
 *      }
 *
 * @param [mrb_state *] mrb The current mruby state.
 * @param [const char *] name The name of the defined class.
 * @param [struct RClass *] super The new class parent.
 * @return [struct RClass *] Reference to the newly defined class.
 * @see mrb_define_class_under
 */
MRB_API struct RClass *mrb_define_class(mrb_state *mrb, const char *name, struct RClass *super);

/**
 * Defines a new module.
 *
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [const char *] char* The name of the module.
 * @return [struct RClass *] Reference to the newly defined module.
 */
MRB_API struct RClass *mrb_define_module(mrb_state *, const char*);
MRB_API mrb_value mrb_singleton_class(mrb_state*, mrb_value);

/**
 * Include a module in another class or module.
 * Equivalent to:
 *
 *   module B
 *     include A
 *   end
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [struct RClass *] RClass* A reference to module or a class.
 * @param [struct RClass *] RClass* A reference to the module to be included.
 */
MRB_API void mrb_include_module(mrb_state*, struct RClass*, struct RClass*);

/**
 * Prepends a module in another class or module.
 *
 * Equivalent to:
 *  module B
 *    prepend A
 *  end
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [struct RClass *] RClass* A reference to module or a class.
 * @param [struct RClass *] RClass* A reference to the module to be prepended.
 */
MRB_API void mrb_prepend_module(mrb_state*, struct RClass*, struct RClass*);

/**
 * Defines a global function in ruby.
 *
 * If you're creating a gem it may look something like this
 *
 * Example:
 *
 *     !!!c
 *     mrb_value example_method(mrb_state* mrb, mrb_value self)
 *     {
 *          puts("Executing example command!");
 *          return self;
 *     }
 *
 *     void mrb_example_gem_init(mrb_state* mrb)
 *     {
 *           mrb_define_method(mrb, mrb->kernel_module, "example_method", example_method, MRB_ARGS_NONE());
 *     }
 *
 * @param [mrb_state *] mrb The MRuby state reference.
 * @param [struct RClass *] cla The class pointer where the method will be defined.
 * @param [const char *] name The name of the method being defined.
 * @param [mrb_func_t] func The function pointer to the method definition.
 * @param [mrb_aspec] aspec The method parameters declaration.
 */
MRB_API void mrb_define_method(mrb_state *mrb, struct RClass *cla, const char *name, mrb_func_t func, mrb_aspec aspec);

/**
 * Defines a class method.
 *
 * Example:
 *
 *     # Ruby style
 *     class Foo
 *       def Foo.bar
 *       end
 *     end
 *     // C style
 *     mrb_value bar_method(mrb_state* mrb, mrb_value self){
 *       return mrb_nil_value();
 *     }
 *     void mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *foo;
 *       foo = mrb_define_class(mrb, "Foo", mrb->object_class);
 *       mrb_define_class_method(mrb, foo, "bar", bar_method, MRB_ARGS_NONE());
 *     }
 * @param [mrb_state *] mrb_state* The MRuby state reference.
 * @param [struct RClass *] RClass* The class where the class method will be defined.
 * @param [const char *] char* The name of the class method being defined.
 * @param [mrb_func_t] mrb_func_t The function pointer to the class method definition.
 * @param [mrb_aspec] mrb_aspec The method parameters declaration.
 */
MRB_API void mrb_define_class_method(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec);
MRB_API void mrb_define_singleton_method(mrb_state*, struct RObject*, const char*, mrb_func_t, mrb_aspec);

/**
 *  Defines a module fuction.
 *
 * Example:
 *
 *        # Ruby style
 *        module Foo
 *          def Foo.bar
 *          end
 *        end
 *        // C style
 *        mrb_value bar_method(mrb_state* mrb, mrb_value self){
 *          return mrb_nil_value();
 *        }
 *        void mrb_example_gem_init(mrb_state* mrb){
 *          struct RClass *foo;
 *          foo = mrb_define_module(mrb, "Foo");
 *          mrb_define_module_function(mrb, foo, "bar", bar_method, MRB_ARGS_NONE());
 *        }
 *  @param [mrb_state *] mrb_state* The MRuby state reference.
 *  @param [struct RClass *] RClass* The module where the module function will be defined.
 *  @param [const char *] char* The name of the module function being defined.
 *  @param [mrb_func_t] mrb_func_t The function pointer to the module function definition.
 *  @param [mrb_aspec] mrb_aspec The method parameters declaration.
 */
MRB_API void mrb_define_module_function(mrb_state*, struct RClass*, const char*, mrb_func_t, mrb_aspec);

/**
 *  Defines a constant.
 *
 * Example:
 *
 *          # Ruby style
 *          class ExampleClass
 *            AGE = 22
 *          end
 *          // C style
 *          #include <stdio.h>
 *          #include <mruby.h>
 *
 *          void
 *          mrb_example_gem_init(mrb_state* mrb){
 *            mrb_define_const(mrb, mrb->kernel_module, "AGE", mrb_fixnum_value(22));
 *          }
 *
 *          mrb_value
 *          mrb_example_gem_final(mrb_state* mrb){
 *          }
 *  @param [mrb_state *] mrb_state* The MRuby state reference.
 *  @param [struct RClass *] RClass* A class or module the constant is defined in.
 *  @param [const char *] name The name of the constant being defined.
 *  @param [mrb_value] mrb_value The value for the constant.
 */
MRB_API void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);

/**
 * Undefines a method.
 *
 * Example:
 *
 *     # Ruby style
 *
 *     class ExampleClassA
 *       def example_method
 *         "example"
 *       end
 *     end
 *     ExampleClassA.new.example_method # => example
 *
 *     class ExampleClassB < ExampleClassA
 *       undef_method :example_method
 *     end
 *
 *     ExampleClassB.new.example_method # => undefined method 'example_method' for ExampleClassB (NoMethodError)
 *
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     mrb_value
 *     mrb_example_method(mrb_state *mrb){
 *       return mrb_str_new_lit(mrb, "example");
 *     }
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *example_class_a;
 *       struct RClass *example_class_b;
 *       struct RClass *example_class_c;
 *
 *       example_class_a = mrb_define_class(mrb, "ExampleClassA", mrb->object_class);
 *       mrb_define_method(mrb, example_class_a, "example_method", mrb_example_method, MRB_ARGS_NONE());
 *       example_class_b = mrb_define_class(mrb, "ExampleClassB", example_class_a);
 *       example_class_c = mrb_define_class(mrb, "ExampleClassC", example_class_b);
 *       mrb_undef_method(mrb, example_class_c, "example_method");
 *     }
 *
 *     mrb_example_gem_final(mrb_state* mrb){
 *     }
 * @param [mrb_state*] mrb_state* The mruby state reference.
 * @param [struct RClass*] RClass* A class the method will be undefined from.
 * @param [const char*] constchar* The name of the method to be undefined.
 */
MRB_API void mrb_undef_method(mrb_state*, struct RClass*, const char*);

/**
 * Undefine a class method.
 * Example:
 *
 *      # Ruby style
 *      class ExampleClass
 *        def self.example_method
 *          "example"
 *        end
 *      end
 *
 *     ExampleClass.example_method
 *
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     mrb_value
 *     mrb_example_method(mrb_state *mrb){
 *       return mrb_str_new_lit(mrb, "example");
 *     }
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *example_class;
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *       mrb_define_class_method(mrb, example_class, "example_method", mrb_example_method, MRB_ARGS_NONE());
 *       mrb_undef_class_method(mrb, example_class, "example_method");
 *      }
 *
 *      void
 *      mrb_example_gem_final(mrb_state* mrb){
 *      }
 * @param [mrb_state*] mrb_state* The mruby state reference.
 * @param [RClass*] RClass* A class the class method will be undefined from.
 * @param [constchar*] constchar* The name of the class method to be undefined.
 */
MRB_API void mrb_undef_class_method(mrb_state*, struct RClass*, const char*);

/**
 * Initialize a new object instace of c class.
 *
 * Example:
 *
 *     # Ruby style
 *     class ExampleClass
 *     end
 *
 *     p ExampleClass # => #<ExampleClass:0x9958588>
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_class;
 *       mrb_value obj;
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class); # => class ExampleClass; end
 *       obj = mrb_obj_new(mrb, example_class, 0, NULL); # => ExampleClass.new
 *       mrb_p(mrb, obj); // => Kernel#p
 *      }
 * @param [mrb_state*] mrb The current mruby state.
 * @param [RClass*] c Reference to the class of the new object.
 * @param [mrb_int] argc Number of arguments in argv
 * @param [const mrb_value *] argv Array of mrb_value to initialize the object
 * @return [mrb_value] The newly initialized object
 */
MRB_API mrb_value mrb_obj_new(mrb_state *mrb, struct RClass *c, mrb_int argc, const mrb_value *argv);

/** @see mrb_obj_new */
MRB_INLINE mrb_value mrb_class_new_instance(mrb_state *mrb, mrb_int argc, const mrb_value *argv, struct RClass *c)
{
  return mrb_obj_new(mrb,c,argc,argv);
}

MRB_API mrb_value mrb_instance_new(mrb_state *mrb, mrb_value cv);

/**
 * Creates a new instance of Class, Class.
 *
 * Example:
 *
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_class;
 *
 *        mrb_value obj;
 *        example_class = mrb_class_new(mrb, mrb->object_class);
 *        obj = mrb_obj_new(mrb, example_class, 0, NULL); // => #<#<Class:0x9a945b8>:0x9a94588>
 *        mrb_p(mrb, obj); // => Kernel#p
 *       }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] super The super class or parent.
 * @return [struct RClass *] Reference to the new class.
 */
MRB_API struct RClass * mrb_class_new(mrb_state *mrb, struct RClass *super);

/**
 * Creates a new module, Module.
 *
 * Example:
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_module;
 *
 *        example_module = mrb_module_new(mrb);
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @return [struct RClass *] Reference to the new module.
 */
MRB_API struct RClass * mrb_module_new(mrb_state *mrb);

/**
 * Returns an mrb_bool. True if class was defined, and false if the class was not defined.
 *
 * Example:
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_class;
 *       mrb_bool cd;
 *
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *       cd = mrb_class_defined(mrb, "ExampleClass");
 *
 *       // If mrb_class_defined returns 1 then puts "True"
 *       // If mrb_class_defined returns 0 then puts "False"
 *       if (cd == 1){
 *         puts("True");
 *       }
 *       else {
 *         puts("False");
 *       }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name A string representing the name of the class.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_class_defined(mrb_state *mrb, const char *name);

/**
 * Gets a class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_class_get(mrb_state *mrb, const char *name);

/**
 * Gets a exception class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_exc_get(mrb_state *mrb, const char *name);

/**
 * Returns an mrb_bool. True if inner class was defined, and false if the inner class was not defined.
 *
 * Example:
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_outer, *example_inner;
 *       mrb_bool cd;
 *
 *       example_outer = mrb_define_module(mrb, "ExampleOuter");
 *
 *       example_inner = mrb_define_class_under(mrb, example_outer, "ExampleInner", mrb->object_class);
 *       cd = mrb_class_defined_under(mrb, example_outer, "ExampleInner");
 *
 *       // If mrb_class_defined_under returns 1 then puts "True"
 *       // If mrb_class_defined_under returns 0 then puts "False"
 *       if (cd == 1){
 *         puts("True");
 *       }
 *       else {
 *         puts("False");
 *       }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the outer class.
 * @param [const char *] name A string representing the name of the inner class.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_class_defined_under(mrb_state *mrb, struct RClass *outer, const char *name);

/**
 * Gets a child class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the parent class.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_class_get_under(mrb_state *mrb, struct RClass *outer, const char *name);

/**
 * Gets a module.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the module.
 * @return [struct RClass *] A reference to the module.
*/
MRB_API struct RClass * mrb_module_get(mrb_state *mrb, const char *name);

/**
 * Gets a module defined under another module.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the outer module.
 * @param [const char *] name The name of the module.
 * @return [struct RClass *] A reference to the module.
*/
MRB_API struct RClass * mrb_module_get_under(mrb_state *mrb, struct RClass *outer, const char *name);
MRB_API mrb_value mrb_notimplement_m(mrb_state*, mrb_value);

/**
 * Duplicate an object.
 *
 * Equivalent to:
 *   Object#dup
 * @param [mrb_state*] mrb The current mruby state.
 * @param [mrb_value] obj Object to be duplicate.
 * @return [mrb_value] The newly duplicated object.
 */
MRB_API mrb_value mrb_obj_dup(mrb_state *mrb, mrb_value obj);
MRB_API mrb_value mrb_check_to_integer(mrb_state *mrb, mrb_value val, const char *method);

/**
 * Returns true if obj responds to the given method. If the method was defined for that
 * class it returns true, it returns false otherwise.
 *
 *      Example:
 *      # Ruby style
 *      class ExampleClass
 *        def example_method
 *        end
 *      end
 *
 *      ExampleClass.new.respond_to?(:example_method) # => true
 *
 *      // C style
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_class;
 *        mrb_sym mid;
 *        mrb_bool obj_resp;
 *
 *        example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *        mrb_define_method(mrb, example_class, "example_method", exampleMethod, MRB_ARGS_NONE());
 *        mid = mrb_intern_str(mrb, mrb_str_new_lit(mrb, "example_method" ));
 *        obj_resp = mrb_obj_respond_to(mrb, example_class, mid); // => 1(true in Ruby world)
 *
 *        // If mrb_obj_respond_to returns 1 then puts "True"
 *        // If mrb_obj_respond_to returns 0 then puts "False"
 *        if (obj_resp == 1) {
 *          puts("True");
 *        }
 *        else if (obj_resp == 0) {
 *          puts("False");
 *        }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] c A reference to a class.
 * @param [mrb_sym] mid A symbol referencing a method id.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_obj_respond_to(mrb_state *mrb, struct RClass* c, mrb_sym mid);

/**
 * Defines a new class under a given module
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer Reference to the module under which the new class will be defined
 * @param [const char *] name The name of the defined class
 * @param [struct RClass *] super The new class parent
 * @return [struct RClass *] Reference to the newly defined class
 * @see mrb_define_class
 */
MRB_API struct RClass * mrb_define_class_under(mrb_state *mrb, struct RClass *outer, const char *name, struct RClass *super);

MRB_API struct RClass * mrb_define_module_under(mrb_state *mrb, struct RClass *outer, const char *name);

/**
 * Function requires n arguments.
 *
 * @param n
 *      The number of required arguments.
 */
#define MRB_ARGS_REQ(n)     ((mrb_aspec)((n)&0x1f) << 18)

/**
 * Funtion takes n optional arguments
 *
 * @param n
 *      The number of optional arguments.
 */
#define MRB_ARGS_OPT(n)     ((mrb_aspec)((n)&0x1f) << 13)

/**
 * Funtion takes n1 mandatory arguments and n2 optional arguments
 *
 * @param n1
 *      The number of required arguments.
 * @param n2
 *      The number of optional arguments.
 */
#define MRB_ARGS_ARG(n1,n2)   (MRB_ARGS_REQ(n1)|MRB_ARGS_OPT(n2))

/** rest argument */
#define MRB_ARGS_REST()     ((mrb_aspec)(1 << 12))

/** required arguments after rest */
#define MRB_ARGS_POST(n)    ((mrb_aspec)((n)&0x1f) << 7)

/** keyword arguments (n of keys, kdict) */
#define MRB_ARGS_KEY(n1,n2) ((mrb_aspec)((((n1)&0x1f) << 2) | ((n2)?(1<<1):0)))

/**
 * Function takes a block argument
 */
#define MRB_ARGS_BLOCK()    ((mrb_aspec)1)

/**
 * Function accepts any number of arguments
 */
#define MRB_ARGS_ANY()      MRB_ARGS_REST()

/**
 * Function accepts no arguments
 */
#define MRB_ARGS_NONE()     ((mrb_aspec)0)

/**
 * Format specifiers for {mrb_get_args} function
 *
 * Must be a C string composed of the following format specifiers:
 *
 * | char | Ruby type      | C types           | Notes                                               |
 * |:----:|----------------|-------------------|----------------------------------------------------|
 * | `o`  | {Object}       | {mrb_value}       | Could be used to retrieve any type of argument     |
 * | `C`  | {Class}/{Module} | {mrb_value}     |                                                    |
 * | `S`  | {String}       | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `A`  | {Array}        | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `H`  | {Hash}         | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `s`  | {String}       | char *, {mrb_int} |  Receive two arguments; `s!` gives (`NULL`,`0`) for `nil`       |
 * | `z`  | {String}       | char *            | `NULL` terminated string; `z!` gives `NULL` for `nil`           |
 * | `a`  | {Array}        | {mrb_value} *, {mrb_int} | Receive two arguments; `a!` gives (`NULL`,`0`) for `nil` |
 * | `f`  | {Float}        | {mrb_float}       |                                                    |
 * | `i`  | {Integer}      | {mrb_int}         |                                                    |
 * | `b`  | boolean        | {mrb_bool}        |                                                    |
 * | `n`  | {Symbol}       | {mrb_sym}         |                                                    |
 * | `&`  | block          | {mrb_value}       |                                                    |
 * | `*`  | rest arguments | {mrb_value} *, {mrb_int} | Receive the rest of arguments as an array.  |
 * | &vert; | optional     |                   | After this spec following specs would be optional. |
 * | `?`  | optional given | {mrb_bool}        | `TRUE` if preceding argument is given. Used to check optional argument is given. |
 *
 * @see mrb_get_args
 */
typedef const char *mrb_args_format;

/**
 * Retrieve arguments from mrb_state.
 *
 * When applicable, implicit conversions (such as `to_str`, `to_ary`, `to_hash`) are
 * applied to received arguments.
 * Used inside a function of mrb_func_t type.
 *
 * @param mrb The current MRuby state.
 * @param format [mrb_args_format] is a list of format specifiers
 * @param ... The passing variadic arguments must be a pointer of retrieving type.
 * @return the number of arguments retrieved.
 * @see mrb_args_format
 */
MRB_API mrb_int mrb_get_args(mrb_state *mrb, mrb_args_format format, ...);

static inline mrb_sym
mrb_get_mid(mrb_state *mrb) /* get method symbol */
{
  return mrb->c->ci->mid;
}

/**
 * Retrieve number of arguments from mrb_state.
 *
 * Correctly handles *splat arguments.
 */
MRB_API mrb_int mrb_get_argc(mrb_state *mrb);

MRB_API mrb_value* mrb_get_argv(mrb_state *mrb);

/* `strlen` for character string literals (use with caution or `strlen` instead)
    Adjacent string literals are concatenated in C/C++ in translation phase 6.
    If `lit` is not one, the compiler will report a syntax error:
     MSVC: "error C2143: syntax error : missing ')' before 'string'"
     GCC:  "error: expected ')' before string constant"
*/
#define mrb_strlen_lit(lit) (sizeof(lit "") - 1)

/**
 * Call existing ruby functions.
 *
 *      #include <stdio.h>
 *      #include <mruby.h>
 *      #include "mruby/compile.h"
 *
 *      int
 *      main()
 *      {
 *        mrb_int i = 99;
 *        mrb_state *mrb = mrb_open();
 *
 *        if (!mrb) { }
 *        FILE *fp = fopen("test.rb","r");
 *        mrb_value obj = mrb_load_file(mrb,fp);
 *        mrb_funcall(mrb, obj, "method_name", 1, mrb_fixnum_value(i));
 *        fclose(fp);
 *        mrb_close(mrb);
 *       }
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [mrb_value] mrb_value A reference to an mruby value.
 * @param [const char*] const char* The name of the method.
 * @param [mrb_int] mrb_int The number of arguments the method has.
 * @param [...] ... Variadic values(not type safe!).
 * @return [mrb_value] mrb_value mruby function value.
 */
MRB_API mrb_value mrb_funcall(mrb_state*, mrb_value, const char*, mrb_int,...);
/**
 * Call existing ruby functions. This is basically the type safe version of mrb_funcall.
 *
 *      #include <stdio.h>
 *      #include <mruby.h>
 *      #include "mruby/compile.h"
 *      int
 *      main()
 *      {
 *        mrb_int i = 99;
 *        mrb_state *mrb = mrb_open();
 *
 *        if (!mrb) { }
 *        mrb_sym m_sym = mrb_intern_lit(mrb, "method_name"); // Symbol for method.
 *
 *        FILE *fp = fopen("test.rb","r");
 *        mrb_value obj = mrb_load_file(mrb,fp);
 *        mrb_funcall_argv(mrb, obj, m_sym, 1, &obj); // Calling ruby function from test.rb.
 *        fclose(fp);
 *        mrb_close(mrb);
 *       }
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [mrb_value] mrb_value A reference to an mruby value.
 * @param [mrb_sym] mrb_sym The symbol representing the method.
 * @param [mrb_int] mrb_int The number of arguments the method has.
 * @param [const mrb_value*] mrb_value* Pointer to the object.
 * @return [mrb_value] mrb_value mruby function value.
 * @see mrb_funcall
 */
MRB_API mrb_value mrb_funcall_argv(mrb_state*, mrb_value, mrb_sym, mrb_int, const mrb_value*);
/**
 * Call existing ruby functions with a block.
 */
MRB_API mrb_value mrb_funcall_with_block(mrb_state*, mrb_value, mrb_sym, mrb_int, const mrb_value*, mrb_value);
/**
 * Create a symbol
 *
 *     # Ruby style:
 *     :pizza # => :pizza
 *
 *     // C style:
 *     mrb_sym m_sym = mrb_intern_lit(mrb, "pizza"); //  => :pizza
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [const char*] const char* The name of the method.
 * @return [mrb_sym] mrb_sym A symbol.
 */
MRB_API mrb_sym mrb_intern_cstr(mrb_state*,const char*);
MRB_API mrb_sym mrb_intern(mrb_state*,const char*,size_t);
MRB_API mrb_sym mrb_intern_static(mrb_state*,const char*,size_t);
#define mrb_intern_lit(mrb, lit) mrb_intern_static(mrb, lit, mrb_strlen_lit(lit))
MRB_API mrb_sym mrb_intern_str(mrb_state*,mrb_value);
MRB_API mrb_value mrb_check_intern_cstr(mrb_state*,const char*);
MRB_API mrb_value mrb_check_intern(mrb_state*,const char*,size_t);
MRB_API mrb_value mrb_check_intern_str(mrb_state*,mrb_value);
MRB_API const char *mrb_sym2name(mrb_state*,mrb_sym);
MRB_API const char *mrb_sym2name_len(mrb_state*,mrb_sym,mrb_int*);
MRB_API mrb_value mrb_sym2str(mrb_state*,mrb_sym);

MRB_API void *mrb_malloc(mrb_state*, size_t);         /* raise RuntimeError if no mem */
MRB_API void *mrb_calloc(mrb_state*, size_t, size_t); /* ditto */
MRB_API void *mrb_realloc(mrb_state*, void*, size_t); /* ditto */
MRB_API void *mrb_realloc_simple(mrb_state*, void*, size_t); /* return NULL if no memory available */
MRB_API void *mrb_malloc_simple(mrb_state*, size_t);  /* return NULL if no memory available */
MRB_API struct RBasic *mrb_obj_alloc(mrb_state*, enum mrb_vtype, struct RClass*);
MRB_API void mrb_free(mrb_state*, void*);

MRB_API mrb_value mrb_str_new(mrb_state *mrb, const char *p, size_t len);

/**
 * Turns a C string into a Ruby string value.
 */
MRB_API mrb_value mrb_str_new_cstr(mrb_state*, const char*);
MRB_API mrb_value mrb_str_new_static(mrb_state *mrb, const char *p, size_t len);
#define mrb_str_new_lit(mrb, lit) mrb_str_new_static(mrb, (lit), mrb_strlen_lit(lit))

#ifdef _WIN32
char* mrb_utf8_from_locale(const char *p, int len);
char* mrb_locale_from_utf8(const char *p, int len);
#define mrb_locale_free(p) free(p)
#define mrb_utf8_free(p) free(p)
#else
#define mrb_utf8_from_locale(p, l) ((char*)p)
#define mrb_locale_from_utf8(p, l) ((char*)p)
#define mrb_locale_free(p)
#define mrb_utf8_free(p)
#endif

/**
 * Creates new mrb_state.
 *
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open(void);

/**
 * Create new mrb_state with custom allocators.
 *
 * @param f
 *      Reference to the allocation function.
 * @param ud
 *      User data will be passed to custom allocator f.
 *      If user data isn't required just pass NULL.
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open_allocf(mrb_allocf f, void *ud);

/**
 * Create new mrb_state with just the MRuby core
 *
 * @param f
 *      Reference to the allocation function.
 *      Use mrb_default_allocf for the default
 * @param ud
 *      User data will be passed to custom allocator f.
 *      If user data isn't required just pass NULL.
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open_core(mrb_allocf f, void *ud);

/**
 * Closes and frees a mrb_state.
 *
 * @param mrb
 *      Pointer to the mrb_state to be closed.
 */
MRB_API void mrb_close(mrb_state *mrb);

/**
 * The default allocation function.
 *
 * @see mrb_allocf
 */
MRB_API void* mrb_default_allocf(mrb_state*, void*, size_t, void*);

MRB_API mrb_value mrb_top_self(mrb_state *);
MRB_API mrb_value mrb_run(mrb_state*, struct RProc*, mrb_value);
MRB_API mrb_value mrb_top_run(mrb_state*, struct RProc*, mrb_value, unsigned int);
MRB_API mrb_value mrb_vm_run(mrb_state*, struct RProc*, mrb_value, unsigned int);
MRB_API mrb_value mrb_vm_exec(mrb_state*, struct RProc*, mrb_code*);
/* compatibility macros */
#define mrb_toplevel_run_keep(m,p,k) mrb_top_run((m),(p),mrb_top_self(m),(k))
#define mrb_toplevel_run(m,p) mrb_toplevel_run_keep((m),(p),0)
#define mrb_context_run(m,p,s,k) mrb_vm_run((m),(p),(s),(k))

MRB_API void mrb_p(mrb_state*, mrb_value);
MRB_API mrb_int mrb_obj_id(mrb_value obj);
MRB_API mrb_sym mrb_obj_to_sym(mrb_state *mrb, mrb_value name);

MRB_API mrb_bool mrb_obj_eq(mrb_state*, mrb_value, mrb_value);
MRB_API mrb_bool mrb_obj_equal(mrb_state*, mrb_value, mrb_value);
MRB_API mrb_bool mrb_equal(mrb_state *mrb, mrb_value obj1, mrb_value obj2);
MRB_API mrb_value mrb_convert_to_integer(mrb_state *mrb, mrb_value val, mrb_int base);
MRB_API mrb_value mrb_Integer(mrb_state *mrb, mrb_value val);
#ifndef MRB_WITHOUT_FLOAT
MRB_API mrb_value mrb_Float(mrb_state *mrb, mrb_value val);
#endif
MRB_API mrb_value mrb_inspect(mrb_state *mrb, mrb_value obj);
MRB_API mrb_bool mrb_eql(mrb_state *mrb, mrb_value obj1, mrb_value obj2);

static inline int mrb_gc_arena_save(mrb_state*);
static inline void mrb_gc_arena_restore(mrb_state*,int);

static inline int
mrb_gc_arena_save(mrb_state *mrb)
{
  return mrb->gc.arena_idx;
}

static inline void
mrb_gc_arena_restore(mrb_state *mrb, int idx)
{
  mrb->gc.arena_idx = idx;
}

MRB_API int mrb_gc_arena_save(mrb_state*);
MRB_API void mrb_gc_arena_restore(mrb_state*,int);

MRB_API void mrb_garbage_collect(mrb_state*);
MRB_API void mrb_full_gc(mrb_state*);
MRB_API void mrb_incremental_gc(mrb_state *);
MRB_API void mrb_gc_mark(mrb_state*,struct RBasic*);
#define mrb_gc_mark_value(mrb,val) do {\
  if (!mrb_immediate_p(val)) mrb_gc_mark((mrb), mrb_basic_ptr(val)); \
} while (0)
MRB_API void mrb_field_write_barrier(mrb_state *, struct RBasic*, struct RBasic*);
#define mrb_field_write_barrier_value(mrb, obj, val) do{\
  if (!mrb_immediate_p(val)) mrb_field_write_barrier((mrb), (obj), mrb_basic_ptr(val)); \
} while (0)
MRB_API void mrb_write_barrier(mrb_state *, struct RBasic*);

MRB_API mrb_value mrb_check_convert_type(mrb_state *mrb, mrb_value val, enum mrb_vtype type, const char *tname, const char *method);
MRB_API mrb_value mrb_any_to_s(mrb_state *mrb, mrb_value obj);
MRB_API const char * mrb_obj_classname(mrb_state *mrb, mrb_value obj);
MRB_API struct RClass* mrb_obj_class(mrb_state *mrb, mrb_value obj);
MRB_API mrb_value mrb_class_path(mrb_state *mrb, struct RClass *c);
MRB_API mrb_value mrb_convert_type(mrb_state *mrb, mrb_value val, enum mrb_vtype type, const char *tname, const char *method);
MRB_API mrb_bool mrb_obj_is_kind_of(mrb_state *mrb, mrb_value obj, struct RClass *c);
MRB_API mrb_value mrb_obj_inspect(mrb_state *mrb, mrb_value self);
MRB_API mrb_value mrb_obj_clone(mrb_state *mrb, mrb_value self);

#ifndef ISPRINT
#define ISASCII(c) ((unsigned)(c) <= 0x7f)
#define ISPRINT(c) (((unsigned)(c) - 0x20) < 0x5f)
#define ISSPACE(c) ((c) == ' ' || (unsigned)(c) - '\t' < 5)
#define ISUPPER(c) (((unsigned)(c) - 'A') < 26)
#define ISLOWER(c) (((unsigned)(c) - 'a') < 26)
#define ISALPHA(c) ((((unsigned)(c) | 0x20) - 'a') < 26)
#define ISDIGIT(c) (((unsigned)(c) - '0') < 10)
#define ISXDIGIT(c) (ISDIGIT(c) || ((unsigned)(c) | 0x20) - 'a' < 6)
#define ISALNUM(c) (ISALPHA(c) || ISDIGIT(c))
#define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#define ISCNTRL(c) ((unsigned)(c) < 0x20 || (c) == 0x7f)
#define TOUPPER(c) (ISLOWER(c) ? ((c) & 0x5f) : (c))
#define TOLOWER(c) (ISUPPER(c) ? ((c) | 0x20) : (c))
#endif

MRB_API mrb_value mrb_exc_new(mrb_state *mrb, struct RClass *c, const char *ptr, size_t len);
MRB_API mrb_noreturn void mrb_exc_raise(mrb_state *mrb, mrb_value exc);

MRB_API mrb_noreturn void mrb_raise(mrb_state *mrb, struct RClass *c, const char *msg);
MRB_API mrb_noreturn void mrb_raisef(mrb_state *mrb, struct RClass *c, const char *fmt, ...);
MRB_API mrb_noreturn void mrb_name_error(mrb_state *mrb, mrb_sym id, const char *fmt, ...);
MRB_API void mrb_warn(mrb_state *mrb, const char *fmt, ...);
MRB_API mrb_noreturn void mrb_bug(mrb_state *mrb, const char *fmt, ...);
MRB_API void mrb_print_backtrace(mrb_state *mrb);
MRB_API void mrb_print_error(mrb_state *mrb);

/* macros to get typical exception objects
   note:
   + those E_* macros requires mrb_state* variable named mrb.
   + exception objects obtained from those macros are local to mrb
*/
#define E_RUNTIME_ERROR             (mrb_exc_get(mrb, "RuntimeError"))
#define E_TYPE_ERROR                (mrb_exc_get(mrb, "TypeError"))
#define E_ARGUMENT_ERROR            (mrb_exc_get(mrb, "ArgumentError"))
#define E_INDEX_ERROR               (mrb_exc_get(mrb, "IndexError"))
#define E_RANGE_ERROR               (mrb_exc_get(mrb, "RangeError"))
#define E_NAME_ERROR                (mrb_exc_get(mrb, "NameError"))
#define E_NOMETHOD_ERROR            (mrb_exc_get(mrb, "NoMethodError"))
#define E_SCRIPT_ERROR              (mrb_exc_get(mrb, "ScriptError"))
#define E_SYNTAX_ERROR              (mrb_exc_get(mrb, "SyntaxError"))
#define E_LOCALJUMP_ERROR           (mrb_exc_get(mrb, "LocalJumpError"))
#define E_REGEXP_ERROR              (mrb_exc_get(mrb, "RegexpError"))
#define E_FROZEN_ERROR              (mrb_exc_get(mrb, "FrozenError"))

#define E_NOTIMP_ERROR              (mrb_exc_get(mrb, "NotImplementedError"))
#ifndef MRB_WITHOUT_FLOAT
#define E_FLOATDOMAIN_ERROR         (mrb_exc_get(mrb, "FloatDomainError"))
#endif

#define E_KEY_ERROR                 (mrb_exc_get(mrb, "KeyError"))

MRB_API mrb_value mrb_yield(mrb_state *mrb, mrb_value b, mrb_value arg);
MRB_API mrb_value mrb_yield_argv(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv);
MRB_API mrb_value mrb_yield_with_class(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv, mrb_value self, struct RClass *c);

/* continue execution to the proc */
/* this function should always be called as the last function of a method */
/* e.g. return mrb_yield_cont(mrb, proc, self, argc, argv); */
mrb_value mrb_yield_cont(mrb_state *mrb, mrb_value b, mrb_value self, mrb_int argc, const mrb_value *argv);

/* mrb_gc_protect() leaves the object in the arena */
MRB_API void mrb_gc_protect(mrb_state *mrb, mrb_value obj);
/* mrb_gc_register() keeps the object from GC. */
MRB_API void mrb_gc_register(mrb_state *mrb, mrb_value obj);
/* mrb_gc_unregister() removes the object from GC root. */
MRB_API void mrb_gc_unregister(mrb_state *mrb, mrb_value obj);

MRB_API mrb_value mrb_to_int(mrb_state *mrb, mrb_value val);
#define mrb_int(mrb, val) mrb_fixnum(mrb_to_int(mrb, val))
MRB_API void mrb_check_type(mrb_state *mrb, mrb_value x, enum mrb_vtype t);

typedef enum call_type {
  CALL_PUBLIC,
  CALL_FCALL,
  CALL_VCALL,
  CALL_TYPE_MAX
} call_type;

MRB_API void mrb_define_alias(mrb_state *mrb, struct RClass *klass, const char *name1, const char *name2);
MRB_API const char *mrb_class_name(mrb_state *mrb, struct RClass* klass);
MRB_API void mrb_define_global_const(mrb_state *mrb, const char *name, mrb_value val);

MRB_API mrb_value mrb_attr_get(mrb_state *mrb, mrb_value obj, mrb_sym id);

MRB_API mrb_bool mrb_respond_to(mrb_state *mrb, mrb_value obj, mrb_sym mid);
MRB_API mrb_bool mrb_obj_is_instance_of(mrb_state *mrb, mrb_value obj, struct RClass* c);
MRB_API mrb_bool mrb_func_basic_p(mrb_state *mrb, mrb_value obj, mrb_sym mid, mrb_func_t func);


/*
 * Resume a Fiber
 *
 * @mrbgem mruby-fiber
 */
MRB_API mrb_value mrb_fiber_resume(mrb_state *mrb, mrb_value fib, mrb_int argc, const mrb_value *argv);

/*
 * Yield a Fiber
 *
 * @mrbgem mruby-fiber
 */
MRB_API mrb_value mrb_fiber_yield(mrb_state *mrb, mrb_int argc, const mrb_value *argv);

/*
 * FiberError reference
 *
 * @mrbgem mruby-fiber
 */
#define E_FIBER_ERROR (mrb_exc_get(mrb, "FiberError"))

/* memory pool implementation */
typedef struct mrb_pool mrb_pool;
MRB_API struct mrb_pool* mrb_pool_open(mrb_state*);
MRB_API void mrb_pool_close(struct mrb_pool*);
MRB_API void* mrb_pool_alloc(struct mrb_pool*, size_t);
MRB_API void* mrb_pool_realloc(struct mrb_pool*, void*, size_t oldlen, size_t newlen);
MRB_API mrb_bool mrb_pool_can_realloc(struct mrb_pool*, void*, size_t);
MRB_API void* mrb_alloca(mrb_state *mrb, size_t);

MRB_API void mrb_state_atexit(mrb_state *mrb, mrb_atexit_func func);

MRB_API void mrb_show_version(mrb_state *mrb);
MRB_API void mrb_show_copyright(mrb_state *mrb);

MRB_API mrb_value mrb_format(mrb_state *mrb, const char *format, ...);

MRB_END_DECL

/* end mruby.h */

/* start mruby/compile.h */
/**
 * MRuby Compiler
 */
MRB_BEGIN_DECL

struct mrb_parser_state;
/* load context */
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

MRB_API mrbc_context* mrbc_context_new(mrb_state *mrb);
MRB_API void mrbc_context_free(mrb_state *mrb, mrbc_context *cxt);
MRB_API const char *mrbc_filename(mrb_state *mrb, mrbc_context *c, const char *s);
MRB_API void mrbc_partial_hook(mrb_state *mrb, mrbc_context *c, int (*partial_hook)(struct mrb_parser_state*), void*data);

/* AST node structure */
typedef struct mrb_ast_node {
  struct mrb_ast_node *car, *cdr;
  uint16_t lineno, filename_index;
} mrb_ast_node;

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

/* saved error message */
struct mrb_parser_message {
  int lineno;
  int column;
  char* message;
};

#define STR_FUNC_PARSING 0x01
#define STR_FUNC_EXPAND  0x02
#define STR_FUNC_REGEXP  0x04
#define STR_FUNC_WORD    0x08
#define STR_FUNC_SYMBOL  0x10
#define STR_FUNC_ARRAY   0x20
#define STR_FUNC_HEREDOC 0x40
#define STR_FUNC_XQUOTE  0x80

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

/* heredoc structure */
struct mrb_parser_heredoc_info {
  mrb_bool allow_indent:1;
  mrb_bool line_head:1;
  enum mrb_string_type type;
  const char *term;
  int term_len;
  mrb_ast_node *doc;
};

#define MRB_PARSER_TOKBUF_MAX 65536
#define MRB_PARSER_TOKBUF_SIZE 256

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

MRB_API struct mrb_parser_state* mrb_parser_new(mrb_state*);
MRB_API void mrb_parser_free(struct mrb_parser_state*);
MRB_API void mrb_parser_parse(struct mrb_parser_state*,mrbc_context*);

MRB_API void mrb_parser_set_filename(struct mrb_parser_state*, char const*);
MRB_API char const* mrb_parser_get_filename(struct mrb_parser_state*, uint16_t idx);

/* utility functions */
#ifndef MRB_DISABLE_STDIO
MRB_API struct mrb_parser_state* mrb_parse_file(mrb_state*,FILE*,mrbc_context*);
#endif
MRB_API struct mrb_parser_state* mrb_parse_string(mrb_state*,const char*,mrbc_context*);
MRB_API struct mrb_parser_state* mrb_parse_nstring(mrb_state*,const char*,size_t,mrbc_context*);
MRB_API struct RProc* mrb_generate_code(mrb_state*, struct mrb_parser_state*);
MRB_API mrb_value mrb_load_exec(mrb_state *mrb, struct mrb_parser_state *p, mrbc_context *c);

/* program load functions */
#ifndef MRB_DISABLE_STDIO
MRB_API mrb_value mrb_load_file(mrb_state*,FILE*);
MRB_API mrb_value mrb_load_file_cxt(mrb_state*,FILE*, mrbc_context *cxt);
#endif
MRB_API mrb_value mrb_load_string(mrb_state *mrb, const char *s);
MRB_API mrb_value mrb_load_nstring(mrb_state *mrb, const char *s, size_t len);
MRB_API mrb_value mrb_load_string_cxt(mrb_state *mrb, const char *s, mrbc_context *cxt);
MRB_API mrb_value mrb_load_nstring_cxt(mrb_state *mrb, const char *s, size_t len, mrbc_context *cxt);

/** @} */
MRB_END_DECL

/* end mruby/compile.h */

/* start mruby/kash.h */
#include <string.h>
/**
 * khash definitions used in mruby's hash table.
 */
MRB_BEGIN_DECL

typedef uint32_t khint_t;
typedef khint_t khiter_t;

#ifndef KHASH_DEFAULT_SIZE
# define KHASH_DEFAULT_SIZE 32
#endif
#define KHASH_MIN_SIZE 8

#define UPPER_BOUND(x) ((x)>>2|(x)>>1)

/* extern uint8_t __m[]; */

/* mask for flags */
static const uint8_t __m_empty[]  = {0x02, 0x08, 0x20, 0x80};
static const uint8_t __m_del[]    = {0x01, 0x04, 0x10, 0x40};
static const uint8_t __m_either[] = {0x03, 0x0c, 0x30, 0xc0};


#define __ac_isempty(ed_flag, i) (ed_flag[(i)/4]&__m_empty[(i)%4])
#define __ac_isdel(ed_flag, i) (ed_flag[(i)/4]&__m_del[(i)%4])
#define __ac_iseither(ed_flag, i) (ed_flag[(i)/4]&__m_either[(i)%4])
#define khash_power2(v) do { \
  v--;\
  v |= v >> 1;\
  v |= v >> 2;\
  v |= v >> 4;\
  v |= v >> 8;\
  v |= v >> 16;\
  v++;\
} while (0)
#define khash_mask(h) ((h)->n_buckets-1)
#define khash_upper_bound(h) (UPPER_BOUND((h)->n_buckets))

/* declare struct kh_xxx and kh_xxx_funcs

   name: hash name
   khkey_t: key data type
   khval_t: value data type
   kh_is_map: (0: hash set / 1: hash map)
*/
#define KHASH_DECLARE(name, khkey_t, khval_t, kh_is_map)                \
  typedef struct kh_##name {                                            \
    khint_t n_buckets;                                                  \
    khint_t size;                                                       \
    khint_t n_occupied;                                                 \
    uint8_t *ed_flags;                                                  \
    khkey_t *keys;                                                      \
    khval_t *vals;                                                      \
  } kh_##name##_t;                                                      \
  void kh_alloc_##name(mrb_state *mrb, kh_##name##_t *h);               \
  kh_##name##_t *kh_init_##name##_size(mrb_state *mrb, khint_t size);   \
  kh_##name##_t *kh_init_##name(mrb_state *mrb);                        \
  void kh_destroy_##name(mrb_state *mrb, kh_##name##_t *h);             \
  void kh_clear_##name(mrb_state *mrb, kh_##name##_t *h);               \
  khint_t kh_get_##name(mrb_state *mrb, kh_##name##_t *h, khkey_t key);           \
  khint_t kh_put_##name(mrb_state *mrb, kh_##name##_t *h, khkey_t key, int *ret); \
  void kh_resize_##name(mrb_state *mrb, kh_##name##_t *h, khint_t new_n_buckets); \
  void kh_del_##name(mrb_state *mrb, kh_##name##_t *h, khint_t x);                \
  kh_##name##_t *kh_copy_##name(mrb_state *mrb, kh_##name##_t *h);

static inline void
kh_fill_flags(uint8_t *p, uint8_t c, size_t len)
{
  while (len-- > 0) {
    *p++ = c;
  }
}

/* define kh_xxx_funcs

   name: hash name
   khkey_t: key data type
   khval_t: value data type
   kh_is_map: (0: hash set / 1: hash map)
   __hash_func: hash function
   __hash_equal: hash comparation function
*/
#define KHASH_DEFINE(name, khkey_t, khval_t, kh_is_map, __hash_func, __hash_equal) \
  void kh_alloc_##name(mrb_state *mrb, kh_##name##_t *h)                \
  {                                                                     \
    khint_t sz = h->n_buckets;                                          \
    size_t len = sizeof(khkey_t) + (kh_is_map ? sizeof(khval_t) : 0);   \
    uint8_t *p = (uint8_t*)mrb_malloc(mrb, sizeof(uint8_t)*sz/4+len*sz); \
    h->size = h->n_occupied = 0;                                        \
    h->keys = (khkey_t *)p;                                             \
    h->vals = kh_is_map ? (khval_t *)(p+sizeof(khkey_t)*sz) : NULL;     \
    h->ed_flags = p+len*sz;                                             \
    kh_fill_flags(h->ed_flags, 0xaa, sz/4);                             \
  }                                                                     \
  kh_##name##_t *kh_init_##name##_size(mrb_state *mrb, khint_t size) {  \
    kh_##name##_t *h = (kh_##name##_t*)mrb_calloc(mrb, 1, sizeof(kh_##name##_t)); \
    if (size < KHASH_MIN_SIZE)                                          \
      size = KHASH_MIN_SIZE;                                            \
    khash_power2(size);                                                 \
    h->n_buckets = size;                                                \
    kh_alloc_##name(mrb, h);                                            \
    return h;                                                           \
  }                                                                     \
  kh_##name##_t *kh_init_##name(mrb_state *mrb) {                       \
    return kh_init_##name##_size(mrb, KHASH_DEFAULT_SIZE);              \
  }                                                                     \
  void kh_destroy_##name(mrb_state *mrb, kh_##name##_t *h)              \
  {                                                                     \
    if (h) {                                                            \
      mrb_free(mrb, h->keys);                                           \
      mrb_free(mrb, h);                                                 \
    }                                                                   \
  }                                                                     \
  void kh_clear_##name(mrb_state *mrb, kh_##name##_t *h)                \
  {                                                                     \
    (void)mrb;                                                          \
    if (h && h->ed_flags) {                                             \
      kh_fill_flags(h->ed_flags, 0xaa, h->n_buckets/4);                 \
      h->size = h->n_occupied = 0;                                      \
    }                                                                   \
  }                                                                     \
  khint_t kh_get_##name(mrb_state *mrb, kh_##name##_t *h, khkey_t key)  \
  {                                                                     \
    khint_t k = __hash_func(mrb,key) & khash_mask(h), step = 0;         \
    (void)mrb;                                                          \
    while (!__ac_isempty(h->ed_flags, k)) {                             \
      if (!__ac_isdel(h->ed_flags, k)) {                                \
        if (__hash_equal(mrb,h->keys[k], key)) return k;                \
      }                                                                 \
      k = (k+(++step)) & khash_mask(h);                                 \
    }                                                                   \
    return kh_end(h);                                                   \
  }                                                                     \
  void kh_resize_##name(mrb_state *mrb, kh_##name##_t *h, khint_t new_n_buckets) \
  {                                                                     \
    if (new_n_buckets < KHASH_MIN_SIZE)                                 \
      new_n_buckets = KHASH_MIN_SIZE;                                   \
    khash_power2(new_n_buckets);                                        \
    {                                                                   \
      kh_##name##_t hh;                                                 \
      uint8_t *old_ed_flags = h->ed_flags;                              \
      khkey_t *old_keys = h->keys;                                      \
      khval_t *old_vals = h->vals;                                      \
      khint_t old_n_buckets = h->n_buckets;                             \
      khint_t i;                                                        \
      hh.n_buckets = new_n_buckets;                                     \
      kh_alloc_##name(mrb, &hh);                                        \
      /* relocate */                                                    \
      for (i=0 ; i<old_n_buckets ; i++) {                               \
        if (!__ac_iseither(old_ed_flags, i)) {                          \
          khint_t k = kh_put_##name(mrb, &hh, old_keys[i], NULL);       \
          if (kh_is_map) kh_value(&hh,k) = old_vals[i];                 \
        }                                                               \
      }                                                                 \
      /* copy hh to h */                                                \
      *h = hh;                                                          \
      mrb_free(mrb, old_keys);                                          \
    }                                                                   \
  }                                                                     \
  khint_t kh_put_##name(mrb_state *mrb, kh_##name##_t *h, khkey_t key, int *ret) \
  {                                                                     \
    khint_t k, del_k, step = 0;                                         \
    if (h->n_occupied >= khash_upper_bound(h)) {                        \
      kh_resize_##name(mrb, h, h->n_buckets*2);                         \
    }                                                                   \
    k = __hash_func(mrb,key) & khash_mask(h);                           \
    del_k = kh_end(h);                                                  \
    while (!__ac_isempty(h->ed_flags, k)) {                             \
      if (!__ac_isdel(h->ed_flags, k)) {                                \
        if (__hash_equal(mrb,h->keys[k], key)) {                        \
          if (ret) *ret = 0;                                            \
          return k;                                                     \
        }                                                               \
      }                                                                 \
      else if (del_k == kh_end(h)) {                                    \
        del_k = k;                                                      \
      }                                                                 \
      k = (k+(++step)) & khash_mask(h);                                 \
    }                                                                   \
    if (del_k != kh_end(h)) {                                           \
      /* put at del */                                                  \
      h->keys[del_k] = key;                                             \
      h->ed_flags[del_k/4] &= ~__m_del[del_k%4];                        \
      h->size++;                                                        \
      if (ret) *ret = 2;                                                \
      return del_k;                                                     \
    }                                                                   \
    else {                                                              \
      /* put at empty */                                                \
      h->keys[k] = key;                                                 \
      h->ed_flags[k/4] &= ~__m_empty[k%4];                              \
      h->size++;                                                        \
      h->n_occupied++;                                                  \
      if (ret) *ret = 1;                                                \
      return k;                                                         \
    }                                                                   \
  }                                                                     \
  void kh_del_##name(mrb_state *mrb, kh_##name##_t *h, khint_t x)       \
  {                                                                     \
    (void)mrb;                                                          \
    mrb_assert(x != h->n_buckets && !__ac_iseither(h->ed_flags, x));    \
    h->ed_flags[x/4] |= __m_del[x%4];                                   \
    h->size--;                                                          \
  }                                                                     \
  kh_##name##_t *kh_copy_##name(mrb_state *mrb, kh_##name##_t *h)       \
  {                                                                     \
    kh_##name##_t *h2;                                                  \
    khiter_t k, k2;                                                     \
                                                                        \
    h2 = kh_init_##name(mrb);                                           \
    for (k = kh_begin(h); k != kh_end(h); k++) {                        \
      if (kh_exist(h, k)) {                                             \
        k2 = kh_put_##name(mrb, h2, kh_key(h, k), NULL);                \
        if (kh_is_map) kh_value(h2, k2) = kh_value(h, k);               \
      }                                                                 \
    }                                                                   \
    return h2;                                                          \
  }


#define khash_t(name) kh_##name##_t

#define kh_init_size(name,mrb,size) kh_init_##name##_size(mrb,size)
#define kh_init(name,mrb) kh_init_##name(mrb)
#define kh_destroy(name, mrb, h) kh_destroy_##name(mrb, h)
#define kh_clear(name, mrb, h) kh_clear_##name(mrb, h)
#define kh_resize(name, mrb, h, s) kh_resize_##name(mrb, h, s)
#define kh_put(name, mrb, h, k) kh_put_##name(mrb, h, k, NULL)
#define kh_put2(name, mrb, h, k, r) kh_put_##name(mrb, h, k, r)
#define kh_get(name, mrb, h, k) kh_get_##name(mrb, h, k)
#define kh_del(name, mrb, h, k) kh_del_##name(mrb, h, k)
#define kh_copy(name, mrb, h) kh_copy_##name(mrb, h)

#define kh_exist(h, x) (!__ac_iseither((h)->ed_flags, (x)))
#define kh_key(h, x) ((h)->keys[x])
#define kh_val(h, x) ((h)->vals[x])
#define kh_value(h, x) ((h)->vals[x])
#define kh_begin(h) (khint_t)(0)
#define kh_end(h) ((h)->n_buckets)
#define kh_size(h) ((h)->size)
#define kh_n_buckets(h) ((h)->n_buckets)

#define kh_int_hash_func(mrb,key) (khint_t)((key)^((key)<<2)^((key)>>2))
#define kh_int_hash_equal(mrb,a, b) (a == b)
#define kh_int64_hash_func(mrb,key) (khint_t)((key)>>33^(key)^(key)<<11)
#define kh_int64_hash_equal(mrb,a, b) (a == b)
static inline khint_t __ac_X31_hash_string(const char *s)
{
    khint_t h = *s;
    if (h) for (++s ; *s; ++s) h = (h << 5) - h + *s;
    return h;
}
#define kh_str_hash_func(mrb,key) __ac_X31_hash_string(key)
#define kh_str_hash_equal(mrb,a, b) (strcmp(a, b) == 0)

typedef const char *kh_cstr_t;

MRB_END_DECL

/* end mruby/khash.h */

/* start mruby/hash.h */
MRB_BEGIN_DECL

struct RHash {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  struct kh_ht *ht;
};

#define mrb_hash_ptr(v)    ((struct RHash*)(mrb_ptr(v)))
#define mrb_hash_value(p)  mrb_obj_value((void*)(p))

MRB_API mrb_value mrb_hash_new_capa(mrb_state*, mrb_int);

/*
 * Initializes a new hash.
 *
 * Equivalent to:
 *
 *      Hash.new
 *
 * @param mrb The mruby state reference.
 * @return The initialized hash.
 */
MRB_API mrb_value mrb_hash_new(mrb_state *mrb);

/*
 * Sets a keys and values to hashes.
 *
 * Equivalent to:
 *
 *      hash[key] = val
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @param key The key to set.
 * @param val The value to set.
 * @return The value.
 */
MRB_API void mrb_hash_set(mrb_state *mrb, mrb_value hash, mrb_value key, mrb_value val);

/*
 * Gets a value from a key. If the key is not found, the default of the
 * hash is used.
 *
 * Equivalent to:
 *
 *     hash[key]
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @param key The key to get.
 * @return The found value.
 */
MRB_API mrb_value mrb_hash_get(mrb_state *mrb, mrb_value hash, mrb_value key);

/*
 * Gets a value from a key. If the key is not found, the default parameter is
 * used.
 *
 * Equivalent to:
 *
 *     hash.hash_key?(key) ? hash[key] : def
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @param key The key to get.
 * @param def The default value.
 * @return The found value.
 */
MRB_API mrb_value mrb_hash_fetch(mrb_state *mrb, mrb_value hash, mrb_value key, mrb_value def);

/*
 * Deletes hash key and value pair.
 *
 * Equivalent to:
 *
 *     hash.delete(key)
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @param key The key to delete.
 * @return The deleted value.
 */
MRB_API mrb_value mrb_hash_delete_key(mrb_state *mrb, mrb_value hash, mrb_value key);

/*
 * Gets an array of keys.
 *
 * Equivalent to:
 *
 *     hash.keys
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @return An array with the keys of the hash.
 */
MRB_API mrb_value mrb_hash_keys(mrb_state *mrb, mrb_value hash);
MRB_API mrb_value mrb_check_hash_type(mrb_state *mrb, mrb_value hash);

/*
 * Check if the hash is empty
 *
 * Equivalent to:
 *
 *     hash.empty?
 *
 * @param mrb The mruby state reference.
 * @param self The target hash.
 * @return True if the hash is empty, false otherwise.
 */
MRB_API mrb_value mrb_hash_empty_p(mrb_state *mrb, mrb_value self);

/*
 * Gets an array of values.
 *
 * Equivalent to:
 *
 *     hash.values
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @return An array with the values of the hash.
 */
MRB_API mrb_value mrb_hash_values(mrb_state *mrb, mrb_value hash);

/*
 * Clears the hash.
 *
 * Equivalent to:
 *
 *     hash.clear
 *
 * @param mrb The mruby state reference.
 * @param hash The target hash.
 * @return The hash
 */
MRB_API mrb_value mrb_hash_clear(mrb_state *mrb, mrb_value hash);

/* declaration of struct kh_ht */
/* be careful when you touch the internal */
typedef struct {
  mrb_value v;
  mrb_int n;
} mrb_hash_value;

KHASH_DECLARE(ht, mrb_value, mrb_hash_value, TRUE)

/* RHASH_TBL allocates st_table if not available. */
#define RHASH(obj)   ((struct RHash*)(mrb_ptr(obj)))
#define RHASH_TBL(h)          (RHASH(h)->ht)
#define RHASH_IFNONE(h)       mrb_iv_get(mrb, (h), mrb_intern_lit(mrb, "ifnone"))
#define RHASH_PROCDEFAULT(h)  RHASH_IFNONE(h)
MRB_API struct kh_ht * mrb_hash_tbl(mrb_state *mrb, mrb_value hash);

#define MRB_HASH_DEFAULT      1
#define MRB_HASH_PROC_DEFAULT 2
#define MRB_RHASH_DEFAULT_P(h) (RHASH(h)->flags & MRB_HASH_DEFAULT)
#define MRB_RHASH_PROCDEFAULT_P(h) (RHASH(h)->flags & MRB_HASH_PROC_DEFAULT)

/* GC functions */
void mrb_gc_mark_hash(mrb_state*, struct RHash*);
size_t mrb_gc_mark_hash_size(mrb_state*, struct RHash*);
void mrb_gc_free_hash(mrb_state*, struct RHash*);

MRB_END_DECL

/* end mruby/hash.h */

/* start mruby/error.h */
MRB_BEGIN_DECL

struct RException {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
};

#define mrb_exc_ptr(v) ((struct RException*)mrb_ptr(v))

MRB_API void mrb_sys_fail(mrb_state *mrb, const char *mesg);
MRB_API mrb_value mrb_exc_new_str(mrb_state *mrb, struct RClass* c, mrb_value str);
#define mrb_exc_new_str_lit(mrb, c, lit) mrb_exc_new_str(mrb, c, mrb_str_new_lit(mrb, lit))
MRB_API mrb_value mrb_make_exception(mrb_state *mrb, mrb_int argc, const mrb_value *argv);
MRB_API mrb_value mrb_exc_backtrace(mrb_state *mrb, mrb_value exc);
MRB_API mrb_value mrb_get_backtrace(mrb_state *mrb);
MRB_API mrb_noreturn void mrb_no_method_error(mrb_state *mrb, mrb_sym id, mrb_value args, const char *fmt, ...);

/* declaration for fail method */
MRB_API mrb_value mrb_f_raise(mrb_state*, mrb_value);

struct RBreak {
  MRB_OBJECT_HEADER;
  struct RProc *proc;
  mrb_value val;
};

/**
 * Protect
 *
 * @mrbgem mruby-error
 */
MRB_API mrb_value mrb_protect(mrb_state *mrb, mrb_func_t body, mrb_value data, mrb_bool *state);

/**
 * Ensure
 *
 * @mrbgem mruby-error
 */
MRB_API mrb_value mrb_ensure(mrb_state *mrb, mrb_func_t body, mrb_value b_data,
                             mrb_func_t ensure, mrb_value e_data);

/**
 * Rescue
 *
 * @mrbgem mruby-error
 */
MRB_API mrb_value mrb_rescue(mrb_state *mrb, mrb_func_t body, mrb_value b_data,
                             mrb_func_t rescue, mrb_value r_data);

/**
 * Rescue exception
 *
 * @mrbgem mruby-error
 */
MRB_API mrb_value mrb_rescue_exceptions(mrb_state *mrb, mrb_func_t body, mrb_value b_data,
                                        mrb_func_t rescue, mrb_value r_data,
                                        mrb_int len, struct RClass **classes);

MRB_END_DECL

/* end mruby/error.h */

/* start mruby/string.h */

MRB_BEGIN_DECL

extern const char mrb_digitmap[];

#define RSTRING_EMBED_LEN_MAX ((mrb_int)(sizeof(void*) * 3 - 1))

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

#define RSTR_EMBED_P(s) ((s)->flags & MRB_STR_EMBED)
#define RSTR_SET_EMBED_FLAG(s) ((s)->flags |= MRB_STR_EMBED)
#define RSTR_UNSET_EMBED_FLAG(s) ((s)->flags &= ~(MRB_STR_EMBED|MRB_STR_EMBED_LEN_MASK))
#define RSTR_SET_EMBED_LEN(s, n) do {\
  size_t tmp_n = (n);\
  s->flags &= ~MRB_STR_EMBED_LEN_MASK;\
  s->flags |= (tmp_n) << MRB_STR_EMBED_LEN_SHIFT;\
} while (0)
#define RSTR_SET_LEN(s, n) do {\
  if (RSTR_EMBED_P(s)) {\
    RSTR_SET_EMBED_LEN((s),(n));\
  }\
  else {\
    s->as.heap.len = (mrb_int)(n);\
  }\
} while (0)
#define RSTR_EMBED_LEN(s)\
  (mrb_int)(((s)->flags & MRB_STR_EMBED_LEN_MASK) >> MRB_STR_EMBED_LEN_SHIFT)
#define RSTR_PTR(s) ((RSTR_EMBED_P(s)) ? (s)->as.ary : (s)->as.heap.ptr)
#define RSTR_LEN(s) ((RSTR_EMBED_P(s)) ? RSTR_EMBED_LEN(s) : (s)->as.heap.len)
#define RSTR_CAPA(s) (RSTR_EMBED_P(s) ? RSTRING_EMBED_LEN_MAX : (s)->as.heap.aux.capa)

#define RSTR_SHARED_P(s) ((s)->flags & MRB_STR_SHARED)
#define RSTR_SET_SHARED_FLAG(s) ((s)->flags |= MRB_STR_SHARED)
#define RSTR_UNSET_SHARED_FLAG(s) ((s)->flags &= ~MRB_STR_SHARED)

#define RSTR_FSHARED_P(s) ((s)->flags & MRB_STR_FSHARED)
#define RSTR_SET_FSHARED_FLAG(s) ((s)->flags |= MRB_STR_FSHARED)
#define RSTR_UNSET_FSHARED_FLAG(s) ((s)->flags &= ~MRB_STR_FSHARED)

#define RSTR_NOFREE_P(s) ((s)->flags & MRB_STR_NOFREE)
#define RSTR_SET_NOFREE_FLAG(s) ((s)->flags |= MRB_STR_NOFREE)
#define RSTR_UNSET_NOFREE_FLAG(s) ((s)->flags &= ~MRB_STR_NOFREE)

#define RSTR_POOL_P(s) ((s)->flags & MRB_STR_POOL)
#define RSTR_SET_POOL_FLAG(s) ((s)->flags |= MRB_STR_POOL)

/*
 * Returns a pointer from a Ruby string
 */
#define mrb_str_ptr(s)       ((struct RString*)(mrb_ptr(s)))
#define RSTRING(s)           mrb_str_ptr(s)
#define RSTRING_PTR(s)       RSTR_PTR(RSTRING(s))
#define RSTRING_EMBED_LEN(s) RSTR_EMBED_LEN(RSTRING(s))
#define RSTRING_LEN(s)       RSTR_LEN(RSTRING(s))
#define RSTRING_CAPA(s)      RSTR_CAPA(RSTRING(s))
#define RSTRING_END(s)       (RSTRING_PTR(s) + RSTRING_LEN(s))
MRB_API mrb_int mrb_str_strlen(mrb_state*, struct RString*);

#define MRB_STR_SHARED    1
#define MRB_STR_FSHARED   2
#define MRB_STR_NOFREE    4
#define MRB_STR_POOL      8
#define MRB_STR_NO_UTF   16
#define MRB_STR_EMBED    32
#define MRB_STR_EMBED_LEN_MASK 0x7c0
#define MRB_STR_EMBED_LEN_SHIFT 6

void mrb_gc_free_str(mrb_state*, struct RString*);
MRB_API void mrb_str_modify(mrb_state*, struct RString*);

/*
 * Finds the index of a substring in a string
 */
MRB_API mrb_int mrb_str_index(mrb_state*, mrb_value, const char*, mrb_int, mrb_int);
#define mrb_str_index_lit(mrb, str, lit, off) mrb_str_index(mrb, str, lit, mrb_strlen_lit(lit), off);

/*
 * Appends self to other. Returns self as a concatnated string.
 *
 *
 *  Example:
 *
 *     !!!c
 *     int
 *     main(int argc,
 *          char **argv)
 *     {
 *       // Variable declarations.
 *       mrb_value str1;
 *       mrb_value str2;
 *
 *       mrb_state *mrb = mrb_open();
 *       if (!mrb)
 *       {
 *          // handle error
 *       }
 *
 *       // Creates new Ruby strings.
 *       str1 = mrb_str_new_lit(mrb, "abc");
 *       str2 = mrb_str_new_lit(mrb, "def");
 *
 *       // Concatnates str2 to str1.
 *       mrb_str_concat(mrb, str1, str2);
 *
 *      // Prints new Concatnated Ruby string.
 *      mrb_p(mrb, str1);
 *
 *      mrb_close(mrb);
 *      return 0;
 *    }
 *
 *
 *  Result:
 *
 *     => "abcdef"
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] self String to concatenate.
 * @param [mrb_value] other String to append to self.
 * @return [mrb_value] Returns a new String appending other to self.
 */
MRB_API void mrb_str_concat(mrb_state*, mrb_value, mrb_value);

/*
 * Adds two strings together.
 *
 *
 *  Example:
 *
 *     !!!c
 *     int
 *     main(int argc,
 *          char **argv)
 *     {
 *       // Variable declarations.
 *       mrb_value a;
 *       mrb_value b;
 *       mrb_value c;
 *
 *       mrb_state *mrb = mrb_open();
 *       if (!mrb)
 *       {
 *          // handle error
 *       }
 *
 *       // Creates two Ruby strings from the passed in C strings.
 *       a = mrb_str_new_lit(mrb, "abc");
 *       b = mrb_str_new_lit(mrb, "def");
 *
 *       // Prints both C strings.
 *       mrb_p(mrb, a);
 *       mrb_p(mrb, b);
 *
 *       // Concatnates both Ruby strings.
 *       c = mrb_str_plus(mrb, a, b);
 *
 *      // Prints new Concatnated Ruby string.
 *      mrb_p(mrb, c);
 *
 *      mrb_close(mrb);
 *      return 0;
 *    }
 *
 *
 *  Result:
 *
 *     => "abc"  # First string
 *     => "def"  # Second string
 *     => "abcdef" # First & Second concatnated.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] a First string to concatenate.
 * @param [mrb_value] b Second string to concatenate.
 * @return [mrb_value] Returns a new String containing a concatenated to b.
 */
MRB_API mrb_value mrb_str_plus(mrb_state*, mrb_value, mrb_value);

/*
 * Converts pointer into a Ruby string.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [void*] p The pointer to convert to Ruby string.
 * @return [mrb_value] Returns a new Ruby String.
 */
MRB_API mrb_value mrb_ptr_to_str(mrb_state *, void*);

/*
 * Returns an object as a Ruby string.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] obj An object to return as a Ruby string.
 * @return [mrb_value] An object as a Ruby string.
 */
MRB_API mrb_value mrb_obj_as_string(mrb_state *mrb, mrb_value obj);

/*
 * Resizes the string's length. Returns the amount of characters
 * in the specified by len.
 *
 * Example:
 *
 *     !!!c
 *     int
 *     main(int argc,
 *          char **argv)
 *     {
 *         // Variable declaration.
 *         mrb_value str;
 *
 *         mrb_state *mrb = mrb_open();
 *         if (!mrb)
 *         {
 *            // handle error
 *         }
 *         // Creates a new string.
 *         str = mrb_str_new_lit(mrb, "Hello, world!");
 *         // Returns 5 characters of
 *         mrb_str_resize(mrb, str, 5);
 *         mrb_p(mrb, str);
 *
 *         mrb_close(mrb);
 *         return 0;
 *      }
 *
 * Result:
 *
 *     => "Hello"
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str The Ruby string to resize.
 * @param [mrb_value] len The length.
 * @return [mrb_value] An object as a Ruby string.
 */
MRB_API mrb_value mrb_str_resize(mrb_state *mrb, mrb_value str, mrb_int len);

/*
 * Returns a sub string.
 *
 *  Example:
 *
 *     !!!c
 *     int
 *     main(int argc,
 *     char const **argv)
 *     {
 *       // Variable declarations.
 *       mrb_value str1;
 *       mrb_value str2;
 *
 *       mrb_state *mrb = mrb_open();
 *       if (!mrb)
 *       {
 *         // handle error
 *       }
 *       // Creates new string.
 *       str1 = mrb_str_new_lit(mrb, "Hello, world!");
 *       // Returns a sub-string within the range of 0..2
 *       str2 = mrb_str_substr(mrb, str1, 0, 2);
 *
 *       // Prints sub-string.
 *       mrb_p(mrb, str2);
 *
 *       mrb_close(mrb);
 *       return 0;
 *     }
 *
 *  Result:
 *
 *     => "He"
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @param [mrb_int] beg The beginning point of the sub-string.
 * @param [mrb_int] len The end point of the sub-string.
 * @return [mrb_value] An object as a Ruby sub-string.
 */
MRB_API mrb_value mrb_str_substr(mrb_state *mrb, mrb_value str, mrb_int beg, mrb_int len);

/*
 * Returns a Ruby string type.
 *
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @return [mrb_value] A Ruby string.
 */
MRB_API mrb_value mrb_string_type(mrb_state *mrb, mrb_value str);

MRB_API mrb_value mrb_check_string_type(mrb_state *mrb, mrb_value str);
MRB_API mrb_value mrb_str_new_capa(mrb_state *mrb, size_t capa);
MRB_API mrb_value mrb_str_buf_new(mrb_state *mrb, size_t capa);

MRB_API const char *mrb_string_value_cstr(mrb_state *mrb, mrb_value *ptr);
MRB_API const char *mrb_string_value_ptr(mrb_state *mrb, mrb_value str);
/*
 * Returns the length of the Ruby string.
 *
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @return [mrb_int] The length of the passed in Ruby string.
 */
MRB_API mrb_int mrb_string_value_len(mrb_state *mrb, mrb_value str);

/*
 * Duplicates a string object.
 *
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @return [mrb_value] Duplicated Ruby string.
 */
MRB_API mrb_value mrb_str_dup(mrb_state *mrb, mrb_value str);

/*
 * Returns a symbol from a passed in Ruby string.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] self Ruby string.
 * @return [mrb_value] A symbol.
 */
MRB_API mrb_value mrb_str_intern(mrb_state *mrb, mrb_value self);

MRB_API mrb_value mrb_str_to_inum(mrb_state *mrb, mrb_value str, mrb_int base, mrb_bool badcheck);
MRB_API double mrb_str_to_dbl(mrb_state *mrb, mrb_value str, mrb_bool badcheck);

/*
 * Returns a converted string type.
 */
MRB_API mrb_value mrb_str_to_str(mrb_state *mrb, mrb_value str);

/*
 * Returns true if the strings match and false if the strings don't match.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str1 Ruby string to compare.
 * @param [mrb_value] str2 Ruby string to compare.
 * @return [mrb_value] boolean value.
 */
MRB_API mrb_bool mrb_str_equal(mrb_state *mrb, mrb_value str1, mrb_value str2);

/*
 * Returns a concated string comprised of a Ruby string and a C string.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @param [const char *] ptr A C string.
 * @param [size_t] len length of C string.
 * @return [mrb_value] A Ruby string.
 * @see mrb_str_cat_cstr
 */
MRB_API mrb_value mrb_str_cat(mrb_state *mrb, mrb_value str, const char *ptr, size_t len);

/*
 * Returns a concated string comprised of a Ruby string and a C string.
 *
 * @param [mrb_state] mrb The current mruby state.
 * @param [mrb_value] str Ruby string.
 * @param [const char *] ptr A C string.
 * @return [mrb_value] A Ruby string.
 * @see mrb_str_cat
 */
MRB_API mrb_value mrb_str_cat_cstr(mrb_state *mrb, mrb_value str, const char *ptr);
MRB_API mrb_value mrb_str_cat_str(mrb_state *mrb, mrb_value str, mrb_value str2);
#define mrb_str_cat_lit(mrb, str, lit) mrb_str_cat(mrb, str, lit, mrb_strlen_lit(lit))

/*
 * Adds str2 to the end of str1.
 */
MRB_API mrb_value mrb_str_append(mrb_state *mrb, mrb_value str, mrb_value str2);

/*
 * Returns 0 if both Ruby strings are equal. Returns a value < 0 if Ruby str1 is less than Ruby str2. Returns a value > 0 if Ruby str2 is greater than Ruby str1.
 */
MRB_API int mrb_str_cmp(mrb_state *mrb, mrb_value str1, mrb_value str2);

/*
 * Returns a newly allocated C string from a Ruby string.
 * This is an utility function to pass a Ruby string to C library functions.
 *
 * - Returned string does not contain any NUL characters (but terminator).
 * - It raises an ArgumentError exception if Ruby string contains
 *   NUL characters.
 * - Retured string will be freed automatically on next GC.
 * - Caller can modify returned string without affecting Ruby string
 *   (e.g. it can be used for mkstemp(3)).
 *
 * @param [mrb_state *] mrb The current mruby state.
 * @param [mrb_value] str Ruby string. Must be an instance of String.
 * @return [char *] A newly allocated C string.
 */
MRB_API char *mrb_str_to_cstr(mrb_state *mrb, mrb_value str);

mrb_value mrb_str_pool(mrb_state *mrb, mrb_value str);
uint32_t mrb_str_hash(mrb_state *mrb, mrb_value str);
mrb_value mrb_str_dump(mrb_state *mrb, mrb_value str);

/*
 * Returns a printable version of str, surrounded by quote marks, with special characters escaped.
 */
mrb_value mrb_str_inspect(mrb_state *mrb, mrb_value str);

void mrb_noregexp(mrb_state *mrb, mrb_value self);
void mrb_regexp_check(mrb_state *mrb, mrb_value obj);

/* For backward compatibility */
#define mrb_str_cat2(mrb, str, ptr) mrb_str_cat_cstr(mrb, str, ptr)
#define mrb_str_buf_cat(mrb, str, ptr, len) mrb_str_cat(mrb, str, ptr, len)
#define mrb_str_buf_append(mrb, str, str2) mrb_str_cat_str(mrb, str, str2)

MRB_END_DECL

/* end mruby/string.h */

/* start mruby/array.h */
MRB_BEGIN_DECL


typedef struct mrb_shared_array {
  int refcnt;
  mrb_int len;
  mrb_value *ptr;
} mrb_shared_array;

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
  } as;
};

#define mrb_ary_ptr(v)    ((struct RArray*)(mrb_ptr(v)))
#define mrb_ary_value(p)  mrb_obj_value((void*)(p))
#define RARRAY(v)  ((struct RArray*)(mrb_ptr(v)))

#define MRB_ARY_EMBED_MASK  7
#define ARY_EMBED_P(a) ((a)->flags & MRB_ARY_EMBED_MASK)
#define ARY_UNSET_EMBED_FLAG(a) ((a)->flags &= ~(MRB_ARY_EMBED_MASK))
#define ARY_EMBED_LEN(a) ((mrb_int)(((a)->flags & MRB_ARY_EMBED_MASK) - 1))
#define ARY_SET_EMBED_LEN(a,len) ((a)->flags = ((a)->flags&~MRB_ARY_EMBED_MASK) | ((uint32_t)(len) + 1))
#define ARY_EMBED_PTR(a) (&((a)->as.embed[0]))

#define ARY_LEN(a) (ARY_EMBED_P(a)?ARY_EMBED_LEN(a):(a)->as.heap.len)
#define ARY_PTR(a) (ARY_EMBED_P(a)?ARY_EMBED_PTR(a):(a)->as.heap.ptr)
#define RARRAY_LEN(a) ARY_LEN(RARRAY(a))
#define RARRAY_PTR(a) ARY_PTR(RARRAY(a))
#define ARY_SET_LEN(a,n) do {\
  if (ARY_EMBED_P(a)) {\
    mrb_assert((n) <= MRB_ARY_EMBED_LEN_MAX); \
    ARY_SET_EMBED_LEN(a,n);\
  }\
  else\
    (a)->as.heap.len = (n);\
} while (0)
#define ARY_CAPA(a) (ARY_EMBED_P(a)?MRB_ARY_EMBED_LEN_MAX:(a)->as.heap.aux.capa)
#define MRB_ARY_SHARED      256
#define ARY_SHARED_P(a) ((a)->flags & MRB_ARY_SHARED)
#define ARY_SET_SHARED_FLAG(a) ((a)->flags |= MRB_ARY_SHARED)
#define ARY_UNSET_SHARED_FLAG(a) ((a)->flags &= ~MRB_ARY_SHARED)

void mrb_ary_decref(mrb_state*, mrb_shared_array*);
MRB_API void mrb_ary_modify(mrb_state*, struct RArray*);
MRB_API mrb_value mrb_ary_new_capa(mrb_state*, mrb_int);

/*
 * Initializes a new array.
 *
 * Equivalent to:
 *
 *      Array.new
 *
 * @param mrb The mruby state reference.
 * @return The initialized array.
 */
MRB_API mrb_value mrb_ary_new(mrb_state *mrb);

/*
 * Initializes a new array with initial values
 *
 * Equivalent to:
 *
 *      Array[value1, value2, ...]
 *
 * @param mrb The mruby state reference.
 * @param size The numer of values.
 * @param vals The actual values.
 * @return The initialized array.
 */
MRB_API mrb_value mrb_ary_new_from_values(mrb_state *mrb, mrb_int size, const mrb_value *vals);

/*
 * Initializes a new array with two initial values
 *
 * Equivalent to:
 *
 *      Array[car, cdr]
 *
 * @param mrb The mruby state reference.
 * @param car The first value.
 * @param cdr The second value.
 * @return The initialized array.
 */
MRB_API mrb_value mrb_assoc_new(mrb_state *mrb, mrb_value car, mrb_value cdr);

/*
 * Concatenate two arrays. The target array will be modified
 *
 * Equivalent to:
 *      ary.concat(other)
 *
 * @param mrb The mruby state reference.
 * @param self The target array.
 * @param other The array that will be concatenated to self.
 */
MRB_API void mrb_ary_concat(mrb_state *mrb, mrb_value self, mrb_value other);

/*
 * Create an array from the input. It tries calling to_a on the
 * value. If value does not respond to that, it creates a new
 * array with just this value.
 *
 * @param mrb The mruby state reference.
 * @param value The value to change into an array.
 * @return An array representation of value.
 */
MRB_API mrb_value mrb_ary_splat(mrb_state *mrb, mrb_value value);

/*
 * Pushes value into array.
 *
 * Equivalent to:
 *
 *      ary << value
 *
 * @param mrb The mruby state reference.
 * @param ary The array in which the value will be pushed
 * @param value The value to be pushed into array
 */
MRB_API void mrb_ary_push(mrb_state *mrb, mrb_value array, mrb_value value);

/*
 * Pops the last element from the array.
 *
 * Equivalent to:
 *
 *      ary.pop
 *
 * @param mrb The mruby state reference.
 * @param ary The array from which the value will be popped.
 * @return The popped value.
 */
MRB_API mrb_value mrb_ary_pop(mrb_state *mrb, mrb_value ary);

/*
 * Returns a reference to an element of the array on the given index.
 *
 * Equivalent to:
 *
 *      ary[n]
 *
 * @param mrb The mruby state reference.
 * @param ary The target array.
 * @param n The array index being referenced
 * @return The referenced value.
 */
MRB_API mrb_value mrb_ary_ref(mrb_state *mrb, mrb_value ary, mrb_int n);

/*
 * Sets a value on an array at the given index
 *
 * Equivalent to:
 *
 *      ary[n] = val
 *
 * @param mrb The mruby state reference.
 * @param ary The target array.
 * @param n The array index being referenced.
 * @param val The value being setted.
 */
MRB_API void mrb_ary_set(mrb_state *mrb, mrb_value ary, mrb_int n, mrb_value val);

/*
 * Replace the array with another array
 *
 * Equivalent to:
 *
 *      ary.replace(other)
 *
 * @param mrb The mruby state reference
 * @param self The target array.
 * @param other The array to replace it with.
 */
MRB_API void mrb_ary_replace(mrb_state *mrb, mrb_value self, mrb_value other);
MRB_API mrb_value mrb_check_array_type(mrb_state *mrb, mrb_value self);

/*
 * Unshift an element into the array
 *
 * Equivalent to:
 *
 *     ary.unshift(item)
 *
 * @param mrb The mruby state reference.
 * @param self The target array.
 * @param item The item to unshift.
 */
MRB_API mrb_value mrb_ary_unshift(mrb_state *mrb, mrb_value self, mrb_value item);

/*
 * Get nth element in the array
 *
 * Equivalent to:
 *
 *     ary[offset]
 *
 * @param ary The target array.
 * @param offset The element position (negative counts from the tail).
 */
MRB_API mrb_value mrb_ary_entry(mrb_value ary, mrb_int offset);

/*
 * Shifts the first element from the array.
 *
 * Equivalent to:
 *
 *      ary.shift
 *
 * @param mrb The mruby state reference.
 * @param self The array from which the value will be shifted.
 * @return The shifted value.
 */
MRB_API mrb_value mrb_ary_shift(mrb_state *mrb, mrb_value self);

/*
 * Removes all elements from the array
 *
 * Equivalent to:
 *
 *      ary.clear
 *
 * @param mrb The mruby state reference.
 * @param self The target array.
 * @return self
 */
MRB_API mrb_value mrb_ary_clear(mrb_state *mrb, mrb_value self);

/*
 * Join the array elements together in a string
 *
 * Equivalent to:
 *
 *      ary.join(sep="")
 *
 * @param mrb The mruby state reference.
 * @param ary The target array
 * @param sep The separater, can be NULL
 */
MRB_API mrb_value mrb_ary_join(mrb_state *mrb, mrb_value ary, mrb_value sep);

/*
 * Update the capacity of the array
 *
 * @param mrb The mruby state reference.
 * @param ary The target array.
 * @param new_len The new capacity of the array
 */
MRB_API mrb_value mrb_ary_resize(mrb_state *mrb, mrb_value ary, mrb_int new_len);

MRB_END_DECL
/* end mruby/array.h */

/* start mruby/class.h */
MRB_BEGIN_DECL

struct RClass {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  struct kh_mt *mt;
  struct RClass *super;
};

#define mrb_class_ptr(v)    ((struct RClass*)(mrb_ptr(v)))
#define RCLASS_SUPER(v)     (((struct RClass*)(mrb_ptr(v)))->super)
#define RCLASS_IV_TBL(v)    (((struct RClass*)(mrb_ptr(v)))->iv)
#define RCLASS_M_TBL(v)     (((struct RClass*)(mrb_ptr(v)))->mt)

static inline struct RClass*
mrb_class(mrb_state *mrb, mrb_value v)
{
  switch (mrb_type(v)) {
  case MRB_TT_FALSE:
    if (mrb_fixnum(v))
      return mrb->false_class;
    return mrb->nil_class;
  case MRB_TT_TRUE:
    return mrb->true_class;
  case MRB_TT_SYMBOL:
    return mrb->symbol_class;
  case MRB_TT_FIXNUM:
    return mrb->fixnum_class;
#ifndef MRB_WITHOUT_FLOAT
  case MRB_TT_FLOAT:
    return mrb->float_class;
#endif
  case MRB_TT_CPTR:
    return mrb->object_class;
  case MRB_TT_ENV:
    return NULL;
  default:
    return mrb_obj_ptr(v)->c;
  }
}

/* TODO: figure out where to put user flags */
/* flags bits >= 18 is reserved */
#define MRB_FLAG_IS_PREPENDED (1 << 19)
#define MRB_FLAG_IS_ORIGIN (1 << 20)
#define MRB_CLASS_ORIGIN(c) do {\
  if (c->flags & MRB_FLAG_IS_PREPENDED) {\
    c = c->super;\
    while (!(c->flags & MRB_FLAG_IS_ORIGIN)) {\
      c = c->super;\
    }\
  }\
} while (0)
#define MRB_FLAG_IS_INHERITED (1 << 21)
#define MRB_INSTANCE_TT_MASK (0xFF)
#define MRB_SET_INSTANCE_TT(c, tt) c->flags = ((c->flags & ~MRB_INSTANCE_TT_MASK) | (char)tt)
#define MRB_INSTANCE_TT(c) (enum mrb_vtype)(c->flags & MRB_INSTANCE_TT_MASK)

MRB_API struct RClass* mrb_define_class_id(mrb_state*, mrb_sym, struct RClass*);
MRB_API struct RClass* mrb_define_module_id(mrb_state*, mrb_sym);
MRB_API struct RClass *mrb_vm_define_class(mrb_state*, mrb_value, mrb_value, mrb_sym);
MRB_API struct RClass *mrb_vm_define_module(mrb_state*, mrb_value, mrb_sym);
MRB_API void mrb_define_method_raw(mrb_state*, struct RClass*, mrb_sym, mrb_method_t);
MRB_API void mrb_define_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid, mrb_func_t func, mrb_aspec aspec);
MRB_API void mrb_alias_method(mrb_state *mrb, struct RClass *c, mrb_sym a, mrb_sym b);

MRB_API mrb_method_t mrb_method_search_vm(mrb_state*, struct RClass**, mrb_sym);
MRB_API mrb_method_t mrb_method_search(mrb_state*, struct RClass*, mrb_sym);

MRB_API struct RClass* mrb_class_real(struct RClass* cl);

void mrb_class_name_class(mrb_state*, struct RClass*, struct RClass*, mrb_sym);
mrb_value mrb_class_find_path(mrb_state*, struct RClass*);
void mrb_gc_mark_mt(mrb_state*, struct RClass*);
size_t mrb_gc_mark_mt_size(mrb_state*, struct RClass*);
void mrb_gc_free_mt(mrb_state*, struct RClass*);

MRB_END_DECL
/* end mruby/class.h */

/* start mruby/range.h */
MRB_BEGIN_DECL

typedef struct mrb_range_edges {
  mrb_value beg;
  mrb_value end;
} mrb_range_edges;

struct RRange {
  MRB_OBJECT_HEADER;
  mrb_range_edges *edges;
  mrb_bool excl : 1;
};

MRB_API struct RRange* mrb_range_ptr(mrb_state *mrb, mrb_value v);
#define mrb_range_raw_ptr(v) ((struct RRange*)mrb_ptr(v))
#define mrb_range_value(p)  mrb_obj_value((void*)(p))

/*
 * Initializes a Range.
 *
 * If the third parameter is FALSE then it includes the last value in the range.
 * If the third parameter is TRUE then it excludes the last value in the range.
 *
 * @param start the beginning value.
 * @param end the ending value.
 * @param exclude represents the inclusion or exclusion of the last value.
 */
MRB_API mrb_value mrb_range_new(mrb_state *mrb, mrb_value start, mrb_value end, mrb_bool exclude);

MRB_API mrb_int mrb_range_beg_len(mrb_state *mrb, mrb_value range, mrb_int *begp, mrb_int *lenp, mrb_int len, mrb_bool trunc);
mrb_value mrb_get_values_at(mrb_state *mrb, mrb_value obj, mrb_int olen, mrb_int argc, const mrb_value *argv, mrb_value (*func)(mrb_state*, mrb_value, mrb_int));

MRB_END_DECL
/* end mruby/range.h */

/* start mruby/irep.h */
MRB_BEGIN_DECL

enum irep_pool_type {
  IREP_TT_STRING,
  IREP_TT_FIXNUM,
  IREP_TT_FLOAT,
};

struct mrb_locals {
  mrb_sym name;
  uint16_t r;
};

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

#define MRB_ISEQ_NO_FREE 1

MRB_API mrb_irep *mrb_add_irep(mrb_state *mrb);
MRB_API mrb_value mrb_load_irep(mrb_state*, const uint8_t*);
MRB_API mrb_value mrb_load_irep_cxt(mrb_state*, const uint8_t*, mrbc_context*);
void mrb_irep_free(mrb_state*, struct mrb_irep*);
void mrb_irep_incref(mrb_state*, struct mrb_irep*);
void mrb_irep_decref(mrb_state*, struct mrb_irep*);
void mrb_irep_cutref(mrb_state*, struct mrb_irep*);

MRB_END_DECL
/* end mruby/irep.h */

/* start mruby/proc.h */
MRB_BEGIN_DECL

struct REnv {
  MRB_OBJECT_HEADER;
  mrb_value *stack;
  struct mrb_context *cxt;
  mrb_sym mid;
};

/* flags (21bits): 1(shared flag):10(cioff/bidx):10(stack_len) */
#define MRB_ENV_SET_STACK_LEN(e,len) (e)->flags = (((e)->flags & ~0x3ff)|((unsigned int)(len) & 0x3ff))
#define MRB_ENV_STACK_LEN(e) ((mrb_int)((e)->flags & 0x3ff))
#define MRB_ENV_STACK_UNSHARED (1<<20)
#define MRB_ENV_UNSHARE_STACK(e) (e)->flags |= MRB_ENV_STACK_UNSHARED
#define MRB_ENV_STACK_SHARED_P(e) (((e)->flags & MRB_ENV_STACK_UNSHARED) == 0)
#define MRB_ENV_BIDX(e) (((e)->flags >> 10) & 0x3ff)
#define MRB_ENV_SET_BIDX(e,idx) (e)->flags = (((e)->flags & ~(0x3ff<<10))|((unsigned int)(idx) & 0x3ff)<<10)

void mrb_env_unshare(mrb_state*, struct REnv*);

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

/* aspec access */
#define MRB_ASPEC_REQ(a)          (((a) >> 18) & 0x1f)
#define MRB_ASPEC_OPT(a)          (((a) >> 13) & 0x1f)
#define MRB_ASPEC_REST(a)         (((a) >> 12) & 0x1)
#define MRB_ASPEC_POST(a)         (((a) >> 7) & 0x1f)
#define MRB_ASPEC_KEY(a)          (((a) >> 2) & 0x1f)
#define MRB_ASPEC_KDICT(a)        ((a) & (1<<1))
#define MRB_ASPEC_BLOCK(a)        ((a) & 1)

#define MRB_PROC_CFUNC_FL 128
#define MRB_PROC_CFUNC_P(p) (((p)->flags & MRB_PROC_CFUNC_FL) != 0)
#define MRB_PROC_CFUNC(p) (p)->body.func
#define MRB_PROC_STRICT 256
#define MRB_PROC_STRICT_P(p) (((p)->flags & MRB_PROC_STRICT) != 0)
#define MRB_PROC_ORPHAN 512
#define MRB_PROC_ORPHAN_P(p) (((p)->flags & MRB_PROC_ORPHAN) != 0)
#define MRB_PROC_ENVSET 1024
#define MRB_PROC_ENV_P(p) (((p)->flags & MRB_PROC_ENVSET) != 0)
#define MRB_PROC_ENV(p) (MRB_PROC_ENV_P(p) ? (p)->e.env : NULL)
#define MRB_PROC_TARGET_CLASS(p) (MRB_PROC_ENV_P(p) ? (p)->e.env->c : (p)->e.target_class)
#define MRB_PROC_SET_TARGET_CLASS(p,tc) do {\
  if (MRB_PROC_ENV_P(p)) {\
    (p)->e.env->c = (tc);\
    mrb_field_write_barrier(mrb, (struct RBasic*)(p)->e.env, (struct RBasic*)tc);\
  }\
  else {\
    (p)->e.target_class = (tc);\
    mrb_field_write_barrier(mrb, (struct RBasic*)p, (struct RBasic*)tc);\
  }\
} while (0)
#define MRB_PROC_SCOPE 2048
#define MRB_PROC_SCOPE_P(p) (((p)->flags & MRB_PROC_SCOPE) != 0)

#define mrb_proc_ptr(v)    ((struct RProc*)(mrb_ptr(v)))

struct RProc *mrb_proc_new(mrb_state*, mrb_irep*);
struct RProc *mrb_closure_new(mrb_state*, mrb_irep*);
MRB_API struct RProc *mrb_proc_new_cfunc(mrb_state*, mrb_func_t);
MRB_API struct RProc *mrb_closure_new_cfunc(mrb_state *mrb, mrb_func_t func, int nlocals);
void mrb_proc_copy(struct RProc *a, struct RProc *b);

/* implementation of #send method */
MRB_API mrb_value mrb_f_send(mrb_state *mrb, mrb_value self);

/* following functions are defined in mruby-proc-ext so please include it when using */
MRB_API struct RProc *mrb_proc_new_cfunc_with_env(mrb_state*, mrb_func_t, mrb_int, const mrb_value*);
MRB_API mrb_value mrb_proc_cfunc_env_get(mrb_state*, mrb_int);
/* old name */
#define mrb_cfunc_env_get(mrb, idx) mrb_proc_cfunc_env_get(mrb, idx)

#ifdef MRB_METHOD_TABLE_INLINE

#define MRB_METHOD_FUNC_FL ((uintptr_t)1U<<(sizeof(uintptr_t)*8-1))
#define MRB_METHOD_FUNC_P(m) ((uintptr_t)(m)&MRB_METHOD_FUNC_FL)
#define MRB_METHOD_FUNC(m) ((mrb_func_t)((uintptr_t)(m)&(~MRB_METHOD_FUNC_FL)))
#define MRB_METHOD_FROM_FUNC(m,fn) m=(mrb_method_t)((struct RProc*)((uintptr_t)(fn)|MRB_METHOD_FUNC_FL))
#define MRB_METHOD_FROM_PROC(m,pr) m=(mrb_method_t)(struct RProc*)(pr)
#define MRB_METHOD_PROC_P(m) (!MRB_METHOD_FUNC_P(m))
#define MRB_METHOD_PROC(m) ((struct RProc*)(m))
#define MRB_METHOD_UNDEF_P(m) ((m)==0)

#else

#define MRB_METHOD_FUNC_P(m) ((m).func_p)
#define MRB_METHOD_FUNC(m) ((m).func)
#define MRB_METHOD_FROM_FUNC(m,fn) do{(m).func_p=TRUE;(m).func=(fn);}while(0)
#define MRB_METHOD_FROM_PROC(m,pr) do{(m).func_p=FALSE;(m).proc=(pr);}while(0)
#define MRB_METHOD_PROC_P(m) (!MRB_METHOD_FUNC_P(m))
#define MRB_METHOD_PROC(m) ((m).proc)
#define MRB_METHOD_UNDEF_P(m) ((m).proc==NULL)

#endif /* MRB_METHOD_TABLE_INLINE */

#define MRB_METHOD_CFUNC_P(m) (MRB_METHOD_FUNC_P(m)?TRUE:(MRB_METHOD_PROC(m)?(MRB_PROC_CFUNC_P(MRB_METHOD_PROC(m))):FALSE))
#define MRB_METHOD_CFUNC(m) (MRB_METHOD_FUNC_P(m)?MRB_METHOD_FUNC(m):((MRB_METHOD_PROC(m)&&MRB_PROC_CFUNC_P(MRB_METHOD_PROC(m)))?MRB_PROC_CFUNC(MRB_METHOD_PROC(m)):NULL))

KHASH_DECLARE(mt, mrb_sym, mrb_method_t, TRUE)

MRB_END_DECL
/* end mruby/proc.h */

/* start mruby/variable.h */
MRB_BEGIN_DECL

typedef struct global_variable {
  int   counter;
  mrb_value *data;
  mrb_value (*getter)(void);
  void  (*setter)(void);
  /* void  (*marker)(); */
  /* int block_trace; */
  /* struct trace_var *trace; */
} global_variable;

struct global_entry {
  global_variable *var;
  mrb_sym id;
};

mrb_value mrb_vm_special_get(mrb_state*, mrb_sym);
void mrb_vm_special_set(mrb_state*, mrb_sym, mrb_value);
mrb_value mrb_vm_iv_get(mrb_state*, mrb_sym);
void mrb_vm_iv_set(mrb_state*, mrb_sym, mrb_value);
mrb_value mrb_vm_cv_get(mrb_state*, mrb_sym);
void mrb_vm_cv_set(mrb_state*, mrb_sym, mrb_value);
mrb_value mrb_vm_const_get(mrb_state*, mrb_sym);
void mrb_vm_const_set(mrb_state*, mrb_sym, mrb_value);
MRB_API mrb_value mrb_const_get(mrb_state*, mrb_value, mrb_sym);
MRB_API void mrb_const_set(mrb_state*, mrb_value, mrb_sym, mrb_value);
MRB_API mrb_bool mrb_const_defined(mrb_state*, mrb_value, mrb_sym);
MRB_API void mrb_const_remove(mrb_state*, mrb_value, mrb_sym);

MRB_API mrb_bool mrb_iv_p(mrb_state *mrb, mrb_sym sym);
MRB_API void mrb_iv_check(mrb_state *mrb, mrb_sym sym);
MRB_API mrb_value mrb_obj_iv_get(mrb_state *mrb, struct RObject *obj, mrb_sym sym);
MRB_API void mrb_obj_iv_set(mrb_state *mrb, struct RObject *obj, mrb_sym sym, mrb_value v);
MRB_API mrb_bool mrb_obj_iv_defined(mrb_state *mrb, struct RObject *obj, mrb_sym sym);
MRB_API mrb_value mrb_iv_get(mrb_state *mrb, mrb_value obj, mrb_sym sym);
MRB_API void mrb_iv_set(mrb_state *mrb, mrb_value obj, mrb_sym sym, mrb_value v);
MRB_API mrb_bool mrb_iv_defined(mrb_state*, mrb_value, mrb_sym);
MRB_API mrb_value mrb_iv_remove(mrb_state *mrb, mrb_value obj, mrb_sym sym);
MRB_API void mrb_iv_copy(mrb_state *mrb, mrb_value dst, mrb_value src);
MRB_API mrb_bool mrb_const_defined_at(mrb_state *mrb, mrb_value mod, mrb_sym id);

/**
 * Get a global variable. Will return nil if the var does not exist
 *
 * Example:
 *
 *     !!!ruby
 *     # Ruby style
 *     var = $value
 *
 *     !!!c
 *     // C style
 *     mrb_sym sym = mrb_intern_lit(mrb, "$value");
 *     mrb_value var = mrb_gv_get(mrb, sym);
 *
 * @param mrb The mruby state reference
 * @param sym The name of the global variable
 * @return The value of that global variable. May be nil
 */
MRB_API mrb_value mrb_gv_get(mrb_state *mrb, mrb_sym sym);

/**
 * Set a global variable
 *
 * Example:
 *
 *     !!!ruby
 *     # Ruby style
 *     $value = "foo"
 *
 *     !!!c
 *     // C style
 *     mrb_sym sym = mrb_intern_lit(mrb, "$value");
 *     mrb_gv_set(mrb, sym, mrb_str_new_lit("foo"));
 *
 * @param mrb The mruby state reference
 * @param sym The name of the global variable
 * @param val The value of the global variable
 */
MRB_API void mrb_gv_set(mrb_state *mrb, mrb_sym sym, mrb_value val);

/**
 * Remove a global variable.
 *
 * Example:
 *
 *     !!!ruby
 *     # Ruby style
 *     $value = nil
 *
 *     !!!c
 *     // C style
 *     mrb_sym sym = mrb_intern_lit(mrb, "$value");
 *     mrb_gv_remove(mrb, sym);
 *
 * @param mrb The mruby state reference
 * @param sym The name of the global variable
 * @param val The value of the global variable
 */
MRB_API void mrb_gv_remove(mrb_state *mrb, mrb_sym sym);

MRB_API mrb_value mrb_cv_get(mrb_state *mrb, mrb_value mod, mrb_sym sym);
MRB_API void mrb_mod_cv_set(mrb_state *mrb, struct RClass * c, mrb_sym sym, mrb_value v);
MRB_API void mrb_cv_set(mrb_state *mrb, mrb_value mod, mrb_sym sym, mrb_value v);
MRB_API mrb_bool mrb_cv_defined(mrb_state *mrb, mrb_value mod, mrb_sym sym);
mrb_value mrb_obj_iv_inspect(mrb_state*, struct RObject*);
mrb_value mrb_mod_constants(mrb_state *mrb, mrb_value mod);
mrb_value mrb_f_global_variables(mrb_state *mrb, mrb_value self);
mrb_value mrb_obj_instance_variables(mrb_state*, mrb_value);
mrb_value mrb_mod_class_variables(mrb_state*, mrb_value);
mrb_value mrb_mod_cv_get(mrb_state *mrb, struct RClass * c, mrb_sym sym);
mrb_bool mrb_mod_cv_defined(mrb_state *mrb, struct RClass * c, mrb_sym sym);

/* GC functions */
void mrb_gc_mark_gv(mrb_state*);
void mrb_gc_free_gv(mrb_state*);
void mrb_gc_mark_iv(mrb_state*, struct RObject*);
size_t mrb_gc_mark_iv_size(mrb_state*, struct RObject*);
void mrb_gc_free_iv(mrb_state*, struct RObject*);

MRB_END_DECL
/* end mruby/variable.h */

/* start mruby/data.h */
MRB_BEGIN_DECL

/**
 * Custom data type description.
 */
typedef struct mrb_data_type {
  /** data type name */
  const char *struct_name;

  /** data type release function pointer */
  void (*dfree)(mrb_state *mrb, void*);
} mrb_data_type;

struct RData {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  const mrb_data_type *type;
  void *data;
};

MRB_API struct RData *mrb_data_object_alloc(mrb_state *mrb, struct RClass* klass, void *datap, const mrb_data_type *type);

#define Data_Wrap_Struct(mrb,klass,type,ptr)\
  mrb_data_object_alloc(mrb,klass,ptr,type)

#define Data_Make_Struct(mrb,klass,strct,type,sval,data) do { \
  sval = mrb_malloc(mrb, sizeof(strct));                     \
  { static const strct zero = { 0 }; *sval = zero; };\
  data = Data_Wrap_Struct(mrb,klass,type,sval);\
} while (0)

#define RDATA(obj)         ((struct RData *)(mrb_ptr(obj)))
#define DATA_PTR(d)        (RDATA(d)->data)
#define DATA_TYPE(d)       (RDATA(d)->type)
MRB_API void mrb_data_check_type(mrb_state *mrb, mrb_value, const mrb_data_type*);
MRB_API void *mrb_data_get_ptr(mrb_state *mrb, mrb_value, const mrb_data_type*);
#define DATA_GET_PTR(mrb,obj,dtype,type) (type*)mrb_data_get_ptr(mrb,obj,dtype)
MRB_API void *mrb_data_check_get_ptr(mrb_state *mrb, mrb_value, const mrb_data_type*);
#define DATA_CHECK_GET_PTR(mrb,obj,dtype,type) (type*)mrb_data_check_get_ptr(mrb,obj,dtype)

/* obsolete functions and macros */
#define mrb_data_check_and_get(mrb,obj,dtype) mrb_data_get_ptr(mrb,obj,dtype)
#define mrb_get_datatype(mrb,val,type) mrb_data_get_ptr(mrb, val, type)
#define mrb_check_datatype(mrb,val,type) mrb_data_get_ptr(mrb, val, type)
#define Data_Get_Struct(mrb,obj,type,sval) do {\
  *(void**)&sval = mrb_data_get_ptr(mrb, obj, type); \
} while (0)

static inline void
mrb_data_init(mrb_value v, void *ptr, const mrb_data_type *type)
{
  mrb_assert(mrb_type(v) == MRB_TT_DATA);
  DATA_PTR(v) = ptr;
  DATA_TYPE(v) = type;
}

MRB_END_DECL
/* end mruby/data.h */

/* start mruby/debug.h */
MRB_BEGIN_DECL

typedef enum mrb_debug_line_type {
  mrb_debug_line_ary = 0,
  mrb_debug_line_flat_map = 1
} mrb_debug_line_type;

typedef struct mrb_irep_debug_info_line {
  uint32_t start_pos;
  uint16_t line;
} mrb_irep_debug_info_line;

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

typedef struct mrb_irep_debug_info {
  uint32_t pc_count;
  uint16_t flen;
  mrb_irep_debug_info_file **files;
} mrb_irep_debug_info;

/*
 * get line from irep's debug info and program counter
 * @return returns NULL if not found
 */
MRB_API const char *mrb_debug_get_filename(mrb_irep *irep, ptrdiff_t pc);

/*
 * get line from irep's debug info and program counter
 * @return returns -1 if not found
 */
MRB_API int32_t mrb_debug_get_line(mrb_irep *irep, ptrdiff_t pc);

MRB_API mrb_irep_debug_info_file *mrb_debug_info_append_file(
    mrb_state *mrb, mrb_irep *irep,
    uint32_t start_pos, uint32_t end_pos);
MRB_API mrb_irep_debug_info *mrb_debug_info_alloc(mrb_state *mrb, mrb_irep *irep);
MRB_API void mrb_debug_info_free(mrb_state *mrb, mrb_irep_debug_info *d);

MRB_END_DECL
/* end mruby/debug.h */
