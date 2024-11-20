# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  WIDGETSNBEXTENSION_PYTHON_PATH
#
# Can be steered by WIDGETSNBEXTENSION_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME widgetsnbextension
   PYTHON_NAMES widgetsnbextension/__init__.py widgetsnbextension.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( widgetsnbextension DEFAULT_MSG
   _WIDGETSNBEXTENSION_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( widgetsnbextension )
