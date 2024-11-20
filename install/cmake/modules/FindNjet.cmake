# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Module finding NJet in the LCG release. Defines:
#  - NJET_FOUND
#  - NJET_INCLUDE_DIR
#  - NJET_INCLUDE_DIRS
#  - NJET_LIBRARIES
#  - NJET_LIBRARY_DIRS
#
# Can be steered by NJET_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Njet
   INCLUDE_SUFFIXES include INCLUDE_NAMES njet.h
   LIBRARY_SUFFIXES lib lib64
   COMPULSORY_COMPONENTS njet2 )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Njet DEFAULT_MSG NJET_INCLUDE_DIR
   NJET_LIBRARIES )
mark_as_advanced( NJET_FOUND NJET_INCLUDE_DIR NJET_INCLUDE_DIRS
   NJET_LIBRARIES NJET_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( njet )
