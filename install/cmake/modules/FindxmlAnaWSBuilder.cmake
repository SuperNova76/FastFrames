# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the xmlAnaWSBuilder external package.
#
# Defines:
#  XMLANAWSBUILDER_FOUND
#  XMLANAWSBUILDER_INCLUDE_DIR
#  XMLANAWSBUILDER_INCLUDE_DIRS
#  XMLANAWSBUILDER_LIBRARIES
#  XMLANAWSBUILDER_LIBRARY_DIRS
#
# The user can set XMLANAWSBUILDER_ATROOT to guide the script.
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME xmlAnaWSBuilder
        INCLUDE_SUFFIXES include INCLUDE_NAMES xmlAnaWSBuilder
        LIBRARY_SUFFIXES lib
        COMPULSORY_COMPONENTS xmlAnaWSBuilder )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( xmlAnaWSBuilder DEFAULT_MSG XMLANAWSBUILDER_INCLUDE_DIRS
        XMLANAWSBUILDER_LIBRARIES )
mark_as_advanced( XMLANAWSBUILDER_FOUND XMLANAWSBUILDER_INCLUDE_DIR XMLANAWSBUILDER_INCLUDE_DIRS
        XMLANAWSBUILDER_LIBRARIES XMLANAWSBUILDER_LIBRARY_DIRS )


