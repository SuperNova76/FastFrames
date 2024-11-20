# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SETUPTOOLS_PYTHON_PATH
#  SETUPTOOLS_BINARY_PATH
#  SETUPTOOLS_easy_install_EXECUTABLE
#
# Can be steered by SETUPTOOLS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
# 'easy_install' is not installed correctly in all LCG releases.
# So we try to find it but don't require the BINARY_PATH.
lcg_python_external_module( NAME setuptools
   PYTHON_NAMES setuptools/__init__.py setuptools.py easy_install.py
   BINARY_NAMES easy_install easy_install.py
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( setuptools DEFAULT_MSG
   _SETUPTOOLS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( setuptools )
