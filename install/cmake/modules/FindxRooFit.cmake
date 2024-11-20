# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the xRooFit external package.
#
# Defines:
#  XROOFIT_FOUND
#  XROOFIT_INCLUDE_DIR
#  XROOFIT_INCLUDE_DIRS
#  XROOFIT_LIBRARIES
#  XROOFIT_LIBRARY_DIRS
#
# The user can set XROOFIT_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME xRooFit
        INCLUDE_SUFFIXES include INCLUDE_NAMES xRooFit
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS xRooFit )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( xRooFit DEFAULT_MSG XROOFIT_INCLUDE_DIRS
        XROOFIT_LIBRARIES )
mark_as_advanced( XROOFIT_FOUND XROOFIT_INCLUDE_DIR XROOFIT_INCLUDE_DIRS
        XROOFIT_LIBRARIES XROOFIT_LIBRARY_DIRS )


