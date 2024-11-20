# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Module setting up the Python binding coming with HepMC3 for the runtime
# environment.
#
# Sets:
#   PYHEPMC3_PYTHON_PATH
#
# Can be steered by PYHEPMC3_LCGROOT and HEPMC3_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pyHepMC3
   PYTHON_NAMES pyHepMC3/__init__.py pyHepMC3.py
   SEARCH_PATHS ${HEPMC3_LCGROOT} )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pyHepMC3 DEFAULT_MSG
   _PYHEPMC3_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( hepmc3
   FOUND_NAME PYHEPMC3
   VERSION_NAME HEPMC3 )
