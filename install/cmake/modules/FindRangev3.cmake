# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  RANGEV3_FOUND
#  RANGEV3_INCLUDE_DIR
#  RANGEV3_INCLUDE_DIRS
#
# Can be steered by RANGEV3_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Rangev3
   INCLUDE_SUFFIXES include INCLUDE_NAMES range/v3/all.hpp )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Rangev3 DEFAULT_MSG RANGEV3_INCLUDE_DIR )
mark_as_advanced( RANGEV3_FOUND RANGEV3_INCLUDE_DIR RANGEV3_INCLUDE_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( rangev3 )
