# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module finding BaurMC in the LCG release. Defines:
#  - BAURMC_FOUND
#  - BAURMC_LIBRARIES
#  - BAURMC_LIBRARY_DIRS
#
# Can be steered by BAURMC_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME BaurMC
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS wgamma zgamma )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( BaurMC DEFAULT_MSG BAURMC_LIBRARY_DIRS
   BAURMC_LIBRARIES )
mark_as_advanced( BAURMC_FOUND BAURMC_LIBRARIES BAURMC_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( baurmc )
