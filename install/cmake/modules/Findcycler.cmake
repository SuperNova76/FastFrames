# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#   CYCLER_PYTHON_PATH
#
# Can be steered by CYCLER_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME cycler
   PYTHON_NAMES cycler.py cycler/__init__.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cycler DEFAULT_MSG
   _CYCLER_PYTHON_PATH )

# Set up the RPM dependency:
lcg_need_rpm( cycler )
