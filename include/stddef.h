///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2002-2012 Intel Corporation. All right reserved.
//
// The information and source code contained herein is the exclusive property
// of Intel Corporation and may not be disclosed, examined, or reproduced
// in whole or in part without explicit written authorization from the Company.
//
///////////////////////////////////////////////////////////////////////////////

// cvs_id[] = "$Id: stddef.h 26525 2012-02-15 10:27:05Z ipolezha $";

#include_next <stddef.h>

#ifdef __INTEL_COMPILER

#if defined(offsetof) && defined(__GNUC__) && (__GNUC__ < 4)
#undef offsetof
#define offsetof(T, member)     ((size_t)&(((T *)0)->member))
#endif

#endif // #ifdef __INTEL_COMPILER
