# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  DISTRO_PYTHON_PATH
#
# Can be steered by DISTRO_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME distro
   PYTHON_NAMES distro/__init__.py distro.py )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( distro DEFAULT_MSG
   _DISTRO_PYTHON_PATH )

# Set up the RPM dependency:
lcg_need_rpm( distro )
