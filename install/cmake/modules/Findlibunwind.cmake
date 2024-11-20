# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  LIBUNWIND_FOUND
#  LIBUNWIND_INCLUDE_DIR
#  LIBUNWIND_INCLUDE_DIRS
#  LIBUNWIND_<component>_FOUND
#  LIBUNWIND_<component>_LIBRARY
#  LIBUNWIND_LIBRARIES
#  LIBUNWIND_LIBRARY_DIRS
#
# Can be steered using LIBUNWIND_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME libunwind
   INCLUDE_SUFFIXES include INCLUDE_NAMES unwind.h
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS unwind )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( libunwind DEFAULT_MSG LIBUNWIND_INCLUDE_DIR
   LIBUNWIND_LIBRARIES )
mark_as_advanced( LIBUNWIND_FOUND LIBUNWIND_INCLUDE_DIR LIBUNWIND_INCLUDE_DIRS
   LIBUNWIND_LIBRARIES LIBUNWIND_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( libunwind )
