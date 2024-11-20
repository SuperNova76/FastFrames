# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Try to find GMP.
#
# Defines:
#  - GMP_FOUND
#  - GMP_INCLUDE_DIRS
#  - GMP_LIBRARIES
#
# Can be steered by GMP_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME gmp
   INCLUDE_SUFFIXES include INCLUDE_NAMES gmp.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS gmp )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( gmp DEFAULT_MSG GMP_INCLUDE_DIR
   GMP_LIBRARIES )
mark_as_advanced( GMP_FOUND GMP_INCLUDE_DIR GMP_INCLUDE_DIRS
   GMP_LIBRARIES GMP_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( gmp )
