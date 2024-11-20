# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the workspaceCombiner external package.
#
# Defines:
#  WORKSPACECOMBINER_FOUND
#  WORKSPACECOMBINER_INCLUDE_DIR
#  WORKSPACECOMBINER_INCLUDE_DIRS
#  WORKSPACECOMBINER_LIBRARIES
#  WORKSPACECOMBINER_LIBRARY_DIRS
#
# The user can set WORKSPACECOMBINER_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME workspaceCombiner
        INCLUDE_SUFFIXES include INCLUDE_NAMES workspaceCombiner
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS workspaceCombiner )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( workspaceCombiner DEFAULT_MSG WORKSPACECOMBINER_INCLUDE_DIRS
        WORKSPACECOMBINER_LIBRARIES )
mark_as_advanced( WORKSPACECOMBINER_FOUND WORKSPACECOMBINER_INCLUDE_DIR WORKSPACECOMBINER_INCLUDE_DIRS
        WORKSPACECOMBINER_LIBRARIES WORKSPACECOMBINER_LIBRARY_DIRS )


