# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the quickFit external package.
#
# Defines:
#  QUICKFIT_FOUND
#  QUICKFIT_INCLUDE_DIR
#  QUICKFIT_INCLUDE_DIRS
#  QUICKFIT_LIBRARIES
#  QUICKFIT_LIBRARY_DIRS
#
# The user can set QUICKFIT_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME quickFit
        INCLUDE_SUFFIXES include INCLUDE_NAMES quickFit
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS quickFit )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( quickFit DEFAULT_MSG QUICKFIT_INCLUDE_DIRS
        QUICKFIT_LIBRARIES )
mark_as_advanced( QUICKFIT_FOUND QUICKFIT_INCLUDE_DIR QUICKFIT_INCLUDE_DIRS
        QUICKFIT_LIBRARIES QUICKFIT_LIBRARY_DIRS )


