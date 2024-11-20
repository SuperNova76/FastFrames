# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the CMSCombine external package.
#
# Defines:
#  CMSCOMBINE_FOUND
#  CMSCOMBINE_INCLUDE_DIR
#  CMSCOMBINE_INCLUDE_DIRS
#  CMSCOMBINE_LIBRARIES
#  CMSCOMBINE_LIBRARY_DIRS
#
# The user can set CMSCOMBINE_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME CMSCombine
        INCLUDE_SUFFIXES include INCLUDE_NAMES CMSCombine
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS CMSCombine )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CMSCombine DEFAULT_MSG CMSCOMBINE_INCLUDE_DIRS
        CMSCOMBINE_LIBRARIES )
mark_as_advanced( CMSCOMBINE_FOUND CMSCOMBINE_INCLUDE_DIR CMSCOMBINE_INCLUDE_DIRS
        CMSCOMBINE_LIBRARIES CMSCOMBINE_LIBRARY_DIRS )


