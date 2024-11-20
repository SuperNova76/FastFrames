# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  STOMPPY_PYTHON_PATH
#
# Can be steered by STOMPPY_LCGROOT.
#
# The LCG include(s).
include( LCGFunctions )
# Find it.
lcg_python_external_module( NAME stomppy
   PYTHON_NAMES stomp/__init__.py )
# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( stomppy DEFAULT_MSG
   _STOMPPY_PYTHON_PATH )
# Set up the RPM dependency.
lcg_need_rpm( stomppy )
