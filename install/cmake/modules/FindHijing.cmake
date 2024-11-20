# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding Hijing in the LCG release. Defines:
#  - HIJING_FOUND
#  - HIJING_LIBRARIES
#  - HIJING_LIBRARY_DIRS
#
# Can be steered by HIJING_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Hijing
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS hijing )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Hijing DEFAULT_MSG HIJING_LIBRARY_DIRS
   HIJING_LIBRARIES )
mark_as_advanced( HIJING_FOUND HIJING_LIBRARIES HIJING_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( hijing )
