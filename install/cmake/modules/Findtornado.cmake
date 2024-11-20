# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  TORNADO_PYTHON_PATH
#
# Can be steered by TORNADO_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME tornado
   PYTHON_NAMES tornado/__init__.py tornado.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( tornado DEFAULT_MSG
   _TORNADO_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( tornado )
