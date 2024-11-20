# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  LIBFFI_FOUND
#  LIBFFI_INCLUDE_DIR
#  LIBFFI_INCLUDE_DIRS
#  LIBFFI_<component>_FOUND
#  LIBFFI_<component>_LIBRARY
#  LIBFFI_LIBRARIES
#  LIBFFI_LIBRARY_DIRS
#
# Can be steered using LIBFFI_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME libffi
   INCLUDE_SUFFIXES include lib/libffi-${LIBFFI_LCGVERSION}/include
   INCLUDE_NAMES ffi.h
   LIBRARY_SUFFIXES lib lib64
   COMPULSORY_COMPONENTS ffi )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( libffi DEFAULT_MSG LIBFFI_INCLUDE_DIR
   LIBFFI_LIBRARIES )
mark_as_advanced( LIBFFI_FOUND LIBFFI_INCLUDE_DIR LIBFFI_INCLUDE_DIRS
   LIBFFI_LIBRARIES LIBFFI_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( libffi )
