# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PROMETHEUS_CLIENT_PYTHON_PATH
#
# Can be steered by PROMETHEUS_CLIENT_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME prometheus_client
   PYTHON_NAMES prometheus_client/__init__.py prometheus_client.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( prometheus_client DEFAULT_MSG
   _PROMETHEUS_CLIENT_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( prometheus_client )
