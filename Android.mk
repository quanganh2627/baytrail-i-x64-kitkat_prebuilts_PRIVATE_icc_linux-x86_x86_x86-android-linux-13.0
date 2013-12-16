ifneq ($(call intel-target-need-intel-libraries),)
# ICC_LIB* are absolute paths, so LOCAL_PATH is /
LOCAL_PATH := /
# if there's ICC compiler configured for some component build,
# then query the compiler about the paths to libs

ICC_LIBCHKP_SO    := $(call intel-target-compiler-lib,libchkp.so)
ICC_LIBCILKRTS_SO := $(call intel-target-compiler-lib,libcilkrts.so)
# ICC may have 2 different Cilk Plus RTS: for GNU Stl and STLport shared
ifeq ($(strip $(ICC_LIBCILKRTS_SO)),libcilkrts.so)
ICC_LIBCILKRTS_SO := $(call intel-target-compiler-lib,gnustl/libcilkrts.so)
endif
ICC_LIBIMF_SO     := $(call intel-target-compiler-lib,libimf.so)
ICC_LIBINTLC_SO   := $(call intel-target-compiler-lib,libintlc.so)
ICC_LIBIRNG_SO    := $(call intel-target-compiler-lib,libirng.so)
ICC_LIBPDBX_SO    := $(call intel-target-compiler-lib,libpdbx.so)
# ICC may have 2 different PDBX libs: for GNU Stl and STLport shared
ifeq ($(strip $(ICC_LIBPDBX_SO)),libpdbx.so)
ICC_LIBPDBX_SO    := $(call intel-target-compiler-lib,gnustl/libpdbx.so)
endif
ICC_LIBSVML_SO    := $(call intel-target-compiler-lib,libsvml.so)


ICC_LIBBFP754_A   := $(call intel-target-compiler-lib,libbfp754.a)
ICC_LIBDECIMAL_A  := $(call intel-target-compiler-lib,libdecimal.a)
ICC_LIBIMF_A      := $(call intel-target-compiler-lib,libimf.a)
ICC_LIBIPGO_A     := $(call intel-target-compiler-lib,libipgo.a)
ICC_LIBIRC_A      := $(call intel-target-compiler-lib,libirc.a)
ICC_LIBIRC_S_A    := $(call intel-target-compiler-lib,libirc_s.a)
ICC_LIBIRNG_A     := $(call intel-target-compiler-lib,libirng.a)
# Provided by other means
ICC_LIBSVML_A     :=

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS :=                       \
                       $(ICC_LIBCHKP_SO)     \
                       $(ICC_LIBCILKRTS_SO)  \
                       $(ICC_LIBIMF_SO)      \
                       $(ICC_LIBINTLC_SO)    \
                       $(ICC_LIBIRNG_SO)     \
                       $(ICC_LIBPDBX_SO)     \
                       $(ICC_LIBSVML_SO)     \
                                             \
                       $(ICC_LIBBFP754_A)    \
                       $(ICC_LIBDECIMAL_A)   \
                       $(ICC_LIBIMF_A)       \
                       $(ICC_LIBIPGO_A)      \
                       $(ICC_LIBIRC_A)       \
                       $(ICC_LIBIRC_S_A)     \
                       $(ICC_LIBIRNG_A)      \
                       $(ICC_LIBSVML_A)

include $(BUILD_MULTI_PREBUILT)

endif
