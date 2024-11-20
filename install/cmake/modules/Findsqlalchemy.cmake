# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SQLALCHEMY_PYTHON_PATH
#
# Can be steered by SQLALCHEMY_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME sqlalchemy
   PYTHON_NAMES sqlalchemy/__init__.py sqlalchemy.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( sqlalchemy DEFAULT_MSG
   _SQLALCHEMY_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( sqlalchemy )
