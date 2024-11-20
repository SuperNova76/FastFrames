# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding Superchic in the LCG release. Defines:
#  - SUPERCHIC_FOUND
#  - SUPERCHIC_INCLUDE_DIR
#  - SUPERCHIC_INCLUDE_DIRS
#  - SUPERCHIC_LIBRARIES
#  - SUPERCHIC_LIBRARY_DIRS
#
# Can be steered by SUPERCHIC_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Superchic
   BINARY_NAMES superchic
   BINARY_SUFFIXES bin bin32 bin64
   INCLUDE_SUFFIXES include src/inc
   INCLUDE_NAMES anom.f bpsi.f
   LIBRARY_SUFFIXES lib lib64
   COMPULSORY_COMPONENTS superchic )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Superchic DEFAULT_MSG SUPERCHIC_INCLUDE_DIR
   SUPERCHIC_LIBRARIES SUPERCHIC_LIBRARY_DIRS )
mark_as_advanced( SUPERCHIC_FOUND SUPERCHIC_INCLUDE_DIR SUPERCHIC_INCLUDE_DIRS
   SUPERCHIC_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( superchic )

# Set the SUPERCHIC environment variable:
if( SUPERCHIC_FOUND )
   set( SUPERCHIC_ENVIRONMENT
      FORCESET SUPERCHICPATH ${SUPERCHIC_LCGROOT} )
endif()
