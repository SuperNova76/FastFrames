# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  PPROF_BINARY_PATH
#  PPROF_pprof_EXECUTABLE
#
# Can be steered using PPROF_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the module.
lcg_external_module( NAME pprof
  BINARY_SUFFIXES bin
  BINARY_NAMES pprof )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pprof DEFAULT_MSG
  PPROF_pprof_EXECUTABLE )

# Set up the RPM dependency.
lcg_need_rpm( pprof )
