/**
*** Copyright (C) 2009 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

#if defined(__ANDROID__)
// Avoid conflicts with definitions from <sysroot>/usr/include/ctype.h
#undef _N
#undef _P
#undef _S
#endif

typedef struct _ATOMIC_ITYPE {
    _INTEGRAL _Val;
#if __cplusplus
    _STRONG_INLINE bool is_lock_free() const volatile
    {
        return true;
    }
    _STRONG_INLINE bool is_lock_free() const
    {
        return true;
    }
    _STRONG_INLINE void store(_INTEGRAL _N,
                              memory_order _O = memory_order_seq_cst) volatile
    {
        __atomic_store_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE void store(_INTEGRAL _N,
                              memory_order _O = memory_order_seq_cst)
    {
        __atomic_store_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL load(memory_order _O = memory_order_seq_cst) const volatile
    {
        return __atomic_load_explicit(&_Val, _O);
    }
    _STRONG_INLINE _INTEGRAL load(memory_order _O = memory_order_seq_cst) const
    {
        return __atomic_load_explicit(&_Val, _O);
    }
    _STRONG_INLINE operator _INTEGRAL() const volatile
    {
        return __atomic_load_explicit(&_Val, memory_order_seq_cst);
    }
    _STRONG_INLINE operator _INTEGRAL() const
    {
        return __atomic_load_explicit(&_Val, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL exchange(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_exchange_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL exchange(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst)
    {
        return __atomic_exchange_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _S, memory_order _F) volatile
    {
        return __atomic_compare_exchange_weak_explicit(&_Val, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _S, memory_order _F)
    {
        return __atomic_compare_exchange_weak_explicit(&_Val, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _S, memory_order _F) volatile
    {
        return __atomic_compare_exchange_strong_explicit(&_Val, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _S, memory_order _F)
    {
        return __atomic_compare_exchange_strong_explicit(&_Val, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_compare_exchange_weak_explicit(&_Val, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _O = memory_order_seq_cst)
    {
        return __atomic_compare_exchange_weak_explicit(&_Val, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_compare_exchange_strong_explicit(&_Val, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _O = memory_order_seq_cst)
    {
        return __atomic_compare_exchange_strong_explicit(&_Val, &_P, _N, _O, memory_order_seq_cst);
    }
#if _ADDITIVE
#if _REALLY_ADDRESS
#define _ADDEND ptrdiff_t
#define _CAST(x) (char *)(x)
#else
#define _ADDEND _INTEGRAL
#define _CAST(x) (x)
#endif
    _STRONG_INLINE _INTEGRAL fetch_add(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_fetch_add_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_add(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return __atomic_fetch_add_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_sub(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_fetch_sub_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_sub(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return __atomic_fetch_sub_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL operator+=(_ADDEND _N) volatile
    {
        return _CAST(__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator+=(_ADDEND _N)
    {
        return _CAST(__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator-=(_ADDEND _N) volatile
    {
        return _CAST(__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator-=(_ADDEND _N)
    {
        return _CAST(__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
#undef _CAST
#if !_REALLY_ADDRESS
    _STRONG_INLINE _INTEGRAL operator++(int) volatile
    {
        return __atomic_fetch_add_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++(int)
    {
        return __atomic_fetch_add_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++() volatile
    {
        return __atomic_add_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++()
    {
        return __atomic_add_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--(int) volatile
    {
        return __atomic_fetch_sub_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--(int)
    {
        return __atomic_fetch_sub_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--() volatile
    {
        return __atomic_sub_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--()
    {
        return __atomic_sub_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
#endif
#endif
#if _BITWISE
    _STRONG_INLINE _INTEGRAL fetch_and(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_fetch_and_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_and(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return __atomic_fetch_and_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_or(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_fetch_or_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_or(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst)
    {
        return __atomic_fetch_or_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_xor(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return __atomic_fetch_xor_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_xor(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return __atomic_fetch_xor_explicit(&_Val, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL operator&=(_INTEGRAL _N) volatile
    {
        return __atomic_and_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator&=(_INTEGRAL _N)
    {
        return __atomic_and_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator|=(_INTEGRAL _N) volatile
    {
        return __atomic_or_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator|=(_INTEGRAL _N)
    {
        return __atomic_or_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator^=(_INTEGRAL _N) volatile
    {
        return __atomic_xor_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator^=(_INTEGRAL _N)
    {
        return __atomic_xor_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
#endif
    _STRONG_INLINE _ATOMIC_ITYPE() _DEFAULTED
    _CONSTEXPR _STRONG_INLINE _ATOMIC_ITYPE(_INTEGRAL _N)
    : _Val(_N)
    {
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N) volatile
    {
        __atomic_store_explicit(&_Val, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N)
    {
        __atomic_store_explicit(&_Val, _N, memory_order_seq_cst);
        return _N;
    }
    _DELETED( _ATOMIC_ITYPE(const _ATOMIC_ITYPE&) )
    _DELETED( _ATOMIC_ITYPE& operator=(const _ATOMIC_ITYPE&) )
    _DELETED( _ATOMIC_ITYPE& operator=(const _ATOMIC_ITYPE&) volatile )
#endif
} _ATOMIC_ITYPE;

#if __cplusplus
#if !_REALLY_ADDRESS
template <>
struct atomic<_INTEGRAL> : _ATOMIC_ITYPE
{
    _STRONG_INLINE atomic() _DEFAULTED
    _CONSTEXPR _STRONG_INLINE atomic(_INTEGRAL _N)
    : _ATOMIC_ITYPE(_N)
    {
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N) volatile
    {
        __atomic_store_explicit(&_Val, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N)
    {
        __atomic_store_explicit(&_Val, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE operator _INTEGRAL() const volatile
    {
        return __atomic_load_explicit(&_Val, memory_order_seq_cst);
    }
    _STRONG_INLINE operator _INTEGRAL() const
    {
        return __atomic_load_explicit(&_Val, memory_order_seq_cst);
    }
    _DELETED( atomic(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) volatile )
};
#endif

_STRONG_INLINE _CONSTEXPR bool atomic_is_lock_free(_ATOMIC_ITYPE const volatile *) _NOEXCEPT
{
    return true;
}

_STRONG_INLINE void atomic_init(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    _P->_Val = _V;
}

_STRONG_INLINE void atomic_store(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    _P->store(_V);
}

_STRONG_INLINE void atomic_store_explicit(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V, memory_order _O) _NOEXCEPT
{
    _P->store(_V, _O);
}

_STRONG_INLINE _INTEGRAL atomic_load(_ATOMIC_ITYPE volatile const *_P) _NOEXCEPT
{
    return _P->load();
}

_STRONG_INLINE _INTEGRAL atomic_load_explicit(_ATOMIC_ITYPE volatile const *_P, memory_order _O) _NOEXCEPT
{
    return _P->load(_O);
}

_STRONG_INLINE _INTEGRAL atomic_exchange(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    return _P->exchange(_V);
}

_STRONG_INLINE _INTEGRAL atomic_exchange_explicit(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V, memory_order _O) _NOEXCEPT
{
    return _P->exchange(_V, _O);
}

_STRONG_INLINE bool atomic_compare_exchange_weak(_ATOMIC_ITYPE volatile *_AP, _INTEGRAL *_IP, _INTEGRAL _V) _NOEXCEPT
{
    return _AP->compare_exchange_weak(*_IP, _V);
}

_STRONG_INLINE bool atomic_compare_exchange_weak_explicit(_ATOMIC_ITYPE volatile *_AP, _INTEGRAL *_IP, _INTEGRAL _V, memory_order _S, memory_order _F) _NOEXCEPT
{
    return _AP->compare_exchange_weak(*_IP, _V, _S, _F);
}

_STRONG_INLINE bool atomic_compare_exchange_strong(_ATOMIC_ITYPE volatile *_AP, _INTEGRAL *_IP, _INTEGRAL _V) _NOEXCEPT
{
    return _AP->compare_exchange_strong(*_IP, _V);
}

_STRONG_INLINE bool atomic_compare_exchange_strong_explicit(_ATOMIC_ITYPE volatile *_AP, _INTEGRAL *_IP, _INTEGRAL _V, memory_order _S, memory_order _F) _NOEXCEPT
{
    return _AP->compare_exchange_strong(*_IP, _V, _S, _F);
}

#if _ADDITIVE
_STRONG_INLINE _INTEGRAL atomic_fetch_add(_ATOMIC_ITYPE volatile *_P, _ADDEND _V) _NOEXCEPT
{
    return _P->fetch_add(_V);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_add_explicit(_ATOMIC_ITYPE volatile *_P, _ADDEND _V, memory_order _O) _NOEXCEPT
{
    return _P->fetch_add(_V, _O);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_sub(_ATOMIC_ITYPE volatile *_P, _ADDEND _V) _NOEXCEPT
{
    return _P->fetch_sub(_V);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_sub_explicit(_ATOMIC_ITYPE volatile *_P, _ADDEND _V, memory_order _O) _NOEXCEPT
{
    return _P->fetch_sub(_V, _O);
}
#endif

#if _BITWISE
_STRONG_INLINE _INTEGRAL atomic_fetch_and(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    return _P->fetch_and(_V);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_and_explicit(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V, memory_order _O) _NOEXCEPT
{
    return _P->fetch_and(_V, _O);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_or(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    return _P->fetch_or(_V);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_or_explicit(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V, memory_order _O) _NOEXCEPT
{
    return _P->fetch_or(_V, _O);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_xor(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V) _NOEXCEPT
{
    return _P->fetch_xor(_V);
}

_STRONG_INLINE _INTEGRAL atomic_fetch_xor_explicit(_ATOMIC_ITYPE volatile *_P, _INTEGRAL _V, memory_order _O) _NOEXCEPT
{
    return _P->fetch_xor(_V, _O);
}
#endif

#endif

#undef _ATOMIC_ITYPE
#undef _INTEGRAL
#undef _REALLY_ADDRESS
#undef _ADDEND
