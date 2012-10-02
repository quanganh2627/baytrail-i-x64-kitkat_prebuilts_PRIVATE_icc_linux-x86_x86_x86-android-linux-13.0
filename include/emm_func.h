/**
***
*** Copyright (C) 1985-2007 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part except as expressly provided
*** by the accompanying LICENSE AGREEMENT
***
**/

/*
 * emm_func.h
 *
 * Header for Intel(R) Pentium(R) 4 processor SSE2 intrinsics 
 * (the functionality intrinsics, i.e. "compatability" mode)
 *
 * Should be included only by emmintrin.h
 */

#ifndef _INCLUDED_EMM
#error Header should only be included from emmintrin.h
#endif

#ifndef _INCLUDED_EMM_FUNC
#define _INCLUDED_EMM_FUNC

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * this is the bit and element numbering convention we will use
 * for __m128d:
 *  127...64  63...0
 *       [1]     [0]
 */

/*
 * WARNING: It is illegal to access elements of an object of type
 *          __m128d or __m128i.  Accessing these elements will cause a
 *          compilation failure when compiling with real intrinsics.
 */
/* 
 * __m128d type declaration 
 */
typedef struct {
  double d[2];
} __m128d;

/*
 * __m128i type declaration
 */
typedef struct {
  union {
    signed char b[16];
    unsigned char ub[16];
    signed short w[8];
    unsigned short uw[8];
    signed int i[4];
    unsigned int ui[4];
    __int64 m[2];
    unsigned __int64 um[2];
  } m128i;
} __m128i;

/* utility for __m128 */
typedef struct {
	union{
		float f[4];
		__m128 m;
	} m128s;
} __m128s;

/* utility for double */
typedef union 
{
   struct {
      unsigned int manlo;
      unsigned int manhi: 20;
      unsigned int exp:11;
      unsigned int sign:1;
   } bits;
   double       d64;
} dbits;

typedef union {
  char sb[8];
  unsigned char ub[8];
  short sw[4];
  unsigned short uw[4];
  int si[2];
  unsigned int ui[2];
  __int64 s64;
  unsigned __int64 u64;
  __m64 m;
} __um64;

/* common 64-bit constant */
#define _MM_BITONE64                (0xffffffffffffffff)

/*
 * If _MM_FUNCTIONALITY defined, do not define these.  Already
 * defined in xmm_func.h 
 */
#ifndef _MM_FUNCTIONALITY

#ifndef _MM_NO_INLINE
#define _MM_INLINE_COMMAND             __inline
#else /* _MM_NO_INLINE */
#define _MM_INLINE_COMMAND
#endif /* _MM_NO_INLINE */

/* MIN and MAX macros for FPs and INTs*/
#define _MM_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define _MM_MAX(a,b)            (((a) > (b)) ? (a) : (b))

#ifndef _MM_NO_ABORT
#define _mminternal_abort(str) \
  { fprintf(stderr, "*** Functionality intrinsics error: %s ***\n", str); \
    exit(1); }
#else
#define _mminternal_abort(str) \
  { fprintf(stderr, "*** Functionality intrinsics warning: %s ***\n", str); }
#endif

/*
 * check to make sure addresses are 16-byte aligned
 */
#ifdef NO_ALIGN_CHECK
#ifndef _MM_NO_ALIGN_CHECK
#define _MM_NO_ALIGN_CHECK
#endif
#endif

#ifndef _MM_NO_ALIGN_CHECK
#define _mminternal_assert_16B(addr) \
  if ((unsigned)addr % 16 != 0) { \
    _mminternal_abort("address must be 16-byte aligned"); \
  }
#else
#define _mminternal_assert_16B(addr) ;
#endif

#ifndef _MM_NO_ACCURACY
/*
 * return same precision results as via emulator
 */
_MM_INLINE_COMMAND static double __cdecl _mminternal_sqrt(double src)
{
  double ret;
	_asm	{
		fld QWORD PTR src
		fsqrt
		fstp ret;
	}
  return ret;
}

#else /* _MM_NO_ACCURACY */
#define _mminternal_sqrt(x) ((double)sqrt((double)x))
#endif /* _MM_NO_ACCURACY */

#endif /* _MM_FUNCTIONALITY */

/* 
 * Conversion routines: for cvt ops, we do the MXCSR default, which
 * is round to nearest 
 */
_MM_INLINE_COMMAND static int _emm_round_nearest(double d) {
  int ret;
	_asm	{
		fld d
		fistp ret
	}
  return ret;
}

_MM_INLINE_COMMAND static double _emm_float2double (float f) {
  double ret;

  __asm {
    fld  f;
    fstp ret;
  }

  return ret;
}

_MM_INLINE_COMMAND static float _emm_double2float (double d) {
  float ret;

  __asm {
    fld  d;
    fstp ret;
  }

  return ret;
}

/* 
 * For cvtt ops, we do round with truncation, so we need to mess with
 * the MXCSR 
 */
_MM_INLINE_COMMAND static int _emm_round_trunc(double d) {
  int ret;
  int saved_cw;
  int new_cw;
	_asm	{
                push      eax
                fld       d
                fstcw     saved_cw
                mov       eax, saved_cw
                or        eax, 3072
                mov       new_cw, eax
                fldcw     new_cw
                fistp     ret
                fldcw     saved_cw
                pop       eax
	}
  return ret;
}



/*********************************************************/
/* MACROs to use for accessing struct FPs in m128s types.*/
/* MM_FPS0 is the lowest FP and MM_FPS3 the highest      */
/*********************************************************/
#define _MM_FPS0(ffps)                (ffps.m128s.f[0])
#define _MM_FPS1(ffps)                (ffps.m128s.f[1])
#define _MM_FPS2(ffps)                (ffps.m128s.f[2])
#define _MM_FPS3(ffps)                (ffps.m128s.f[3])
/*********************************************************/
/* MACROs to use for accessing struct DP FPs in m128d.   */
/* MM_DP0 is the lowest DP and MM_DP1 the highest DP     */
/*********************************************************/
#define _MM_DP0(dpfps)                (dpfps.d[0])
#define _MM_DP1(dpfps)                (dpfps.d[1])
/***********************************************************/
/* MACROs to use for accessing m128 struct FPs as ints.    */
/* MM_INT0 is the lowest __INT64 and MM_INT641 the highest */
/***********************************************************/
#define _MM_INT640(dpfps)                (*((__int64 *)(&(dpfps.d[0]))))
#define _MM_INT641(dpfps)                (*((__int64 *)(&(dpfps.d[1]))))



/****************************************************/
/*       FUNC INTRINSIC FUNCTIONS START HERE        */
/****************************************************/

/****************************************************/
/*  NAME : mm_add_sd                                */
/*  DESCRIPTION : scalar single DP FP add           */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_add_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) + _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_add_pd                                */
/*  DESCRIPTION : packed DP FP add                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_add_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) + _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a) + _MM_DP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_sub_sd                                */
/*  DESCRIPTION : scalar single DP FP subtract      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_sub_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) - _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_sub_pd                                */
/*  DESCRIPTION : packed DP FP subtract             */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_sub_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) - _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a) - _MM_DP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_mul_sd                                */
/*  DESCRIPTION : scalar single DP FP multiply      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_mul_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) * _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_mul_pd                                */
/*  DESCRIPTION : packed DP FP multiply             */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_mul_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) * _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a) * _MM_DP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_sqrt_sd                               */
/*  DESCRIPTION : scalar single DP FP square root   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_sqrt_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _mminternal_sqrt(_MM_DP0(b));
  _MM_DP1(ret) = _MM_DP1(a);
  return ret;
}

/****************************************************/
/*  NAME : mm_sqrt_pd                               */
/*  DESCRIPTION : packed DP FP square root          */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_sqrt_pd(__m128d a)
{
  __m128d ret;

  _MM_DP0(ret) = _mminternal_sqrt(_MM_DP0(a));
  _MM_DP1(ret) = _mminternal_sqrt(_MM_DP1(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_div_sd                                */
/*  DESCRIPTION : scalar single DP FP divide        */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_div_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) / _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_div_pd                                */
/*  DESCRIPTION : packed DP FP divide               */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_div_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a) / _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a) / _MM_DP1(b);

  return ret;
}

/******************************************************/
/*  NAME : mm_min_sd                                  */
/*  DESCRIPTION : scalar single DP FP minimum         */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128d _mm_min_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_MIN(_MM_DP0(a), _MM_DP0(b));
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/******************************************************/
/*  NAME : mm_min_pd                                  */
/*  DESCRIPTION : packed DP FP minimum                */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128d _mm_min_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_MIN(_MM_DP0(a), _MM_DP0(b));
  _MM_DP1(ret) = _MM_MIN(_MM_DP1(a), _MM_DP1(b));

  return ret;
}

/******************************************************/
/*  NAME : mm_max_sd                                  */
/*  DESCRIPTION : scalar single DP FP maximum         */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128d _mm_max_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_MAX(_MM_DP0(a), _MM_DP0(b));
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/******************************************************/
/*  NAME : mm_max_pd                                  */
/*  DESCRIPTION : packed single DP FP minimum         */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128d _mm_max_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_MAX(_MM_DP0(a), _MM_DP0(b));
  _MM_DP1(ret) = _MM_MAX(_MM_DP1(a), _MM_DP1(b));

  return ret;
}

/* Logical Operations */

