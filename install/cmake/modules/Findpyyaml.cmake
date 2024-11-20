# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYYAML_PYTHON_PATH
#
# Can be steered by PYYAML_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pyyaml
   PYTHON_NAMES yaml/__init__.py yaml.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pyyaml DEFAULT_MSG
   _PYYAML_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( PyYAML )
