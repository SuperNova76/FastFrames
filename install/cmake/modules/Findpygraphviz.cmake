# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYGRRAPHVIZ_PYTHON_PATH
#
# Can be steered by PYGRAPHVIZ_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pygraphviz
   PYTHON_NAMES pygraphviz/__init__.py pygraphviz.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pygraphviz DEFAULT_MSG
   _PYGRAPHVIZ_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pygraphviz )