/****************************************************/
/*  NAME : mm_and_pd                                */
/*  DESCRIPTION : bitwise logical AND               */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_and_pd(__m128d a, __m128d b)
{
  __m128d ret;
  
  _MM_INT640(ret) = _MM_INT640(a) & _MM_INT640(b);
  _MM_INT641(ret) = _MM_INT641(a) & _MM_INT641(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_andnot_pd                             */
/*  DESCRIPTION : bitwise logical ANDNOT            */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_andnot_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (~_MM_INT640(a)) & _MM_INT640(b);
  _MM_INT641(ret) = (~_MM_INT641(a)) & _MM_INT641(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_or_pd                                 */
/*  DESCRIPTION : bitwise logical OR                */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_or_pd(__m128d a, __m128d b)
{
  __m128d ret;
  
  _MM_INT640(ret) = _MM_INT640(a) | _MM_INT640(b);
  _MM_INT641(ret) = _MM_INT641(a) | _MM_INT641(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_xor_pd                                */
/*  DESCRIPTION : bitwise logical XOR               */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_xor_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = _MM_INT640(a) ^ _MM_INT640(b);
  _MM_INT641(ret) = _MM_INT641(a) ^ _MM_INT641(b);

  return ret;
}

/* Comparisons */

/****************************************************/
/*  NAME : mm_cmpeq_sd                              */
/*  DESCRIPTION : scalar single DP FP compare equal */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpeq_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (_MM_DP0(a) == _MM_DP0(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpeq_pd                              */
/*  DESCRIPTION : packed single DP FP compare equal */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpeq_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (_MM_DP0(a) == _MM_DP0(b)) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (_MM_DP1(a) == _MM_DP1(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmplt_sd                              */
/*  DESCRIPTION : scalar single DP FP less than     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmplt_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (_MM_DP0(a) < _MM_DP0(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmplt_pd                              */
/*  DESCRIPTION : packed single DP FP less than     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmplt_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (_MM_DP0(a) < _MM_DP0(b)) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (_MM_DP1(a) < _MM_DP1(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmple_sd                              */
/*  DESCRIPTION : scalar DP FP less than equal      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmple_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (_MM_DP0(a) <= _MM_DP0(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmple_pd                              */
/*  DESCRIPTION : packed DP FP less than equal      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmple_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (_MM_DP0(a) <= _MM_DP0(b)) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (_MM_DP1(a) <= _MM_DP1(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_sd                              */
/*  DESCRIPTION : scalar single DP greater than     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpgt_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (_MM_DP0(a) > _MM_DP0(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_pd                              */
/*  DESCRIPTION : packed single DP FP greater than  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpgt_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (_MM_DP0(a) > _MM_DP0(b)) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (_MM_DP1(a) > _MM_DP1(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpge_sd                              */
/*  DESCRIPTION : scalar single DP FP greater equal */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpge_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (_MM_DP0(a) >= _MM_DP0(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpge_pd                              */
/*  DESCRIPTION : packed single DP FP greater equal */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpge_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (_MM_DP0(a) >= _MM_DP0(b)) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (_MM_DP1(a) >= _MM_DP1(b)) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpneq_sd                             */
/*  DESCRIPTION : scalar single DP FP not equal to  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpneq_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (!(_MM_DP0(a) == _MM_DP0(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpneq_pd                             */
/*  DESCRIPTION : packed single DP FP not equal to  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpneq_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (!(_MM_DP0(a) == _MM_DP0(b))) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (!(_MM_DP1(a) == _MM_DP1(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnlt_sd                             */
/*  DESCRIPTION : scalar single DP FP not less than */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnlt_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (!(_MM_DP0(a) < _MM_DP0(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpnlt_pd                             */
/*  DESCRIPTION : packed single DP FP not less than */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnlt_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (!(_MM_DP0(a) < _MM_DP0(b))) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (!(_MM_DP1(a) < _MM_DP1(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnle_sd                             */
/*  DESCRIPTION : scalar single DP FP not less than */
/*                or equal to                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnle_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (!(_MM_DP0(a) <= _MM_DP0(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnle_pd                             */
/*  DESCRIPTION : packed single DP FP not less than */
/*                or equal to                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnle_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (!(_MM_DP0(a) <= _MM_DP0(b))) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (!(_MM_DP1(a) <= _MM_DP1(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpngt_sd                             */
/*  DESCRIPTION : scalar single DP FP not greater   */
/*                than                              */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpngt_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (!(_MM_DP0(a) > _MM_DP0(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpngt_pd                             */
/*  DESCRIPTION : packed single DP FP not greater   */
/*                than                              */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpngt_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (!(_MM_DP0(a) > _MM_DP0(b))) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (!(_MM_DP1(a) > _MM_DP1(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnge_sd                             */
/*  DESCRIPTION : scalar single DP FP not greater   */
/*                than or equal to                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnge_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP1(ret) = _MM_DP1(a);
  _MM_INT640(ret) = (!(_MM_DP0(a) >= _MM_DP0(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnge_pd                             */
/*  DESCRIPTION : packed single DP FP not greater   */
/*                than or equal to                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpnge_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_INT640(ret) = (!(_MM_DP0(a) >= _MM_DP0(b))) ? _MM_BITONE64 : 0x0;
  _MM_INT641(ret) = (!(_MM_DP1(a) >= _MM_DP1(b))) ? _MM_BITONE64 : 0x0;

  return ret;
}

/*
 * cmp_ord and cmp_unord intrinsics 
 */

/****************************************************/
/*  NAME : mm_cmpord_pd                             */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpord_pd(__m128d a, __m128d b)
{

  __m128d ret;

  dbits a0, a1, b0, b1;
  a0.d64 = a.d[0];
  a1.d64 = a.d[1];
  b0.d64 = b.d[0];
  b1.d64 = b.d[1];

/* checking to see neither are QNaN / SNaN */
  if ((a0.bits.exp == 0x7ff) && ((a0.bits.manhi != 0) || (a0.bits.manlo != 0)))
	  _MM_INT640(ret) = 0x0;
  else
  {
	  if ((b0.bits.exp == 0x7ff) && ((b0.bits.manhi != 0) || (b0.bits.manlo != 0)))
	  _MM_INT640(ret) = 0x0;
	  else
	  _MM_INT640(ret) = _MM_BITONE64;
  }

  if ((a1.bits.exp == 0x7ff) && ((a1.bits.manhi != 0) || (a1.bits.manlo != 0)))
	  _MM_INT641(ret) = 0x0;
  else
  {
	  if ((b1.bits.exp == 0x7ff) && ((b1.bits.manhi != 0) || (b1.bits.manlo != 0)))
	  _MM_INT641(ret) = 0x0;
	  else
	  _MM_INT641(ret) = _MM_BITONE64;

  }
 return ret;
}

/****************************************************/
/*  NAME : mm_cmpord_sd                             */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpord_sd(__m128d a, __m128d b)
{
	  __m128d ret;

  dbits a0, a1, b0, b1;
  a0.d64 = a.d[0];
  b0.d64 = b.d[0];

/* checking to see if either are QNaN / SNaN */
  if ((a0.bits.exp == 0x7ff) && ((a0.bits.manhi != 0) || (a0.bits.manlo != 0)))
	  _MM_INT640(ret) = 0x0;
  else
  {
	  if ((b0.bits.exp == 0x7ff) && ((b0.bits.manhi != 0) || (b0.bits.manlo != 0)))
	  _MM_INT640(ret) = 0x0;
	  else
	  _MM_INT640(ret) = _MM_BITONE64;
  }

  ret.d[1] = a.d[1];

 return ret;
}

/****************************************************/
/*  NAME : mm_cmpunord_pd                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpunord_pd(__m128d a, __m128d b)
{
  __m128d ret;

  dbits a0, a1, b0, b1;
  a0.d64 = a.d[0];
  a1.d64 = a.d[1];
  b0.d64 = b.d[0];
  b1.d64 = b.d[1];

/* checking to see if either are QNaN / SNaN */
  if ((a0.bits.exp == 0x7ff) && ((a0.bits.manhi != 0) || (a0.bits.manlo != 0)))
	  _MM_INT640(ret) = _MM_BITONE64;
  else
  {
	  if ((b0.bits.exp == 0x7ff) && ((b0.bits.manhi != 0) || (b0.bits.manlo != 0)))
	  _MM_INT640(ret) = _MM_BITONE64;
	  else
	  _MM_INT640(ret) = 0x0;
  }

  if ((a1.bits.exp == 0x7ff) && ((a1.bits.manhi != 0) || (a1.bits.manlo != 0)))
	  _MM_INT641(ret) = _MM_BITONE64;
  else
  {
	  if ((b1.bits.exp == 0x7ff) && ((b1.bits.manhi != 0) || (b1.bits.manlo != 0)))
	  _MM_INT641(ret) = _MM_BITONE64;
	  else
	  _MM_INT641(ret) = 0x0;

  }
 return ret;
}

/****************************************************/
/*  NAME : mm_cmpunord_sd                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cmpunord_sd(__m128d a, __m128d b)
{
  __m128d ret;

  dbits a0, a1, b0, b1;
  a0.d64 = a.d[0];
  b0.d64 = b.d[0];

/* checking to see if either are QNaN / SNaN */
  if ((a0.bits.exp == 0x7ff) && ((a0.bits.manhi != 0) || (a0.bits.manlo != 0)))
	  _MM_INT640(ret) = _MM_BITONE64;
  else
  {
	  if ((b0.bits.exp == 0x7ff) && ((b0.bits.manhi != 0) || (b0.bits.manlo != 0)))
	  _MM_INT640(ret) = _MM_BITONE64;
	  else
	  _MM_INT640(ret) = 0x0;
  }

  ret.d[1] = a.d[1];

 return ret;
}

/****************************************************/
/*  NAME : mm_comieq_sd                             */
/*  DESCRIPTION : scalar double FP equal to         */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comieq_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) == _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comilt_sd                             */
/*  DESCRIPTION : scalar double FP less than        */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comilt_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) < _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comile_sd                             */
/*  DESCRIPTION : scalar double FP less than or     */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comile_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) <= _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comigt_sd                             */
/*  DESCRIPTION : scalar double FP greater than     */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comigt_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) > _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comige_sd                             */
/*  DESCRIPTION : scalar double FP greater than or  */
/*                equal to                          */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comige_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) >= _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comineq_sd                            */
/*  DESCRIPTION : scalar double FP not equal to     */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_comineq_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) != _MM_DP0(b)) ? 1 : 0;

  return ret;
}


/****************************************************/
/*  NAME : mm_ucomieq_sd                            */
/*  DESCRIPTION : scalar double FP equal to         */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomieq_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) == _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomilt_sd                            */
/*  DESCRIPTION : scalar double FP less than        */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomilt_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) < _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomile_sd                            */
/*  DESCRIPTION : scalar double FP less than or     */
/*                equal to                          */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomile_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) <= _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomigt_sd                            */
/*  DESCRIPTION : scalar double FP greater than     */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomigt_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) > _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomige_sd                            */
/*  DESCRIPTION : scalar double FP greater than or  */
/*                equal to                          */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomige_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) >= _MM_DP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomineq_sd                           */
/*  DESCRIPTION : scalar double FP not equal to     */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_ucomineq_sd(__m128d a, __m128d b)
{
  int ret;

  ret = (_MM_DP0(a) != _MM_DP0(b)) ? 1 : 0;

  return ret;
}


/* Conversion Operations */

/****************************************************/
/*  NAME : mm_cvtpd_ps                              */
/*  DESCRIPTION : packed DP FP to packed FP         */
/*                conversion.                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 _mm_cvtpd_ps(__m128d a)
{
  __m128s ret;

  _MM_FPS0(ret) = _emm_double2float( _MM_DP0(a) );
  _MM_FPS1(ret) = _emm_double2float( _MM_DP1(a) );
  _MM_FPS2(ret) = 0.0f;
  _MM_FPS3(ret) = 0.0f;

  return ret.m128s.m;
}

/****************************************************/
/*  NAME : mm_cvtps_pd                              */
/*  DESCRIPTION : packed FP to packed DP FP         */
/*                conversion.                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cvtps_pd(__m128 a)
{
  __m128d ret;
  __m128s s128;
  s128.m128s.m = a;
  _MM_DP0(ret) = _emm_float2double(_MM_FPS0(s128));
  _MM_DP1(ret) = _emm_float2double(_MM_FPS1(s128));

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtepi32_pd                           */
/*  DESCRIPTION : lower 2 signed 32-bit INT32       */
/*                to 2 DP FP conversion             */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cvtepi32_pd(__m128i a)
{
  __m128d ret;

  _MM_DP0(ret) = (double)a.m128i.i[0];
  _MM_DP1(ret) = (double)a.m128i.i[1];

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtpd_epi32                           */
/*  DESCRIPTION : lower 2 DP FP values to 2         */
/*                32-bit signed integer conversions */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cvtpd_epi32(__m128d a)
{
  __m128i ret;

  ret.m128i.i[0] = _emm_round_nearest(_MM_DP0(a));
  ret.m128i.i[1] = _emm_round_nearest(_MM_DP1(a));
  ret.m128i.i[2] = 0x0;
  ret.m128i.i[3] = 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtsd_si32                            */
/*  DESCRIPTION : scalar single DP to signed INT32  */
/*                conversion                        */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_cvtsd_si32(__m128d a)
{
  int ret;

  ret = _emm_round_nearest(_MM_DP0(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtsd_ss                              */
/*  DESCRIPTION : scalar single DP to a scalar      */
/*                single SP FP.                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 _mm_cvtsd_ss(__m128 a, __m128d b)
{
  __m128s ret;
  __m128s as;
  as.m128s.m = a;

  _MM_FPS0(ret) = _emm_double2float(_MM_DP0(b));
  _MM_FPS1(ret) = _MM_FPS1(as);
  _MM_FPS2(ret) = _MM_FPS2(as);
  _MM_FPS3(ret) = _MM_FPS3(as);
 
  return ret.m128s.m;
}

/****************************************************/
/*  NAME : mm_cvtsi32_sd                            */
/*  DESCRIPTION : scalar single DP to a scalar      */
/*                single SP FP.                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cvtsi32_sd(__m128d a, int b)
{
  __m128d ret;

  _MM_DP0(ret) = (double) b;
  _MM_DP1(ret) = _MM_DP1(a);
 
  return ret;
}

/****************************************************/
/*  NAME : mm_cvtss_sd                              */
/*  DESCRIPTION : scalar single DP to a scalar      */
/*                single SP FP.                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_cvtss_sd(__m128d a, __m128 b)
{
  __m128d ret;
  __m128s s128;

  s128.m128s.m = b;

  _MM_DP0(ret) = _emm_float2double(_MM_FPS0(s128));
  _MM_DP1(ret) = _MM_DP1(a);
 
  return ret;
}

/****************************************************/
/*  NAME : mm_cvttpd_epi32                          */
/*  DESCRIPTION : scalar DP FP to a signed          */
/*                INT32 (truncate).                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cvttpd_epi32(__m128d a)
{
  __m128i ret;

  ret.m128i.i[0] = _emm_round_trunc(_MM_DP0(a));
  ret.m128i.i[1] = _emm_round_trunc(_MM_DP1(a));
  ret.m128i.i[2] = 0x0;
  ret.m128i.i[3] = 0x0;
 
  return ret;
}

/****************************************************/
/*  NAME : mm_cvttsd_si32                           */
/*  DESCRIPTION : scalar single DP to signed INT32  */
/*                conversion (truncated).           */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_cvttsd_si32(__m128d a)
{
  int ret;

  ret = _emm_round_trunc(_MM_DP0(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtepi32_ps                           */
/*  DESCRIPTION : packed signed INT32 conversion    */
/*                to packed SP FP                   */
/****************************************************/
_MM_INLINE_COMMAND static __m128 _mm_cvtepi32_ps(__m128i a)
{
  __m128s ret;

  _MM_FPS0(ret) = (float)(a.m128i.i[0]);
  _MM_FPS1(ret) = (float)(a.m128i.i[1]);  
  _MM_FPS2(ret) = (float)(a.m128i.i[2]);
  _MM_FPS3(ret) = (float)(a.m128i.i[3]);

  return ret.m128s.m;
}

/****************************************************/
/*  NAME : mm_cvtps_epi32                           */
/*  DESCRIPTION : packed single FP to packed signed */
/*                INT32 conversion .                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cvtps_epi32(__m128 a)
{
  __m128i ret;
  __m128s stemp;

  stemp.m128s.m = a;

  ret.m128i.i[0] =  _emm_round_nearest(_MM_FPS0(stemp));
  ret.m128i.i[1] =  _emm_round_nearest(_MM_FPS1(stemp));
  ret.m128i.i[2] =  _emm_round_nearest(_MM_FPS2(stemp));
  ret.m128i.i[3] =  _emm_round_nearest(_MM_FPS3(stemp));

  return ret;
}


/****************************************************/
/*  NAME : mm_cvttps_epi32                          */
/*  DESCRIPTION : packed single FP to packed signed */
/*                INT32 conversion (truncate) .     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cvttps_epi32(__m128 a)
{
  __m128i ret;
  __m128s s128;

  s128.m128s.m = a;

  ret.m128i.i[0] = _emm_round_trunc(_MM_FPS0(s128));
  ret.m128i.i[1] = _emm_round_trunc(_MM_FPS1(s128));
  ret.m128i.i[2] = _emm_round_trunc(_MM_FPS2(s128));
  ret.m128i.i[3] = _emm_round_trunc(_MM_FPS3(s128));

  return ret;
}

/* Miscellaneous DP */

/****************************************************/
/*  NAME : mm_unpackhi_pd                           */
/*  DESCRIPTION : unpack high packed single DP FP   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_unpackhi_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP1(a);
  _MM_DP1(ret) = _MM_DP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_unpacklo_pd                           */
/*  DESCRIPTION : unpack low packed single DP FP    */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_unpacklo_pd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a);
  _MM_DP1(ret) = _MM_DP0(b);

  return ret;
}


/****************************************************/
/*  NAME : mm_movemask_pd                           */
/*  DESCRIPTION : move mask to integer              */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_movemask_pd(__m128d a)
{
  int ret;

  ret = (((*((unsigned int*)&a.d[0]+1)) >> 31) | (((*((unsigned int*)&a.d[1]+1)) >> 30) & 0x2));

  return ret;
}

/****************************************************/
/*  NAME : mm_shuffle_pd                            */
/*  DESCRIPTION : shuffles the DP FP values in a and*/
/*                b according to the constant i     */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_shuffle_pd(__m128d a, __m128d b, int i)
{
  __m128d ret;

  ret.d[0] = (i & 0x1) ? a.d[1] : a.d[0];
  ret.d[1] = (i & 0x2) ? b.d[1] : b.d[0];

  return ret;
}


/* Memory and Initialization */


/* Load Operations */

/****************************************************/
/*  NAME : mm_load_pd                               */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of an xmm reg.   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_load_pd(double *dp)
{
  __m128d ret;

  _mminternal_assert_16B(dp);
  _MM_DP0(ret) = dp[0];
  _MM_DP1(ret) = dp[1];

  return ret;
}

/****************************************************/
/*  NAME : mm_load1_pd                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of an xmm reg.   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_load1_pd(double *dp)
{
  __m128d ret;

  _MM_DP0(ret) = *dp;
  _MM_DP1(ret) = *dp;

  return ret;
}

/****************************************************/
/*  NAME : mm_loadr_pd                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of an xmm reg.   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_loadr_pd(double *dp)
{
  __m128d ret;

  _mminternal_assert_16B(dp);
  _MM_DP0(ret) = dp[1];
  _MM_DP1(ret) = dp[0];

  return ret;
}

/****************************************************/
/*  NAME : mm_loadu_pd                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of an xmm reg.   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_loadu_pd(double *dp)
{
  __m128d ret;

  /* no check for alignment */
  _MM_DP0(ret) = dp[0];
  _MM_DP1(ret) = dp[1];

  return ret;
}

/****************************************************/
/*  NAME : mm_load_sd                               */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of an xmm reg.   */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_load_sd(double *dp)
{
  __m128d ret;

  _MM_DP0(ret) = *dp;
  _MM_DP1(ret) = 0.0;

  return ret;
}

/****************************************************/
/*  NAME : mm_loadh_pd                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper 64 bits of an xmm reg.      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_loadh_pd(__m128d a, double *dp)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(a);
  _MM_DP1(ret) = *dp;

  return ret;
}

/****************************************************/
/*  NAME : mm_loadl_pd                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                lower 64 bits of an xmm reg.      */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_loadl_pd(__m128d a, double *dp)
{
  __m128d ret;

  _MM_DP0(ret) = *dp;
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/* Set Operations */

/****************************************************/
/*  NAME : mm_set_sd                                */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_set_sd(double w)
{
  __m128d ret;

  _MM_DP0(ret) = w;
  _MM_DP1(ret) = 0.0;
  return ret;
}

/****************************************************/
/*  NAME : mm_set1_pd                               */
/*  DESCRIPTION : packed single set 1               */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_set1_pd(double a)
{
  __m128d ret;

  _MM_DP0(ret) = a;
  _MM_DP1(ret) = a;

  return ret;
}

/****************************************************/
/*  NAME : mm_set_pd                                */
/*  DESCRIPTION : packed single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_set_pd(double z, double y)
{
  __m128d ret;

  _MM_DP0(ret) = y;
  _MM_DP1(ret) = z;

  return ret;
}

/****************************************************/
/*  NAME : mm_setr_pd                               */
/*  DESCRIPTION : reversed packed single set        */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_setr_pd(double y, double z)
{
  __m128d ret;

  _MM_DP0(ret) = y;
  _MM_DP1(ret) = z;

  return ret;
}

/****************************************************/
/*  NAME : mm_setzero_pd                            */
/*  DESCRIPTION : set all elements to zero          */
/****************************************************/
_MM_INLINE_COMMAND static __m128d _mm_setzero_pd(void)
{
  __m128d ret;

  _MM_DP0(ret) = 0.0;
  _MM_DP1(ret) = 0.0;

  return ret;
}

/****************************************************/
/*  NAME : mm_move_sd                               */
/*  DESCRIPTION : scalar move                       */
/****************************************************/

_MM_INLINE_COMMAND static __m128d _mm_move_sd(__m128d a, __m128d b)
{
  __m128d ret;

  _MM_DP0(ret) = _MM_DP0(b);
  _MM_DP1(ret) = _MM_DP1(a);

  return ret;
}

/* Store Operations */

/****************************************************/
/*  NAME : mm_store_sd                              */
/*  DESCRIPTION : Move upper 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_store_sd(double *dp, __m128d a)
{
  *dp = _MM_DP0(a);
}

/****************************************************/
/*  NAME : mm_store1_pd                             */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_store1_pd(double *dp, __m128d a)
{

  _mminternal_assert_16B(dp);
  dp[0] = _MM_DP0(a);
  dp[1] = _MM_DP0(a);

}

/****************************************************/
/*  NAME : mm_store_pd                              */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_store_pd(double *dp, __m128d a)
{
  _mminternal_assert_16B(dp);
  dp[0] = _MM_DP0(a);
  dp[1] = _MM_DP1(a);

}

/****************************************************/
/*  NAME : mm_storeu_pd                             */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_storeu_pd(double *dp, __m128d a)
{
  dp[0] = _MM_DP0(a);
  dp[1] = _MM_DP1(a);
}

/****************************************************/
/*  NAME : mm_storer_pd                             */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_storer_pd(double *dp, __m128d a)
{
  _mminternal_assert_16B(dp);
  dp[0] = _MM_DP1(a);
  dp[1] = _MM_DP0(a);
}

/****************************************************/
/*  NAME : mm_storel_pd                             */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_storel_pd(double *dp, __m128d a)
{
  *dp = _MM_DP0(a);
}

/****************************************************/
/*  NAME : mm_storeh_pd                             */
/*  DESCRIPTION : Move upper 64 bits of an xmm reg  */
/*                to memory                         */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_storeh_pd(double *dp, __m128d a)
{
  *dp = _MM_DP1(a);
}

/* Cacheability Support */

/****************************************************/
/*  NAME : mm_stream_pd                             */
/*  DESCRIPTION : Move lower 64 bits of an xmm reg  */
/*                to memory without polluting the   */
/*                cache                             */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_stream_pd(double *dp, __m128d a)
{
  _mminternal_assert_16B(dp);
  dp[0] = _MM_DP0(a);
  dp[1] = _MM_DP1(a);
}

/*
 * Integer intrinsics
 */

/****************************************************/
/*  NAME : _mm_add_epi8                             */
/*  DESCRIPTION : Adds 16 unsigned or signed 8-bit  */
/*                integers in a to b                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_add_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<16; i++)
	ret.m128i.b[i] = a.m128i.b[i] +  b.m128i.b[i];
  return ret;
}

/****************************************************/
/*  NAME : _mm_add_epi16                            */
/*  DESCRIPTION : Adds 8 unsigned or signed 16-bit  */
/*                integers in a to b                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_add_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<8; i++)
	ret.m128i.w[i] = a.m128i.w[i] +  b.m128i.w[i];
  return ret;
}

/****************************************************/
/*  NAME : _mm_add_epi32                            */
/*  DESCRIPTION : Adds 4 unsigned or signed 32-bit  */
/*                integers in a to b                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_add_epi32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  ret.m128i.i[0] = a.m128i.i[0] +  b.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1] +  b.m128i.i[1];  
  ret.m128i.i[2] = a.m128i.i[2] +  b.m128i.i[2];
  ret.m128i.i[3] = a.m128i.i[3] +  b.m128i.i[3];
  return ret;
}

/****************************************************/
/*  NAME : _mm_add_si64                             */
/*  DESCRIPTION : Adds unsigned or signed 64-bit    */
/*                integer in a to b                 */
/****************************************************/

_MM_INLINE_COMMAND static __m64 _mm_add_si64(__m64 a, __m64 b)
{
  __um64 ua, ub;
  __um64 ret;
  ua.m = a;
  ub.m = b;

  ret.s64 = ua.s64 + ub.s64;
  return ret.m;
}

/****************************************************/
/*  NAME : _mm_add_epi64                            */
/*  DESCRIPTION : Adds 2 unsigned or signed 64-bit  */
/*                integers in a to b                */
/****************************************************/

_MM_INLINE_COMMAND static __m128i _mm_add_epi64(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  ret.m128i.m[0] = a.m128i.m[0] +  b.m128i.m[0];
  ret.m128i.m[1] = a.m128i.m[1] +  b.m128i.m[1];  
  return ret;
}

/****************************************************/
/*  NAME : _mm_adds_epi8                            */
/*  DESCRIPTION : Adds 16 unsigned or signed 8-bit  */
/*                integers in a to b and saturates  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_adds_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  short stemp;
  int i;

  for (i=0; i<16; i++)
    {
	stemp = ((short)a.m128i.b[i] + (short)b.m128i.b[i]);
        if (stemp > 0x7F) 
			ret.m128i.b[i] = 0x7F;
		else
		{
			if (stemp < (char)0x80) 
				ret.m128i.b[i] = 0x80;
			else 
				ret.m128i.b[i] = stemp;
		}


    }
  return ret;
}

/****************************************************/
/*  NAME : _mm_adds_epi16                           */
/*  DESCRIPTION : Adds 8 unsigned or signed 16-bit  */
/*                integers in a to b and saturates  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_adds_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i,itemp;

  for (i=0; i<8; i++)
    {
        itemp = (int)a.m128i.w[i] +  (int)b.m128i.w[i];
        if (itemp > 0x7FFF) 
	    ret.m128i.w[i] = 0x7FFF;
	else
	  {
	    if (itemp < (short)0x8000) 
	        ret.m128i.w[i] = 0x8000;
	    else
	        ret.m128i.w[i] = itemp;
	  }
    }
  return ret;
}
/****************************************************/
/*  NAME : _mm_adds_epu8                            */
/*  DESCRIPTION : Adds 16 unsigned 8-bit            */
/*                integers in a to b and saturates  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_adds_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  unsigned short ustemp;

  for (i=0; i<16; i++)
    {
        ustemp = (unsigned short)a.m128i.ub[i] +  (unsigned short)b.m128i.ub[i];
        if (ustemp > 0xFF) 
	    ret.m128i.ub[i] = 0xFF;
	else
	    ret.m128i.ub[i] = ustemp;
    }
  return ret;
}

/****************************************************/
/*  NAME : _mm_adds_epu16                           */
/*  DESCRIPTION : Adds 8 unsigned 16-bit            */
/*                integers in a to b and saturates  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_adds_epu16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  unsigned int uitemp;

  for (i=0; i<8; i++)
    {
		uitemp = (unsigned int)a.m128i.uw[i] +  (unsigned int)b.m128i.uw[i];
        if (uitemp > 0xFFFF) 
			ret.m128i.uw[i] = 0xFFFF;
		else
			ret.m128i.uw[i] = uitemp;
    }
 
  return ret;
}

/****************************************************/
/*  NAME : _mm_avg_epu8                             */
/*  DESCRIPTION : Averages 16 unsigned 8-bit        */
/*                integers in a and b and rounds    */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_avg_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  unsigned short ustemp;

  for (i=0; i<16; i++)
  {
    ustemp = a.m128i.ub[i] +  b.m128i.ub[i] + 1;
    ret.m128i.ub[i] = (unsigned char)(ustemp >> 1);
  }
 
  return ret;
}

/****************************************************/
/*  NAME : _mm_avg_epu16                            */
/*  DESCRIPTION : Averages 8 unsigned 16-bit        */
/*                integers in a and b and rounds    */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_avg_epu16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  unsigned int uitemp;

  for (i=0; i<8; i++)
  {
    uitemp = a.m128i.uw[i] +  b.m128i.uw[i] + 1;
    ret.m128i.uw[i] = (unsigned short)(uitemp >> 1);
  }
 
  return ret;
}

/****************************************************/
/*  NAME : _mm_madd_epi16                           */
/*  DESCRIPTION : Multiplies 8 signed 16-bit        */
/*                integers a and b and Adds signed  */
/*                32-bit integer results pairwise   */
/*                and packs                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_madd_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  ret.m128i.i[0] = (a.m128i.w[0] *  b.m128i.w[0]) +
             (a.m128i.w[1] *  b.m128i.w[1]);
  ret.m128i.i[1] = (a.m128i.w[2] *  b.m128i.w[2]) +
             (a.m128i.w[3] *  b.m128i.w[3]);
  ret.m128i.i[2] = (a.m128i.w[4] *  b.m128i.w[4]) +
             (a.m128i.w[5] *  b.m128i.w[5]);
  ret.m128i.i[3] = (a.m128i.w[6] *  b.m128i.w[6]) +
             (a.m128i.w[7] *  b.m128i.w[7]);
   
  return ret;
}

/****************************************************/
/*  NAME : _mm_max_epi16                            */
/*  DESCRIPTION : Finds max of 8 signed 16-bit      */
/*                integers a and b                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_max_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<8; i++)
	ret.m128i.w[i] = _MM_MAX(a.m128i.w[i], b.m128i.w[i]);
  return ret;
}

/****************************************************/
/*  NAME : _mm_max_epu8                             */
/*  DESCRIPTION : Finds max of 16 unsigned 8-bit    */
/*                integers a and b                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_max_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<16; i++)
	ret.m128i.b[i] = _MM_MAX(a.m128i.ub[i], b.m128i.ub[i]);
  return ret;
}

/****************************************************/
/*  NAME : _mm_min_epi16                            */
/*  DESCRIPTION : Finds min of 8 signed 16-bit      */
/*                integers a and b                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_min_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<8; i++)
	ret.m128i.w[i] = _MM_MIN(a.m128i.w[i], b.m128i.w[i]);
  return ret;
}

/****************************************************/
/*  NAME : _mm_min_epu8                             */
/*  DESCRIPTION : Finds min of 16 unsigned 8-bit    */
/*                integers a and b                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_min_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<16; i++)
	ret.m128i.b[i] = _MM_MIN(a.m128i.ub[i], b.m128i.ub[i]);
  return ret;
}

/****************************************************/
/*  NAME : _mm_mulhi_epi16                          */
/*  DESCRIPTION : Multiplies 8 signed 16-bit        */
/*                integers a and b.  Packs upper    */
/*                16-bits                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_mulhi_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i, itemp;

  for (i=0; i<8; i++)
  {
		itemp = (int)a.m128i.w[i] * (int)b.m128i.w[i];
		ret.m128i.w[i] = (*((short*)&itemp+1));
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_mulhi_epu16                          */
/*  DESCRIPTION : Multiplies 8 unsigned 16-bit      */
/*                integers a and b.  Packs upper    */
/*                16-bits                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_mulhi_epu16(__m128i a, __m128i b)
{
  __m128i ret;
  int i,itemp;

  for (i=0; i<8; i++) 
  {
	itemp = (int)a.m128i.uw[i] * (int)b.m128i.uw[i];
	ret.m128i.uw[i] = (*((unsigned short*)&itemp+1));
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_mullo_epi16                          */
/*  DESCRIPTION : Multiplies 8 signed or unsigned   */
/*                16-bit integers in a and b. Packs */
/*                lower 16-bits                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_mullo_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i,itemp;

  for (i=0; i<8; i++)
  {
	 itemp = (int)a.m128i.uw[i] * (int)b.m128i.uw[i];
     ret.m128i.uw[i] = (*((short*)(&(itemp))));
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_mul_su32                             */
/*  DESCRIPTION : Multiplies 32-bit integers in     */
/*                a and b. Returns the 64-bit result*/
/****************************************************/
_MM_INLINE_COMMAND static __m64 _mm_mul_su32(__m64 a, __m64 b)
{
  __um64 ua, ub, ret;

  ua.m = a;
  ub.m = b;

  ret.s64 = (__int64)ua.ui[0] * (__int64)ub.ui[0];
  return ret.m;
}

/****************************************************/
/*  NAME : _mm_mul_epu32                            */
/*  DESCRIPTION : Multiplies 2 32-bit integers in   */
/*                a and b. Packs 2 64-bit results   */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_mul_epu32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  __int64 itemp = (__int64)a.m128i.ui[0] * (__int64)b.m128i.ui[0];
  ret.m128i.m[0] = itemp;
  itemp = (__int64)a.m128i.ui[2] * (__int64)b.m128i.ui[2];
  ret.m128i.m[1] = itemp;
  return ret;
}

/****************************************************/
/*  NAME : _mm_sad_epu8                             */
/*  DESCRIPTION : Computes abs of 16 8-bit integers */
/*                in a and b. Sums & Packs          */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sad_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i,temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7;


    temp0 = abs(a.m128i.ub[0] - b.m128i.ub[0]);
    temp1 = abs(a.m128i.ub[1] - b.m128i.ub[1]);
    temp2 = abs(a.m128i.ub[2] - b.m128i.ub[2]);
    temp3 = abs(a.m128i.ub[3] - b.m128i.ub[3]);
    temp4 = abs(a.m128i.ub[4] - b.m128i.ub[4]);
    temp5 = abs(a.m128i.ub[5] - b.m128i.ub[5]);
    temp6 = abs(a.m128i.ub[6] - b.m128i.ub[6]);
    temp7 = abs(a.m128i.ub[7] - b.m128i.ub[7]);

	ret.m128i.w[0] = temp0 + temp1 + temp2 + temp3 + temp4 + temp5 + temp6 + temp7;
    temp0 = abs(a.m128i.ub[8] - b.m128i.ub[8]);
    temp1 = abs(a.m128i.ub[9] - b.m128i.ub[9]);
    temp2 = abs(a.m128i.ub[10] - b.m128i.ub[10]);
    temp3 = abs(a.m128i.ub[11] - b.m128i.ub[11]);
    temp4 = abs(a.m128i.ub[12] - b.m128i.ub[12]);
    temp5 = abs(a.m128i.ub[13] - b.m128i.ub[13]);
    temp6 = abs(a.m128i.ub[14] - b.m128i.ub[14]);
    temp7 = abs(a.m128i.ub[15] - b.m128i.ub[15]);
    ret.m128i.w[4] = temp0 + temp1 + temp2 + temp3 + temp4 + temp5 + temp6 + temp7;

  ret.m128i.w[1] = 0x0;
  ret.m128i.w[2] = 0x0;
  ret.m128i.w[3] = 0x0;
  ret.m128i.w[5] = 0x0;
  ret.m128i.w[6] = 0x0;
  ret.m128i.w[7] = 0x0;

  return ret;
}

/****************************************************/
/*  NAME : _mm_sub_epi8                             */
/*  DESCRIPTION : Subtracts 16 unsigned or signed   */
/*                8-bit integers in b from a        */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sub_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<16; i++)
	ret.m128i.b[i] = (a.m128i.b[i] - b.m128i.b[i]);

  return ret;
}

/****************************************************/
/*  NAME : _mm_sub_epi16                            */
/*  DESCRIPTION : Subtracts 8 unsigned or signed    */
/*                16-bit integers in b from a       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sub_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0; i<8; i++)
	ret.m128i.w[i] = (a.m128i.w[i] - b.m128i.w[i]);

  return ret;
}

/****************************************************/
/*  NAME : _mm_sub_epi32                            */
/*  DESCRIPTION : Subtracts 4 unsigned or signed    */
/*                32-bit integers in b from a       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sub_epi32(__m128i a, __m128i b)
{
  __m128i ret;

  ret.m128i.i[0] = a.m128i.i[0] - b.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1] - b.m128i.i[1];
  ret.m128i.i[2] = a.m128i.i[2] - b.m128i.i[2];
  ret.m128i.i[3] = a.m128i.i[3] - b.m128i.i[3];

  return ret;
}

/****************************************************/
/*  NAME : _mm_sub_si64                             */
/*  DESCRIPTION : Subtracts unsigned or signed      */
/*                64-bit integer in b from a        */
/****************************************************/

_MM_INLINE_COMMAND static __m64 _mm_sub_si64(__m64 a, __m64 b)
{
  __um64 ua, ub;
  __um64 ret;
  ua.m = a;
  ub.m = b;

  ret.s64 = ua.s64 - ub.s64;
  return ret.m;
}

/******************************************************/
/*  NAME : _mm_sub_epi64                              */
/*  DESCRIPTION : Subtracts 2 unsigned or signed      */
/*                64-bit integers in b from a         */
/******************************************************/

_MM_INLINE_COMMAND static __m128i _mm_sub_epi64(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  ret.m128i.m[0] = a.m128i.m[0] -  b.m128i.m[0];
  ret.m128i.m[1] = a.m128i.m[1] -  b.m128i.m[1];  
  return ret;
}

/****************************************************/
/*  NAME : _mm_subs_epi8                            */
/*  DESCRIPTION : Subtracts 16 unsigned or signed   */
/*                8-bit integers in b from a and    */
/*                saturates                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_subs_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  short stemp;

  for (i=0;i<16;i++) 
  {
    stemp = (short)a.m128i.b[i] - (short)b.m128i.b[i];
    if (stemp > 0x7F) 
      ret.m128i.b[i] = 0x7F;
    else
    {
      if (stemp < (short)0xFF80) 
	ret.m128i.b[i] = 0x80;
      else
	ret.m128i.b[i] = stemp;
    }
  }
  
  return ret;
}

/****************************************************/
/*  NAME : _mm_subs_epi16                           */
/*  DESCRIPTION : Subtracts 8 unsigned or signed    */
/*                16-bit integers in b from a and   */
/*                saturates                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_subs_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i, itemp;

  for (i=0;i<8;i++)
  {
    itemp = (int)a.m128i.w[i] - (int)b.m128i.w[i];
    if (itemp > 0x7FFF) 
      ret.m128i.w[i] = 0x7FFF;
    else
    {
      if (itemp < (int)0xFFFF8000) 
	ret.m128i.w[i] = 0x8000;
      else
	ret.m128i.w[i] = itemp;
    }
  }
  
  return ret;
}

/****************************************************/
/*  NAME : _mm_subs_epu8                            */
/*  DESCRIPTION : Subtracts 16 unsigned             */
/*                8-bit integers in b from a and    */
/*                saturates                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_subs_epu8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  short temp;

  for (i=0;i<16;i++)
  {
    temp = (unsigned short)a.m128i.ub[i] - (unsigned short)b.m128i.ub[i];
    if (temp < 0) 
      ret.m128i.ub[i] = 0x0;
    else
      ret.m128i.ub[i] = temp;
  }
  
  return ret;
}

/****************************************************/
/*  NAME : _mm_subs_epu16                           */
/*  DESCRIPTION : Subtracts 8 unsigned              */
/*                16-bit integers in b from a and   */
/*                saturates                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_subs_epu16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;
  int temp;

  for (i=0;i<8;i++)
  {
    temp = (unsigned int)a.m128i.uw[i] - (unsigned int)b.m128i.uw[i];
    if (temp < 0x0) 
      ret.m128i.uw[i] = 0x0;
    else
      ret.m128i.uw[i] = temp;
  }
  
  return ret;
}

/* Logicals */

/****************************************************/
/*  NAME : _mm_and_si128                            */
/*  DESCRIPTION : Computes bitwise AND of 128-bit   */
/*                value in a and in b               */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_and_si128(__m128i a, __m128i b)
{
  __m128i ret;

  ret.m128i.i[0] = a.m128i.i[0] & b.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1] & b.m128i.i[1];
  ret.m128i.i[2] = a.m128i.i[2] & b.m128i.i[2];
  ret.m128i.i[3] = a.m128i.i[3] & b.m128i.i[3];

  return ret;
}

/****************************************************/
/*  NAME : _mm_andnot_si128                         */
/*  DESCRIPTION : Computes bitwise ANDNOT of 128-bit*/
/*                value in a and in b               */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_andnot_si128(__m128i a, __m128i b)
{
  __m128i ret;

  ret.m128i.i[0] = (~a.m128i.i[0]) & b.m128i.i[0];
  ret.m128i.i[1] = (~a.m128i.i[1]) & b.m128i.i[1];
  ret.m128i.i[2] = (~a.m128i.i[2]) & b.m128i.i[2];
  ret.m128i.i[3] = (~a.m128i.i[3]) & b.m128i.i[3];

  return ret;
}

/****************************************************/
/*  NAME : _mm_or_si128                             */
/*  DESCRIPTION : Computes bitwise IR of 128-bit    */
/*                value in a and in b               */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_or_si128(__m128i a, __m128i b)
{
  __m128i ret;

  ret.m128i.i[0] = a.m128i.i[0] | b.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1] | b.m128i.i[1];
  ret.m128i.i[2] = a.m128i.i[2] | b.m128i.i[2];
  ret.m128i.i[3] = a.m128i.i[3] | b.m128i.i[3];

  return ret;
}

/****************************************************/
/*  NAME : _mm_xor_si128                            */
/*  DESCRIPTION : Computes bitwise XOR of 128-bit   */
/*                value in a and in b               */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_xor_si128(__m128i a, __m128i b)
{
  __m128i ret;

  ret.m128i.i[0] = a.m128i.i[0] ^ b.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1] ^ b.m128i.i[1];
  ret.m128i.i[2] = a.m128i.i[2] ^ b.m128i.i[2];
  ret.m128i.i[3] = a.m128i.i[3] ^ b.m128i.i[3];

  return ret;
}

/* Shifts */

/****************************************************/
/*  NAME : _mm_slli_si128                           */
/*  DESCRIPTION : Shifts 128-bit value in a left by */
/*                count bytes while shifting in 0s. */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_slli_si128(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 16)
	  count = 16;
  else
  {

	  for (i=15;i >= count;i--)
		{
		ret.m128i.b[i] = a.m128i.b[i-count];
		}
  }

  for (i=0;i<count;i++)
    ret.m128i.b[i] = 0x00;

  return ret;
}

/****************************************************/
/*  NAME : _mm_slli_epi16                           */
/*  DESCRIPTION : Shifts 8 signed or unsigned 16-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_slli_epi16(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 15) {
	  ret.m128i.i[0]=0;
	  ret.m128i.i[1]=0;
	  ret.m128i.i[2]=0;
	  ret.m128i.i[3]=0;

  }
  else
  {
	for (i=0;i<8;i++)
		ret.m128i.w[i] = a.m128i.w[i] << count;
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_sll_epi16                            */
/*  DESCRIPTION : Shifts 8 signed or unsigned 16-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sll_epi16(__m128i a, __m128i count)
{
  __m128i ret;
  int i;
  /* upper 64 bits ignored */

  for (i=0;i<8;i++)
  {
	if ((count.m128i.i[1] != 0) |
		(count.m128i.w[1] != 0) |
	        (count.m128i.uw[0] > 16))
		ret.m128i.w[i] = 0x0;
	else
		  ret.m128i.w[i] = a.m128i.w[i] << count.m128i.w[0];
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_slli_epi32                           */
/*  DESCRIPTION : Shifts 4 signed or unsigned 32-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_slli_epi32(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 31){
	ret.m128i.i[0]=0;
	ret.m128i.i[1]=0;
	ret.m128i.i[2]=0;
	ret.m128i.i[3]=0;
  }
  else
  {
	for (i=0;i<4;i++)
		ret.m128i.i[i] = a.m128i.i[i] << count;
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_sll_epi32                            */
/*  DESCRIPTION : Shifts 4 signed or unsigned 32-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sll_epi32(__m128i a, __m128i count)
{
  __m128i ret;
  int i;

  for (i=0;i<4;i++)
  {
	if ((count.m128i.b[3] != 0) |
		(count.m128i.i[1] != 0) |
	        (count.m128i.i[0] > 31))
		ret.m128i.i[i] = 0x00000000;
	else
		  ret.m128i.i[i] = a.m128i.i[i] << count.m128i.i[0];
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_slli_epi64                           */
/*  DESCRIPTION : Shifts 2 signed or unsigned 64-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_slli_epi64(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 63){
	ret.m128i.i[0]=0;
	ret.m128i.i[1]=0;
	ret.m128i.i[2]=0;
	ret.m128i.i[3]=0;
  }
  else
  {
	for (i=0;i<2;i++)
		ret.m128i.um[i] = a.m128i.um[i] << count;
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_sll_epi64                            */
/*  DESCRIPTION : Shifts 2 signed or unsigned 64-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sll_epi64(__m128i a, __m128i count)
{
  __m128i ret;
  int i;

  for (i=0;i<2;i++)
  {
	if ((count.m128i.b[3] != 0) |
		(count.m128i.i[1] != 0) |
	        (count.m128i.i[0] > 63))
		ret.m128i.um[i] = 0x00000000;
	else
		  ret.m128i.um[i] = a.m128i.um[i] << count.m128i.i[0];
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_srai_epi16                           */
/*  DESCRIPTION : Shifts 8 signed 16-bit            */
/*                integers in a right by count bits */
/*                while shifting in sign bit.       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srai_epi16(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 15) {
	  ret.m128i.w[0] = a.m128i.w[0] >> 15;
	  ret.m128i.w[1] = a.m128i.w[1] >> 15;
	  ret.m128i.w[2] = a.m128i.w[2] >> 15;
	  ret.m128i.w[3] = a.m128i.w[3] >> 15;
	  ret.m128i.w[4] = a.m128i.w[4] >> 15;
	  ret.m128i.w[5] = a.m128i.w[5] >> 15;
	  ret.m128i.w[6] = a.m128i.w[6] >> 15;
	  ret.m128i.w[7] = a.m128i.w[7] >> 15;
  }
  else
  {
  for (i=0;i<8;i++)
    ret.m128i.w[i] = a.m128i.w[i] >> count;
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_sra_epi16                            */
/*  DESCRIPTION : Shifts 8 signed 16-bit            */
/*                integers in a right by count bits */
/*                while shifting in sign bit.       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sra_epi16(__m128i a, __m128i count)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
  {
	if ((count.m128i.i[1] != 0) |
		(count.m128i.w[1] != 0) |
	        (count.m128i.w[0] > 16))
		ret.m128i.w[i] = a.m128i.w[i] >> 16;
	else
		  ret.m128i.w[i] = a.m128i.w[i] >> count.m128i.w[0];
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_srai_epi32                           */
/*  DESCRIPTION : Shifts 4 signed 32-bit            */
/*                integers in a right by count bits */
/*                while shifting in sign bit.       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srai_epi32(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 31) {
	  ret.m128i.i[0] = a.m128i.i[0] >> 31;
	  ret.m128i.i[1] = a.m128i.i[1] >> 31;
	  ret.m128i.i[2] = a.m128i.i[2] >> 31;
	  ret.m128i.i[3] = a.m128i.i[3] >> 31;
  }
  else
  {  
	  for (i=0;i<4;i++)
		  ret.m128i.i[i] = a.m128i.i[i] >> count;
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_sra_epi32                            */
/*  DESCRIPTION : Shifts 4 signed 32-bit            */
/*                integers in a right by count bits */
/*                while shifting in sign bit.       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_sra_epi32(__m128i a, __m128i count)
{
  __m128i ret;
  int i;

  for (i=0;i<4;i++)
  {
	if ((count.m128i.b[3] != 0) |
		(count.m128i.i[1] != 0) |
	        (count.m128i.i[0] > 31))
		ret.m128i.i[i] = a.m128i.i[i] >> 31;
	else
		  ret.m128i.i[i] = a.m128i.i[i] >> count.m128i.i[0];
  }


  return ret;
}

/****************************************************/
/*  NAME : _mm_srli_si128                           */
/*  DESCRIPTION : Shifts 128-bit value in a by      */
/*                count bytes while shifting in 0s. */ 
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srli_si128(__m128i a, int count)
{
  __m128i ret;
  int i;

  for (i=0;i <= 15-count;i++)
    {
      ret.m128i.b[i] = a.m128i.b[i+count];
    }

  for (i=15;i>15-count;i--)
    ret.m128i.b[i] = 0x00;

  return ret;
}

/****************************************************/
/*  NAME : _mm_srli_epi16                           */
/*  DESCRIPTION : Shifts 8 signed or unsigned 16-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srli_epi16(__m128i a, int count)
{
  __m128i ret;
  int i;
  if (count > 15) {
	  ret.m128i.uw[0] = 0;
	  ret.m128i.uw[1] = 0;
	  ret.m128i.uw[2] = 0;
	  ret.m128i.uw[3] = 0;
	  ret.m128i.uw[4] = 0;
	  ret.m128i.uw[5] = 0;
	  ret.m128i.uw[6] = 0;
	  ret.m128i.uw[7] = 0;
  }
  else
  {
	for (i=0;i<8;i++)
		{
		ret.m128i.uw[i] = a.m128i.uw[i] >> count;
		}
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_srl_epi16                            */
/*  DESCRIPTION : Shifts 8 signed or unsigned 16-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srl_epi16(__m128i a, __m128i count)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
  {
	if ((count.m128i.ui[1] != 0) |
		(count.m128i.uw[1] != 0) |
	        (count.m128i.uw[0] > 16))
		ret.m128i.uw[i] = 0x0;
	else
		  ret.m128i.uw[i] = a.m128i.uw[i] >> count.m128i.uw[0];
  }

  
  return ret;
}

/****************************************************/
/*  NAME : _mm_srli_epi32                           */
/*  DESCRIPTION : Shifts 4 signed or unsigned 32-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srli_epi32(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 31) {
	  ret.m128i.ui[0] = 0;
	  ret.m128i.ui[1] = 0;
	  ret.m128i.ui[2] = 0;
	  ret.m128i.ui[3] = 0;
  }
  else
  {
	  for (i=0;i<4;i++)
		{
		ret.m128i.ui[i] = a.m128i.ui[i] >> count;
		}
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_srl_epi32                            */
/*  DESCRIPTION : Shifts 4 signed or unsigned 32-bit*/
/*                integers in a left by count bits  */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srl_epi32(__m128i a, __m128i count)
{
  __m128i ret;
  int i;
  for (i=0;i<4;i++)
  {
	if ((count.m128i.ub[3] != 0) |
		(count.m128i.ui[1] != 0) |
	        (count.m128i.ui[0] > 31))
		ret.m128i.ui[i] = 0x00000000;
	else
		  ret.m128i.ui[i] = a.m128i.ui[i] >> count.m128i.ui[0];
  }
  return ret;
}

/****************************************************/
/*  NAME : _mm_srli_epi64                           */
/*  DESCRIPTION : Shifts 2 signed or unsigned 64-bit*/
/*                integers in a right by count bits */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srli_epi64(__m128i a, int count)
{
  __m128i ret;
  int i;

  if (count > 63) {
	  ret.m128i.ui[0] = 0;
	  ret.m128i.ui[1] = 0;
	  ret.m128i.ui[2] = 0;
	  ret.m128i.ui[3] = 0;
  }
  else
  {
	  for (i=0;i<2;i++)
		{
		ret.m128i.um[i] = a.m128i.um[i] >> count;
		}
  }

  return ret;
}

/****************************************************/
/*  NAME : _mm_srl_epi64                            */
/*  DESCRIPTION : Shifts 2 signed or unsigned 64-bit*/
/*                integers in a right by count bits */
/*                while shifting in 0s.             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_srl_epi64(__m128i a, __m128i count)
{
  __m128i ret;
  int i;
  for (i=0;i<2;i++)
  {
	if ((count.m128i.ub[3] != 0) |
		(count.m128i.ui[1] != 0) |
	        (count.m128i.ui[0] > 63))
		ret.m128i.um[i] = 0x00000000;
	else
		  ret.m128i.um[i] = a.m128i.um[i] >> count.m128i.ui[0];
  }
  return ret;
}

/* Comparisons */

/****************************************************/
/*  NAME : mm_cmpeq_epi8                            */
/*  DESCRIPTION : scalar INT8 equal                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpeq_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<16;i++)
    ret.m128i.b[i] = (a.m128i.b[i] == b.m128i.b[i]) ? 0xff: 0x00;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpeq_epi16                           */
/*  DESCRIPTION : scalar INT16 equal                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpeq_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
    ret.m128i.w[i] = (a.m128i.w[i] == b.m128i.w[i]) ? 0xffff: 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpeq_epi32                           */
/*  DESCRIPTION : scalar INT32 equal                */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpeq_epi32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  ret.m128i.i[0] = (a.m128i.i[0] == b.m128i.i[0]) ? 0xffffffff: 0x00000000;
  ret.m128i.i[1] = (a.m128i.i[1] == b.m128i.i[1]) ? 0xffffffff: 0x00000000;
  ret.m128i.i[2] = (a.m128i.i[2] == b.m128i.i[2]) ? 0xffffffff: 0x00000000;
  ret.m128i.i[3] = (a.m128i.i[3] == b.m128i.i[3]) ? 0xffffffff: 0x00000000;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_epi8                            */
/*  DESCRIPTION : scalar INT8 greater than          */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpgt_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<16;i++)
    ret.m128i.b[i] = (a.m128i.b[i] > b.m128i.b[i]) ? 0xff: 0x00;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_epi16                           */
/*  DESCRIPTION : scalar INT16 greater than         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpgt_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
    ret.m128i.w[i] = (a.m128i.w[i] > b.m128i.w[i]) ? 0xffff: 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_epi32                           */
/*  DESCRIPTION : scalar INT32 greater than         */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmpgt_epi32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<4;i++)
    ret.m128i.i[i] = (a.m128i.i[i] > b.m128i.i[i]) ? 0xffffffff: 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmplt_epi8                            */
/*  DESCRIPTION : scalar INT8 less than             */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmplt_epi8(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<16;i++)
    ret.m128i.b[i] = (a.m128i.b[i] < b.m128i.b[i]) ? 0xff: 0x00;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmplt_epi16                           */
/*  DESCRIPTION : scalar INT16 less than            */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmplt_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
    ret.m128i.w[i] = (a.m128i.w[i] < b.m128i.w[i]) ? 0xffff: 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmplt_epi32                           */
/*  DESCRIPTION : scalar INT32 less than            */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cmplt_epi32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<4;i++)
    ret.m128i.i[i] = (a.m128i.i[i] < b.m128i.i[i]) ? 0xffffffff: 0x0;

  return ret;
}

