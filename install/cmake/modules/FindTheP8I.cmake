# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Locate the TheP8I package.
# Defines:
#  - TheP8I_FOUND
#  - TheP8I_INCLUDE_DIR
#  - TheP8I_INCLUDE_DIRS
#  - TheP8I_LIBRARIES
#  - TheP8I_LIBRARY_DIRS
#
# Can be steered by TheP8I_LCGROOT.
#

# The LCG include(s): 
include( LCGFunctions )
   
# Declare the external module:
lcg_external_module( NAME TheP8I
   INCLUDE_SUFFIXES share INCLUDE_NAMES TheP8I/TheP8IDefaults.in
   LIBRARY_SUFFIXES lib64/ThePEG
   COMPULSORY_COMPONENTS TheP8I )


# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( TheP8I DEFAULT_MSG THEP8I_INCLUDE_DIR
   THEP8I_LIBRARIES  )
mark_as_advanced( THEP8I_FOUND THEP8I_INCLUDE_DIR THEP8I_INCLUDE_DIRS
   THEP8I_LIBRARIES THEP8I_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( thep8i )
