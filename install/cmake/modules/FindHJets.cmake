# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding HJets in the LCG release. Defines:
#  - HJETS_FOUND
#  - HJETS_INCLUDE_DIR
#  - HJETS_INCLUDE_DIRS
#  - HJETS_LIBRARIES
#  - HJETS_LIBRARY_DIRS
#
# Can be steered by HJETS_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME HJets
   INCLUDE_SUFFIXES include INCLUDE_NAMES HJets/Amplitudes/AmplitudeBase.h
   LIBRARY_SUFFIXES lib lib/HJets
   COMPULSORY_COMPONENTS HJets${CMAKE_SHARED_LIBRARY_SUFFIX} )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( HJets DEFAULT_MSG HJETS_INCLUDE_DIR
   HJETS_LIBRARIES )
mark_as_advanced( HJETS_FOUND HJETS_INCLUDE_DIR HJETS_INCLUDE_DIRS
   HJETS_LIBRARIES HJETS_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( hjets )
