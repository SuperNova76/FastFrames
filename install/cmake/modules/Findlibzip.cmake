# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  LIBZIP_FOUND
#  LIBZIP_INCLUDE_DIR
#  LIBZIP_INCLUDE_DIRS
#  LIBZIP_<component>_FOUND
#  LIBZIP_<component>_LIBRARY
#  LIBZIP_LIBRARIES
#  LIBZIP_LIBRARY_DIRS
#
# Can be steered using LIBZIP_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME libzip
   INCLUDE_SUFFIXES include INCLUDE_NAMES zip.h
   LIBRARY_SUFFIXES lib lib64
   COMPULSORY_COMPONENTS zip )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( libzip DEFAULT_MSG LIBZIP_INCLUDE_DIR
   LIBZIP_LIBRARIES )
mark_as_advanced( LIBZIP_FOUND LIBZIP_INCLUDE_DIR LIBZIP_INCLUDE_DIRS
   LIBZIP_LIBRARIES LIBZIP_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( libzip )
