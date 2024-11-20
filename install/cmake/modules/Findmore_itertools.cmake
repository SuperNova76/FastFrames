# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MORE_ITERTOOLS_PYTHON_PATH
#
# Can be steered by MORE_ITERTOOLS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME more_itertools
   PYTHON_NAMES more_itertools/__init__.py more_itertools.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( more_itertools DEFAULT_MSG
   _MORE_ITERTOOLS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( more_itertools )
