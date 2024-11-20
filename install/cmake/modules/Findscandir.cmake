# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SCANDIR_PYTHON_PATH
#
# Can be steered by SCANDIR_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME scandir
   PYTHON_NAMES scandir/__init__.py scandir.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( scandir DEFAULT_MSG
   _SCANDIR_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( scandir )
