# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the RooFitExtensions external package.
#
# Defines:
#  ROOFITEXTENSIONS_FOUND
#  ROOFITEXTENSIONS_INCLUDE_DIR
#  ROOFITEXTENSIONS_INCLUDE_DIRS
#  ROOFITEXTENSIONS_LIBRARIES
#  ROOFITEXTENSIONS_LIBRARY_DIRS
#
# The user can set ROOFITEXTENSIONS_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME RooFitExtensions
        INCLUDE_SUFFIXES include INCLUDE_NAMES RooFitExtensions
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS RooFitExtensions )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( RooFitExtensions DEFAULT_MSG ROOFITEXTENSIONS_INCLUDE_DIRS
        ROOFITEXTENSIONS_LIBRARIES )
mark_as_advanced( ROOFITEXTENSIONS_FOUND ROOFITEXTENSIONS_INCLUDE_DIR ROOFITEXTENSIONS_INCLUDE_DIRS
        ROOFITEXTENSIONS_LIBRARIES ROOFITEXTENSIONS_LIBRARY_DIRS )