/* Conversions */

/****************************************************/
/*  NAME : mm_cvtsi32_si128                         */
/*  DESCRIPTION : int to packed __m128i             */
/*                conversion.                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_cvtsi32_si128(int a)
{
  __m128i ret;

  ret.m128i.i[0] = a;
  ret.m128i.i[1] = 0x0;
  ret.m128i.i[2] = 0x0;
  ret.m128i.i[3] = 0x0;

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtsi128_si32                         */
/*  DESCRIPTION : INT32 of __m128i to INT           */
/*                conversion.                       */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_cvtsi128_si32(__m128i a)
{
  int ret;

  ret = a.m128i.i[0];

  return ret;
}

/* Miscellaneous */

/****************************************************/
/*  NAME : _mm_packs_epi16                          */
/*  DESCRIPTION : Packs 16 signed 16-bit integers   */
/*                from a and b into 8 bit integers  */
/*                and saturates                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_packs_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
    {
     if (a.m128i.w[i] > 0x7F)
	 ret.m128i.b[i] = 0x7F;
     else
     {
	 if (a.m128i.w[i] < (char)0x80) 
	   ret.m128i.b[i] = 0x80;
	 else
	   ret.m128i.b[i] = a.m128i.w[i];
     }

     if (b.m128i.w[i] > 0x7F)
	 ret.m128i.b[i+8] = 0x7F;
     else
     {
	 if (b.m128i.w[i] < (char)0x80) 
	   ret.m128i.b[i+8] = 0x80;
	 else
	   ret.m128i.b[i+8] = b.m128i.w[i];
     }
    }

  return ret;
}

/****************************************************/
/*  NAME : _mm_packs_epi32                          */
/*  DESCRIPTION : Packs 8 signed 32-bit integers    */
/*                from a and b into 16 bit integers */
/*                and saturates                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_packs_epi32(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<4;i++)
    {
     if (a.m128i.i[i] > 0x7FFF)
	 ret.m128i.w[i] = 0x7FFF;
     else
       {
	 if (a.m128i.i[i] < (short)0x8000) 
	   ret.m128i.w[i] = 0x8000;
	 else
	   ret.m128i.w[i] = a.m128i.i[i];
       }

     if (b.m128i.i[i] > 0x7FFF)
	 ret.m128i.w[i+4] = 0x7FFF;
     else
       {
	 if (b.m128i.i[i] < (short)0x8000) 
	   ret.m128i.w[i+4] = 0x8000;
	 else
	   ret.m128i.w[i+4] = b.m128i.i[i];
       }
    }

  return ret;
}

/****************************************************/
/*  NAME : _mm_packus_epi16                         */
/*  DESCRIPTION : Packs 16 unsigned 16-bit integers */
/*                from a and b into 8 bit integers  */
/*                and saturates                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_packus_epi16(__m128i a, __m128i b)
{
  __m128i ret;
  int i;

  for (i=0;i<8;i++)
    {
     if (a.m128i.w[i] > 0xFF)
	 ret.m128i.ub[i] = 0xFF;
     else
       {
	 if (a.m128i.w[i] < 0x00) 
	   ret.m128i.ub[i] = 0x00;
	 else
	   ret.m128i.ub[i] = a.m128i.w[i];
       }

     if (b.m128i.w[i] > 0xFF)
	 ret.m128i.ub[i+8] = 0xFF;
     else
       {
	 if (b.m128i.w[i] < 0x00) 
	   ret.m128i.ub[i+8] = 0x00;
	 else
	   ret.m128i.ub[i+8] = b.m128i.w[i];
       }
    }
  return ret;

}

/****************************************************/
/*  NAME : _mm_extract_epi16                        */
/*  DESCRIPTION : Extracts the selected signed or   */
/*                unsigned 16-bit integer from a    */
/*                and 0 extends.                    */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_extract_epi16(__m128i a, int imm)
{
  int ret;

  ret = (int)(a.m128i.w[imm & 0x07]);

  return ret;
}

