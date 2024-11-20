# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding AlpGen in the LCG release. Defines:
#  - ALPGEN_FOUND
#  - ALPGEN_LIBRARIES
#  - ALPGEN_LIBRARY_DIRS
#
# Can be steered by ALPGEN_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME AlpGen
   INCLUDE_SUFFIXES share/sources INCLUDE_NAMES 2Qlib/2Q.inc 4Qlib/4Q.inc
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS alpgen )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( AlpGen DEFAULT_MSG ALPGEN_INCLUDE_DIR
   ALPGEN_LIBRARIES )
mark_as_advanced( ALPGEN_FOUND ALPGEN_LIBRARIES ALPGEN_LIBRARY_DIRS )

# Set additional environment variable(s):
find_path( ALPGENPATH NAMES 2Qlib/2Q.inc 4Qlib/4Q.inc
   PATHS ${ALPGEN_LCGROOT} PATH_SUFFIXES share/sources )
if( ALPGENPATH )
   set( ALPGEN_ENVIRONMENT
      SET ALPGENPATH ${ALPGENPATH} )
endif()

# Set up the RPM dependency:
lcg_need_rpm( alpgen )
