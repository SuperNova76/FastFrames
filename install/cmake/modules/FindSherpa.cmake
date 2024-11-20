# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Module finding Sherpa in the LCG release. Defines:
#  - SHERPA_FOUND
#  - SHERPA_INCLUDE_DIR
#  - SHERPA_INCLUDE_DIRS
#  - SHERPA_<component>_FOUND
#  - SHERPA_<component>_LIBRARY
#  - SHERPA_LIBRARIES
#  - SHERPA_LIBRARY_DIRS
#  - SHERPA_Sherpa_EXECUTABLE
#  - SHERPA_Sherpa-config_EXECUTABLE
#  - SHERPA_BINARY_PATH
#
# Can be steered by SHERPA_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Sherpa
   INCLUDE_SUFFIXES include/SHERPA-MC INCLUDE_NAMES SHERPA/Main/Sherpa.H
   LIBRARY_SUFFIXES lib/SHERPA-MC
   COMPULSORY_COMPONENTS SherpaMain ToolsMath ToolsOrg
   BINARY_NAMES Sherpa Sherpa-config
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Sherpa DEFAULT_MSG SHERPA_INCLUDE_DIR
   SHERPA_LIBRARIES )
mark_as_advanced( SHERPA_FOUND SHERPA_INCLUDE_DIR SHERPA_INCLUDE_DIRS
   SHERPA_LIBRARIES SHERPA_LIBRARY_DIRS SHERPA_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( sherpa )
