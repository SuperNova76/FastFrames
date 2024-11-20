# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SINGLEDISPATCH_PYTHON_PATH
#
# Can be steered by SINGLEDISPATCH_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME singledispatch
   PYTHON_NAMES singledispatch/__init__.py singledispatch.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( singledispatch DEFAULT_MSG
   _SINGLEDISPATCH_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( singledispatch )
