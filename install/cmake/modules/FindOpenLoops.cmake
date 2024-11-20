# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding HJets in the LCG release. Defines:
#  - OPENLOOPS_FOUND
#  - OPENLOOPS_INCLUDE_DIR
#  - OPENLOOPS_INCLUDE_DIRS
#  - OPENLOOPS_LIBRARIES
#  - OPENLOOPS_LIBRARY_DIRS
#
# Can be steered by OPENLOOPS_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME OpenLoops
   INCLUDE_SUFFIXES include INCLUDE_NAMES openloops.h
   LIBRARY_SUFFIXES lib lib64
   COMPULSORY_COMPONENTS openloops )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( OpenLoops DEFAULT_MSG OPENLOOPS_INCLUDE_DIR
   OPENLOOPS_LIBRARIES )
mark_as_advanced( OPENLOOPS_FOUND OPENLOOPS_INCLUDE_DIR OPENLOOPS_INCLUDE_DIRS
   OPENLOOPS_LIBRARIES OPENLOOPS_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( openloops )
