# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Locate the Apfel package.
# Defines:
#  - APFEL_FOUND
#  - APFEL_INCLUDE_DIR
#  - APFEL_INCLUDE_DIRS
#  - APFEL_LIBRARIES
#  - APFEL_LIBRARY_DIRS
#
# Can be steered by APFEL_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Apfel
   INCLUDE_SUFFIXES include INCLUDE_NAMES APFEL/APFEL.h 
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS APFEL )


# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Apfel DEFAULT_MSG APFEL_INCLUDE_DIR 
   APFEL_LIBRARIES  )
mark_as_advanced( APFEL_FOUND APFEL_INCLUDE_DIR APFEL_INCLUDE_DIRS 
   APFEL_LIBRARIES APFEL_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( apfel )