/****************************************************/
/*  NAME : _mm_insert_epi16                         */
/*  DESCRIPTION : Inserts the least significant 16  */
/*                bits of b into the select 16-bit  */
/*                integer of a.                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_insert_epi16(__m128i a, int b, int imm)
{
  __m128i ret;

  ret = a;
  ret.m128i.w[imm & 0x07] = b;
  return ret;
}

/****************************************************/
/*  NAME : mm_movemask_epi8                         */
/*  DESCRIPTION : move mask to integer              */
/****************************************************/
_MM_INLINE_COMMAND static int _mm_movemask_epi8(__m128i a)
{
  int ret;


  ret = ((((a.m128i.b[0])>>7)&0x1) | (((a.m128i.b[1])>>6)&0x2) |
         (((a.m128i.b[2])>>5)&0x4) | (((a.m128i.b[3])>>4)&0x8) |
         (((a.m128i.b[4])>>3)&0x10) | (((a.m128i.b[5])>>2)&0x20) |
         (((a.m128i.b[6])>>1)&0x40) | (((a.m128i.b[7])>>4)&0x80) |
         (((a.m128i.b[8])<<1)&0x100) | (((a.m128i.b[9])<<2)&0x200) |
         (((a.m128i.b[10])<<3)&0x400) | (((a.m128i.b[11])<<4)&0x800) |
         (((a.m128i.b[12])<<5)&0x1000) | (((a.m128i.b[13])<<6)&0x2000) |
        (((a.m128i.b[14])<<7)&0x4000) | (((a.m128i.b[15])<<8)&0x8000));

  return ret;
}

