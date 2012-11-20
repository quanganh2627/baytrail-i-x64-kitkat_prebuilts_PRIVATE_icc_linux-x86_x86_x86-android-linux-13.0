#!/bin/bash
#
# Copyright  (C) 1985-2012 Intel Corporation. All rights reserved.
#
# The information and source code contained herein is the exclusive property
# of Intel Corporation and may not be disclosed, examined, or reproduced in
# whole or in part without explicit written authorization from the Company.
#

INSTALL_DIR=/nfs/ins/proj/icl/embedded/public/android/builds/20121115_13.0

# Check if script is sourced:
case $0 in
*iccvars*)
    echo "Warning: THIS SCRIPT SHALL BE SOURCED!"
;;
esac

test -z "$ANDROID_SYSROOT" || test -z "$ANDROID_GNU_X86_TOOLCHAIN"
if [ $? -eq 0 ]; then
    echo
    echo Error: ANDROID_SYSROOT and ANDROID_GNU_X86_TOOLCHAIN echo environment variables must be set !!
    echo
    echo For example if NDK is the top of Android NDK:
    echo
    echo export ANDROID_SYSROOT=\$NDK/platforms/android-14/arch-x86
    echo export ANDROID_GNU_X86_TOOLCHAIN=\$NDK/toolchains/x86-4.6/prebuilt/linux-x86
    echo 
    echo or if TOPDIR is the top of Android repository:
    echo
    echo export ANDROID_SYSROOT=\$TOPDIR/prebuilts/ndk/android-ndk-r7/platforms/android-14/arch-x86
    echo export ANDROID_GNU_X86_TOOLCHAIN=\$TOPDIR/prebuilts/gcc/linux-x86/x86/i686-linux-android-4.6
    echo
    return 1
fi

if [ -z "${INTEL_LICENSE_FILE}" ]
then
    INTEL_LICENSE_FILE="${INSTALL_DIR}/licenses:/opt/intel/licenses:${HOME}/intel/licenses:/Users/Shared/Library/Application Support/Intel/Licenses";
else
    INTEL_LICENSE_FILE="${INTEL_LICENSE_FILE}:${INSTALL_DIR}/licenses:/opt/intel/licenses:${HOME}/intel/licenses:/Users/Shared/Library/Application Support/Intel/Licenses";
fi
export INTEL_LICENSE_FILE;

if [ -z "${NLSPATH}" ] 
then
    NLSPATH="${INSTALL_DIR}/lib/locale/en_US/%N"; 
else
    NLSPATH="${INSTALL_DIR}/lib/locale/en_US/%N:${NLSPATH}";
fi
export NLSPATH;

if [ -z "${MANPATH}" ]
then
    MANPATH="${INSTALL_DIR}/man"; 
else
    MANPATH="${INSTALL_DIR}/man:${MANPATH}";
fi
export MANPATH;

if [ -z "${PATH}" ]
then
    PATH="${INSTALL_DIR}/bin";
else
    PATH="${INSTALL_DIR}/bin:${PATH}";
fi
export PATH;
