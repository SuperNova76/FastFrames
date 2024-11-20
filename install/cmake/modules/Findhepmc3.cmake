# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# - Locate hepmc3 package
# Defines:
#
#  HEPMC3_FOUND
#  HEPMC3_INCLUDE_DIR
#  HEPMC3_INCLUDE_DIRS
#  HEPMC3_<component>_FOUND
#  HEPMC3_<component>_LIBRARY
#  HEPMC3_LIBRARIES
#  HEPMC3_LIBRARY_DIRS
#
# Can be steered by HEPMC3_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME hepmc3
   INCLUDE_SUFFIXES include INCLUDE_NAMES HepMC3/HepMC3.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS HepMC3 )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( hepmc3 DEFAULT_MSG HEPMC3_INCLUDE_DIR
   HEPMC3_LIBRARIES )
mark_as_advanced( HEPMC3_FOUND HEPMC3_INCLUDE_DIR HEPMC3_INCLUDE_DIRS
   HEPMC3_LIBRARIES HEPMC3_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( hepmc3 )
