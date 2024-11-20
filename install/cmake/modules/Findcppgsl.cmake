# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  CPPGSL_FOUND
#  CPPGSL_INCLUDE_DIR
#  CPPGSL_INCLUDE_DIRS
#
# Can be steered by CPPGSL_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME cppgsl
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES gsl/gsl )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cppgsl DEFAULT_MSG CPPGSL_INCLUDE_DIR )
mark_as_advanced( CPPGSL_FOUND CPPGSL_INCLUDE_DIR CPPGSL_INCLUDE_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( cppgsl )
