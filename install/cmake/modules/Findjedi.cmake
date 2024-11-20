# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  JEDI_PYTHON_PATH
#
# Can be steered by JEDI_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME jedi
   PYTHON_NAMES jedi/__init__.py jedi.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( jedi DEFAULT_MSG
   _JEDI_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( jedi )
