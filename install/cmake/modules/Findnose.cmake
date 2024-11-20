# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  NOSE_PYTHON_PATH
#  NOSE_BINARY_PATH
#  NOSE_nosetests_EXECUTABLE
#
# Can be steered by NOSE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME nose
   PYTHON_NAMES nose/__init__.py nose.py
   BINARY_NAMES nosetests
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( nose DEFAULT_MSG
   _NOSE_BINARY_PATH _NOSE_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( nose )
