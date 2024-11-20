# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  FUNCSIGS_PYTHON_PATH
#
# Can be steered by FUNCSIGS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME funcsigs
   PYTHON_NAMES funcsigs/__init__.py funcsigs.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( funcsigs DEFAULT_MSG
   _FUNCSIGS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( funcsigs )
