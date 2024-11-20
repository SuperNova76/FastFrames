# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SEND2TRASH_PYTHON_PATH
#
# Can be steered by SEND2TRASH_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME send2trash
   PYTHON_NAMES send2trash/__init__.py send2trash.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( send2trash DEFAULT_MSG
   _SEND2TRASH_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( send2trash )
