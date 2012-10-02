/**
*** Copyright (C) 2006-2007 Intel Corporation.  All rights reserved.
***/

#define _MUDFLAP

#include <mf-runtime.h>

#ifdef _MUDFLAP
/* we need these declarations but they are conditonalized out of mf-runtime.h> */
struct __mf_cache { __mf_uintptr_t low; __mf_uintptr_t high; };
extern struct __mf_cache __mf_lookup_cache [];
extern __mf_uintptr_t __mf_lc_mask;
extern unsigned char __mf_lc_shift;
#endif


static inline void * __intel_mf_check (void *ptr, __mf_size_t sz, int type, const char *location)
       __attribute((nothrow));

static inline void * __intel_mf_check (void *ptr, __mf_size_t sz, int type, const char *location)
{
    __mf_uintptr_t pt = (__mf_uintptr_t)ptr;
    __mf_uintptr_t ptcmp = pt + sz - 1; 
    struct __mf_cache *t_ptr = &__mf_lookup_cache [(pt >> __mf_lc_shift) & __mf_lc_mask];
    if ((pt < t_ptr->low) || (ptcmp > t_ptr->high) /*|| (type != 0)*/) {
      __mf_check (ptr,sz,type,location);
    }
    return ptr;
}

static inline void * __intel_mf_check_index (void *base, __mf_uintptr_t index, __mf_size_t sz, int type, const char *location)
	__attribute((nothrow));

/*	Index from pointer version */
static inline void * __intel_mf_check_index (void *base, __mf_uintptr_t index, __mf_size_t sz, int type, const char *location)
{
    __intel_mf_check(base, (index + 1) *sz , type,location);
    return (void *)((char *)base + index * sz);
}

/*	Field offset */
static inline void * __intel_mf_check_field (void *base, __mf_uintptr_t offset, __mf_size_t sz, int type, const char *location)
{
    __intel_mf_check(base, offset + sz , type,location);
    return (void *)((char *)base + offset);
}


