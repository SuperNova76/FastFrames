# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding Pythia6 in the LCG release. Defines:
#  - PYTHIA6_FOUND
#  - PYTHIA6_INCLUDE_DIR
#  - PYTHIA6_INCLUDE_DIRS
#  - PYTHIA6_LIBRARIES
#  - PYTHIA6_LIBRARY_DIRS
#
# Can be steered by PYTHIA6_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Pythia6
   INCLUDE_SUFFIXES include INCLUDE_NAMES general_pythia.inc
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS pythia6 )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Pythia6 DEFAULT_MSG PYTHIA6_INCLUDE_DIR
   PYTHIA6_LIBRARIES )
mark_as_advanced( PYTHIA6_FOUND PYTHIA6_INCLUDE_DIR PYTHIA6_INCLUDE_DIRS
   PYTHIA6_LIBRARIES PYTHIA6_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( pythia6 )
