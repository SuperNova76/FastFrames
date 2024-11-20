# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  HEAPTRACK_FOUND
#  HEAPTRACK_INCLUDE_DIR
#  HEAPTRACK_INCLUDE_DIRS
#  HEAPTRACK_BINARY_PATH
#  HEAPTRACK_heaptrack_EXECUTABLE
#
# Can be steered by HEAPTRACK_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME heaptrack
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES heaptrack_api.h
   BINARY_SUFFIXES bin
   BINARY_NAMES heaptrack )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( heaptrack DEFAULT_MSG
   HEAPTRACK_INCLUDE_DIR HEAPTRACK_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( heaptrack )
