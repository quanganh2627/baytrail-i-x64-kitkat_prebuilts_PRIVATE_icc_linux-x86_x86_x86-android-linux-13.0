//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 1985-2011 Intel Corporation. All right reserved.
//
// The information and source code contained herein is the exclusive property
// of Intel Corporation and may not be disclosed, examined, or reproduced in
// whole or in part without explicit written authorization from the Company.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef __INTEL_LIMITS_H
#define __INTEL_LIMITS_H    1

#include_next <limits.h>

        // CHAR PROPERTIES
#undef CHAR_BIT
#define CHAR_BIT            8

#undef UCHAR_MAX
#define UCHAR_MAX           0xff

#undef CHAR_MAX
#undef CHAR_MIN

#if __CHAR_UNSIGNED__
  #define CHAR_MAX          UCHAR_MAX
  #define CHAR_MIN          0
#else
  #define CHAR_MAX          SCHAR_MAX
  #define CHAR_MIN          SCHAR_MIN
#endif

        // SIGNED CHAR PROPERTIES
#undef SCHAR_MAX
#undef SCHAR_MIN

#define SCHAR_MAX           0x7f
#define SCHAR_MIN           (-SCHAR_MAX - 1)

        // MULTIBYTE PROPERTIES
#ifndef MB_LEN_MAX
  #define MB_LEN_MAX        8
#endif

        // SHORT PROPERTIES
#undef SHRT_MAX
#undef SHRT_MIN
#undef USHRT_MAX

#define SHRT_MAX            0x7fff
#define SHRT_MIN            (-SHRT_MAX - 1)
#define USHRT_MAX           0xffff

        // INT PROPERTIES
#undef INT_MAX
#undef INT_MIN
#undef UINT_MAX

#define INT_MAX             0x7fffffff
#define UINT_MAX            0xffffffffU
#define INT_MIN             (-INT_MAX - 1)

        // LONG PROPERTIES
#undef LONG_MAX
#undef LONG_MIN
#undef ULONG_MAX

#ifndef __LP64__
  #define LONG_MAX          0x7fffffffL
  #define ULONG_MAX         0xffffffffUL
#else
  #define LONG_MAX          0x7fffffffffffffffL
  #define ULONG_MAX         0xffffffffffffffffUL
#endif

#define LONG_MIN            (-LONG_MAX - 1)

        // LONG LONG PROPERTIES
#if defined(LLONG_MAX)||defined(LLONG_MIN)||defined(__USE_ISOC99)
  #undef LLONG_MAX
  #undef LLONG_MIN

  #define LLONG_MAX         0x7fffffffffffffffLL
  #define LLONG_MIN         (-LLONG_MAX - 1)
#endif

#ifdef __USE_ISOC99
  #undef ULLONG_MAX
  #define ULLONG_MAX        0xffffffffffffffffULL
#endif

// These macros defined for compatibility with GNU compiler
#ifndef __LONG_LONG_MAX__
  #define __LONG_LONG_MAX__ 0x7fffffffffffffffLL
#endif

#if (defined(__GNU_LIBRARY__) && defined(__USE_GNU)) || \
    (!defined(__GNU_LIBRARY__) && !defined(__STRICT_ANSI__))
  #undef LLONG_MAX
  #undef LLONG_MIN
  #define LLONG_MAX         0x7fffffffffffffffLL
  #define LLONG_MIN         (-LLONG_MAX - 1)
  #undef LONG_LONG_MAX
  #undef LONG_LONG_MIN
  #define LONG_LONG_MAX     LLONG_MAX
  #define LONG_LONG_MIN     LLONG_MIN
  #undef ULONG_LONG_MAX
  #define ULONG_LONG_MAX    ULLONG_MAX
#endif
// End of GNU compiler compatibility macros section

#else
#include_next <limits.h>
#endif // __INTEL_LIMITS_H

