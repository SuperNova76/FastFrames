# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  CHARDET_BINARY_PATH
#  CHARDET_PYTHON_PATH
#  CHARDET_chardetect_EXECUTABLE
#
# Can be steered by CHARDET_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME chardet
   PYTHON_NAMES chardet/__init__.py chardet.py
   BINARY_NAMES chardetect
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( chardet DEFAULT_MSG
   _CHARDET_PYTHON_PATH _CHARDET_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( chardet )
