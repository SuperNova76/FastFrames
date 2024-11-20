# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the MCUtils package.
# Defines:
#  - MCUTILS_FOUND
#  - MCUTILS_INCLUDE_DIR
#  - MCUTILS_INCLUDE_DIRS
#
# Can be steered by MCUTILS_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME MCUtils
   INCLUDE_SUFFIXES include INCLUDE_NAMES MCUtils/MCUtils.h )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MCUtils DEFAULT_MSG MCUTILS_INCLUDE_DIR )
mark_as_advanced( MCUTILS_FOUND MCUTILS_INCLUDE_DIR MCUTILS_INCLUDE_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( mcutils )
