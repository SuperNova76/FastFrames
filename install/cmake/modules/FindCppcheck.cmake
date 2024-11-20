# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Locate cppcheck exectuable
#
# Sets:
#  CPPCHECK_cppcheck_EXECUTABLE
#

include( LCGFunctions )

# Find the cppcheck executable:
lcg_external_module( NAME Cppcheck
   BINARY_NAMES cppcheck
   BINARY_SUFFIXES bin )

# Extract version:
if( CPPCHECK_cppcheck_EXECUTABLE )
   execute_process( COMMAND "${CPPCHECK_cppcheck_EXECUTABLE}" --version
      OUTPUT_VARIABLE CPPCHECK_VERSION_OUTPUT
      OUTPUT_STRIP_TRAILING_WHITESPACE )
   string( REGEX REPLACE "^Cppcheck (.*)" "\\1"
      CPPCHECK_VERSION "${CPPCHECK_VERSION_OUTPUT}" )
   unset( CPPCHECK_VERSION_OUTPUT )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Cppcheck
   FOUND_VAR CPPCHECK_FOUND
   REQUIRED_VARS CPPCHECK_cppcheck_EXECUTABLE
   VERSION_VAR CPPCHECK_VERSION )
