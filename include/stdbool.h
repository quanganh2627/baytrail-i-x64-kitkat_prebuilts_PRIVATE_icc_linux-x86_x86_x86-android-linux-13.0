/**
*** Copyright (C) 1985-2009 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
**/


/*
 * Standard stdbool.h header file
 */

/* As Intel compiler doesn't understand the name of standard boolean type
_Bool as a keyword, we need to declare it here */

#ifndef _STDBOOL
#define _STDBOOL

#ifdef __cplusplus
  #define _Bool bool
  #define true true
  #define false false
#else
  #define bool _Bool
  #define true 1
  #define false 0
#endif  /* __cplusplus */

#define __bool_true_false_are_defined 1

#endif /* _STDBOOL */


