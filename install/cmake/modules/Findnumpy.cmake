# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  NUMPY_PYTHON_PATH
#  NUMPY_BINARY_PATH
#  NUMPY_f2py_EXECUTABLE
#
# Can be steered by NUMPY_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME numpy
   PYTHON_NAMES numpy/__init__.py numpy.py
   BINARY_NAMES f2py f2py3
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( numpy DEFAULT_MSG
   _NUMPY_PYTHON_PATH _NUMPY_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( numpy )
