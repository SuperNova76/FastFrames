# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  ATOMICWRITES_PYTHON_PATH
#
# Can be steered by ATOMICWRITES_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME atomicwrites
   PYTHON_NAMES atomicwrites/__init__.py atomicwrites.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( atomicwrites DEFAULT_MSG
   _ATOMICWRITES_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( atomicwrites )
