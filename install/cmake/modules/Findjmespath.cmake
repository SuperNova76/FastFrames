# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  JMESPATH_PYTHON_PATH
#
# Can be steered by JMESPATH_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME jmespath
   PYTHON_NAMES jmespath/__init__.py jmespath.py 
   BINARY_NAMES jp.py
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( jmespath DEFAULT_MSG
   _JMESPATH_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( jmespath )
