# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PBR_PYTHON_PATH
#  PBR_BINARY_PATH
#  PBR_pbr_EXECUTABLE
#
# Can be steered by PBR_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pbr
   PYTHON_NAMES pbr/__init__.py pbr.py
   BINARY_NAMES pbr
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pbr DEFAULT_MSG
   _PBR_PYTHON_PATH _PBR_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pbr )