/*****************************************************************/
/*  NAME : mm_shuffle_epi32                                      */
/*  DESCRIPTION : shuffle single FP                              */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_shuffle_epi32(__m128i a, int imm8)
{
  __m128i ret;

  int t;

  t =  ((imm8 >> 0) & 0x3) ;
  ret.m128i.i[0] = (t==0 ? a.m128i.i[0] :
		  (t==1 ? a.m128i.i[1] :
		   (t==2 ? a.m128i.i[2]: a.m128i.i[3])));

  t =  ((imm8 >> 2) & 0x3) ;
 ret.m128i.i[1] = (t==0 ? a.m128i.i[0] :
		  (t==1 ? a.m128i.i[1] :
		   (t==2 ? a.m128i.i[2]: a.m128i.i[3])));

  t =  ((imm8 >> 4) & 0x3) ;
 ret.m128i.i[2] = (t==0 ? a.m128i.i[0] :
		  (t==1 ? a.m128i.i[1] :
		   (t==2 ? a.m128i.i[2]: a.m128i.i[3])));

  t =  ((imm8 >> 6) & 0x3) ;
 ret.m128i.i[3] = (t==0 ? a.m128i.i[0] :
		  (t==1 ? a.m128i.i[1] :
		   (t==2 ? a.m128i.i[2]: a.m128i.i[3])));
  
  return ret;
}

