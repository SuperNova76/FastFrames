# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  JSONSCHEMA_PYTHON_PATH
#  JSONSCHEMA_BINARY_PATH
#  JSONSCHEMA_jsonschema_EXECUTABLE
#
# Can be steered by JSONSCHEMA_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME jsonschema
   PYTHON_NAMES jsonschema/__init__.py jsonschema.py
   BINARY_NAMES jsonschema
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( jsonschema DEFAULT_MSG
   _JSONSCHEMA_PYTHON_PATH _JSONSCHEMA_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( jsonschema )
