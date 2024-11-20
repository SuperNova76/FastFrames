# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# - Find QMTest.
#
# This module will define the following variables:
#  QMTEST_EXECUTABLE  - the qmtest main script
#  QMTEST_PYTHON_PATH - directory containing the Python module 'qm'
#  QMTEST_BINARY_PATH - directory with the qmtest executable
#  QMTEST_PREFIX_PATH - directory holding QMTest on the whole
#  QMTEST_qmtest_EXECUTABLE - the main executable
#
# Can be steered by QMTEST_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME QMTest
   PYTHON_NAMES qm/__init__.py qm.py
   PYTHON_SUFFIXES Lib/site-packages
   BINARY_NAMES qmtest
   BINARY_SUFFIXES bin Scripts )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( QMTest DEFAULT_MSG
   _QMTEST_PYTHON_PATH _QMTEST_BINARY_PATH )

# Set up the QMTEST_EXECUTABLE variable.
if( QMTEST_FOUND )
   set( QMTEST_EXECUTABLE "${_QMTEST_BINARY_PATH}/qmtest" )
endif()

# Set up the RPM dependency:
lcg_need_rpm( QMtest )