/*****************************************************************/
/*  NAME : mm_shufflehi_epi16                                    */
/*  DESCRIPTION : Shuffle the upper 4 signed or unsigned 16-bit  */
/*                integers in a and the upper 4 signed 16-bit    */
/*                integers in b as specified in imm              */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_shufflehi_epi16(__m128i a, int imm8)
{
  __m128i ret;

  int t;

  ret.m128i.w[0] = a.m128i.w[0];
  ret.m128i.w[1] = a.m128i.w[1];
  ret.m128i.w[2] = a.m128i.w[2];
  ret.m128i.w[3] = a.m128i.w[3];

  t =  ((imm8 >> 0) & 0x3) ;
  ret.m128i.w[4] = (t==0 ? a.m128i.w[4] :
		  (t==1 ? a.m128i.w[5] :
		   (t==2 ? a.m128i.w[6]: a.m128i.w[7])));

  t =  ((imm8 >> 2) & 0x3) ;
  ret.m128i.w[5] = (t==0 ? a.m128i.w[4] :
		  (t==1 ? a.m128i.w[5] :
		   (t==2 ? a.m128i.w[6]: a.m128i.w[7])));

  t =  ((imm8 >> 4) & 0x3) ;
  ret.m128i.w[6] = (t==0 ? a.m128i.w[4] :
		  (t==1 ? a.m128i.w[5] :
		   (t==2 ? a.m128i.w[6]: a.m128i.w[7])));

  t =  ((imm8 >> 6) & 0x3) ;
  ret.m128i.w[7] = (t==0 ? a.m128i.w[4] :
		  (t==1 ? a.m128i.w[5] :
		   (t==2 ? a.m128i.w[6]: a.m128i.w[7])));
  
  return ret;
}

