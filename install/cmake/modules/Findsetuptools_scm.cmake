# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SETUPTOOLS_SCM_PYTHON_PATH
#
# Can be steered by SETUPTOOLS_SCM_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME setuptools_scm
   PYTHON_NAMES setuptools_scm/__init__.py setuptools_scm.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( setuptools_scm DEFAULT_MSG
   _SETUPTOOLS_SCM_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( setuptools_scm )