/*****************************************************************/
/*  NAME : mm_shufflelo_epi16                                    */
/*  DESCRIPTION : Shuffle the lower 4 signed or unsigned 16-bit  */
/*                integers in a and the lower 4 signed 16-bit    */
/*                integers in b as specified in imm              */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_shufflelo_epi16(__m128i a, int imm8)
{
  __m128i ret;

  int t;

  t =  ((imm8 >> 0) & 0x3) ;
  ret.m128i.w[0] = (t==0 ? a.m128i.w[0] :
		  (t==1 ? a.m128i.w[1] :
		   (t==2 ? a.m128i.w[2]: a.m128i.w[3])));

  t =  ((imm8 >> 2) & 0x3) ;
  ret.m128i.w[1] = (t==0 ? a.m128i.w[0] :
		  (t==1 ? a.m128i.w[1] :
		   (t==2 ? a.m128i.w[2]: a.m128i.w[3])));

  t =  ((imm8 >> 4) & 0x3) ;
  ret.m128i.w[2] = (t==0 ? a.m128i.w[0] :
		  (t==1 ? a.m128i.w[1] :
		   (t==2 ? a.m128i.w[2]: a.m128i.w[3])));

  t =  ((imm8 >> 6) & 0x3) ;
  ret.m128i.w[3] = (t==0 ? a.m128i.w[0] :
		  (t==1 ? a.m128i.w[1] :
		   (t==2 ? a.m128i.w[2]: a.m128i.w[3])));

  ret.m128i.w[4] = a.m128i.w[4];
  ret.m128i.w[5] = a.m128i.w[5];
  ret.m128i.w[6] = a.m128i.w[6];
  ret.m128i.w[7] = a.m128i.w[7];
  
  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpackhi_epi8                                      */
/*  DESCRIPTION : Interleaves upper 8 signed or unsigned 8-bit   */
/*                integers in a with the upper 8 8-bit integers  */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpackhi_epi8(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  for (i=0;i<8;i++) {
    ret.m128i.b[i*2] = a.m128i.b[i+8];
    ret.m128i.b[i*2+1] = b.m128i.b[i+8];
  }
  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpackhi_epi16                                     */
/*  DESCRIPTION : Interleaves upper 4 signed or unsigned 16-bit  */
/*                integers in a with the upper 4 16-bit integers */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpackhi_epi16(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  for (i=0;i<4;i++) {
    ret.m128i.w[i*2] = a.m128i.w[i+4];
    ret.m128i.w[i*2+1] = b.m128i.w[i+4];
  }
  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpackhi_epi32                                     */
/*  DESCRIPTION : Interleaves upper 2 signed or unsigned 32-bit  */
/*                integers in a with the upper 2 32-bit integers */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpackhi_epi32(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  ret.m128i.i[0] = a.m128i.i[2];
  ret.m128i.i[1] = b.m128i.i[2];
  ret.m128i.i[2] = a.m128i.i[3];
  ret.m128i.i[3] = b.m128i.i[3];

  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpackhi_epi64                                     */
/*  DESCRIPTION : Interleaves upper signed or unsigned 64-bit    */
/*                integer in a with the upper 64-bit integer     */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpackhi_epi64(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  ret.m128i.i[0] = a.m128i.i[2];
  ret.m128i.i[1] = a.m128i.i[3];

  ret.m128i.i[2] = b.m128i.i[2];
  ret.m128i.i [3] = b.m128i.i[3];

  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpacklo_epi8                                      */
/*  DESCRIPTION : Interleaves lower 8 signed or unsigned 8-bit   */
/*                integers in a with the lower 8 8-bit integers  */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpacklo_epi8(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  for (i=0;i<8;i++) {
    ret.m128i.b[i*2] = a.m128i.b[i];
    ret.m128i.b[i*2+1] = b.m128i.b[i];
  }
  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpacklo_epi16                                     */
/*  DESCRIPTION : Interleaves lower 4 signed or unsigned 16-bit  */
/*                integers in a with the lower 4 16-bit integers */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpacklo_epi16(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  for (i=0;i<4;i++) {
    ret.m128i.w[i*2] = a.m128i.w[i];
    ret.m128i.w[i*2+1] = b.m128i.w[i];
  }
  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpacklo_epi32                                     */
/*  DESCRIPTION : Interleaves lower 2 signed or unsigned 32-bit  */
/*                integers in a with the lower 2 32-bit integers */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpacklo_epi32(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  ret.m128i.i[0] = a.m128i.i[0];
  ret.m128i.i[1] = b.m128i.i[0];
  ret.m128i.i[2] = a.m128i.i[1];
  ret.m128i.i[3] = b.m128i.i[1];

  return ret;
}

/*****************************************************************/
/*  NAME : mm_unpacklo_epi64                                     */
/*  DESCRIPTION : Interleaves lower signed or unsigned 64-bit    */
/*                integer in a with the lower 64-bit integer     */
/*                in b                                           */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128i _mm_unpacklo_epi64(__m128i a, __m128i b)
{
 
  __m128i ret;
  int i;

  ret.m128i.i[0] = a.m128i.i[0];
  ret.m128i.i[1] = a.m128i.i[1];
  ret.m128i.i[2] = b.m128i.i[0];
  ret.m128i.i[3] = b.m128i.i[1];

  return ret;
}

/* Memory Initialization */

/* Load Operations */

/****************************************************/
/*  NAME : mm_load_si128                            */
/*  DESCRIPTION : Loads 128-bit value               */
/*                Address p must be 16-byte aligned */
/*                from                              */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_load_si128(__m128i *p)
{
  __m128i ret;

  _mminternal_assert_16B(p);
  ret = *p;

  return ret;
}

/****************************************************/
/*  NAME : mm_loadu_si128                           */
/*  DESCRIPTION : Loads 128-bit value               */
/*                Address p need not be 16-B aligned*/
/*                from                              */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_loadu_si128(__m128i *p)
{
  __m128i ret;

  ret = *p;

  return ret;
}


/* Set Operations */

/****************************************************/
/*  NAME : mm_set_epi64                             */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set_epi64(__m64 q1, __m64 q0)
{
  __m128i ret;

  ret.m128i.m[0] = *((__int64*)&q0);
  ret.m128i.m[1] = *((__int64*)&q1);
  return ret;
}


/****************************************************/
/*  NAME : mm_set_epi32                             */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set_epi32(int i3, int i2, int i1, int i0)
{
  __m128i ret;

  ret.m128i.i[0] = i0;
  ret.m128i.i[1] = i1;
  ret.m128i.i[2] = i2;
  ret.m128i.i[3] = i3;

  return ret;
}

/****************************************************/
/*  NAME : mm_set_epi16                             */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set_epi16(short w7, short w6, short w5, short w4, short w3, short w2, short w1, short w0)
{
  __m128i ret;

  ret.m128i.w[0] = w0;
  ret.m128i.w[1] = w1;
  ret.m128i.w[2] = w2;
  ret.m128i.w[3] = w3;
  ret.m128i.w[4] = w4;
  ret.m128i.w[5] = w5;
  ret.m128i.w[6] = w6;
  ret.m128i.w[7] = w7;

  return ret;
}


/****************************************************/
/*  NAME : mm_set_epi8                              */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set_epi8
(char b15, char b14, char b13, char b12, char b11, char b10, char b9, char b8, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0)
{
  __m128i ret;

  ret.m128i.b[0] = b0;
  ret.m128i.b[1] = b1;
  ret.m128i.b[2] = b2;
  ret.m128i.b[3] = b3;
  ret.m128i.b[4] = b4;
  ret.m128i.b[5] = b5;
  ret.m128i.b[6] = b6;
  ret.m128i.b[7] = b7;
  ret.m128i.b[8] = b8;
  ret.m128i.b[9] = b9;
  ret.m128i.b[10] = b10;
  ret.m128i.b[11] = b11;
  ret.m128i.b[12] = b12;
  ret.m128i.b[13] = b13;
  ret.m128i.b[14] = b14;
  ret.m128i.b[15] = b15;

  return ret;
}

/****************************************************/
/*  NAME : mm_set1_epi64                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set1_epi64(__m64 q)
{
  __m128i ret;

  ret.m128i.m[0] = (*(__int64*)&q);
  ret.m128i.m[1] = (*(__int64*)&q);
  return ret;
}


/****************************************************/
/*  NAME : mm_set1_epi32                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set1_epi32(int i)
{
  __m128i ret;

  ret.m128i.i[0] = i;
  ret.m128i.i[1] = i;
  ret.m128i.i[2] = i;
  ret.m128i.i[3] = i;

  return ret;
}

/****************************************************/
/*  NAME : mm_set1_epi16                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set1_epi16(short w)
{
  __m128i ret;

  ret.m128i.w[0] = w;
  ret.m128i.w[1] = w;
  ret.m128i.w[2] = w;
  ret.m128i.w[3] = w;
  ret.m128i.w[4] = w;
  ret.m128i.w[5] = w;
  ret.m128i.w[6] = w;
  ret.m128i.w[7] = w;

  return ret;
}


/****************************************************/
/*  NAME : mm_set1_epi8                             */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_set1_epi8(char b)
{
  __m128i ret;

  ret.m128i.b[0] = b;
  ret.m128i.b[1] = b;
  ret.m128i.b[2] = b;
  ret.m128i.b[3] = b;
  ret.m128i.b[4] = b;
  ret.m128i.b[5] = b;
  ret.m128i.b[6] = b;
  ret.m128i.b[7] = b;
  ret.m128i.b[8] = b;
  ret.m128i.b[9] = b;
  ret.m128i.b[10] = b;
  ret.m128i.b[11] = b;
  ret.m128i.b[12] = b;
  ret.m128i.b[13] = b;
  ret.m128i.b[14] = b;
  ret.m128i.b[15] = b;

  return ret;
}

/****************************************************/
/*  NAME : mm_setr_epi64                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_setr_epi64(__m64 q0, __m64 q1)
{
  __m128i ret;

  ret.m128i.m[0] = (*(__int64*)&q0);
  ret.m128i.m[1] = (*(__int64*)&q1);
  return ret;
}


/****************************************************/
/*  NAME : mm_setr_epi32                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_setr_epi32(int i0, int i1, int i2, int i3)
{
  __m128i ret;

  ret.m128i.i[0] = i0;
  ret.m128i.i[1] = i1;
  ret.m128i.i[2] = i2;
  ret.m128i.i[3] = i3;

  return ret;
}

/****************************************************/
/*  NAME : mm_setr_epi16                            */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_setr_epi16(short w0, short w1, short w2, short w3, short w4, short w5, short w6, short w7)
{
  __m128i ret;

  ret.m128i.w[0] = w0;
  ret.m128i.w[1] = w1;
  ret.m128i.w[2] = w2;
  ret.m128i.w[3] = w3;
  ret.m128i.w[4] = w4;
  ret.m128i.w[5] = w5;
  ret.m128i.w[6] = w6;
  ret.m128i.w[7] = w7;

  return ret;
}


/****************************************************/
/*  NAME : mm_setr_epi8                             */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_setr_epi8
(char b0, char b1, char b2, char b3, char b4, char b5, char b6, char b7, char b8, char b9, char b10, char b11, char b12, char b13, char b14, char b15)
{
  __m128i ret;

  ret.m128i.b[0] = b0;
  ret.m128i.b[1] = b1;
  ret.m128i.b[2] = b2;
  ret.m128i.b[3] = b3;
  ret.m128i.b[4] = b4;
  ret.m128i.b[5] = b5;
  ret.m128i.b[6] = b6;
  ret.m128i.b[7] = b7;
  ret.m128i.b[8] = b8;
  ret.m128i.b[9] = b9;
  ret.m128i.b[10] = b10;
  ret.m128i.b[11] = b11;
  ret.m128i.b[12] = b12;
  ret.m128i.b[13] = b13;
  ret.m128i.b[14] = b14;
  ret.m128i.b[15] = b15;

  return ret;
}

/****************************************************/
/*  NAME : mm_setzero_si128                         */
/*  DESCRIPTION : scalar single set                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128i _mm_setzero_si128()
{
  __m128i ret;

  ret.m128i.i[0] = 0x0;
  ret.m128i.i[1] = 0x0;
  ret.m128i.i[2] = 0x0;
  ret.m128i.i[3] = 0x0;
  return ret;
}


/* Store Operations */

/****************************************************/
/*  NAME : mm_store_si128                           */
/*  DESCRIPTION : Stores 128-bit value b            */
/*                Address p must be 16-B aligned    */
/****************************************************/

_MM_INLINE_COMMAND static void _mm_store_si128(__m128i *p, __m128i b)
{
  _mminternal_assert_16B(p);

  ((int*)p)[0] = b.m128i.i[0];
  ((int*)p)[1] = b.m128i.i[1];
  ((int*)p)[2] = b.m128i.i[2];
  ((int*)p)[3] = b.m128i.i[3];
}

/****************************************************/
/*  NAME : mm_storeu_si128                          */
/*  DESCRIPTION : Stores 128-bit value              */
/*                Address p need not be 16-B aligned*/
/****************************************************/

_MM_INLINE_COMMAND static void _mm_storeu_si128(__m128i *p, __m128i b)
{
  *p = b;
}

/****************************************************/
/*  NAME : mm_maskmove_si128                        */
/*  DESCRIPTION : Conditional store bytes           */
/*  NOTE:         Address p need not be 16-byte     */
/*                aligned                           */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_maskmoveu_si128(__m128i d, __m128i n, char *p)
{
  int i;
  for(i=0;i<16;i++) {
    if(n.m128i.b[i] & 0x80) 
      p[i] = d.m128i.b[i];
  }

}

/* Cacheability Support */

/****************************************************/
/*  NAME : mm_stream_si128                          */
/*  DESCRIPTION : Stores the data in a to address p */
/*                without polluting caches.         */
/*  NOTE:         Address p must be 16-byte aligned */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_stream_si128(__m128i *p, __m128i a)
{
  _mminternal_assert_16B(p);
  *p = a;
}

/****************************************************/
/*  NAME : mm_clflush                               */
/*  DESCRIPTION : Cache line containing p is flushed*/
/*                and invalidated from all caches in*/
/*  NOTE:         the coherency domain              */
/****************************************************/
_MM_INLINE_COMMAND static void _mm_clflush(void *p)
{
}

#endif	/* _INCLUDED_EMM_FUNC */
