# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# - Locate FastJet library and header files
# Defines:
#
#  FASTJET_FOUND
#  FASTJET_INCLUDE_DIR
#  FASTJET_INCLUDE_DIRS
#  FASTJET_<component>_FOUND
#  FASTJET_<component>_LIBRARY
#  FASTJET_LIBRARIES
#  FASTJET_LIBRARY_DIRS
#  FASTJET_CONFIG_SCRIPT
#
# Can be steered by FASTJET_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME FastJet
   INCLUDE_SUFFIXES include INCLUDE_NAMES fastjet/version.hh
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS fastjet fastjetplugins fastjettools siscone
   siscone_spherical
   COMPULSORY_COMPONENTS fastjet )

# If a fortran compiler is available, and it's gfortran, then link
# against the gfortran library as well. Since FastJet may or may not
# use Fortran code in such a setup. If it doesn't, -Wl,--only-needed
# should take care of removing the dependency during linking.
if( FASTJET_LIBRARIES AND ( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU" ) )
   list( APPEND FASTJET_LIBRARIES gfortran )
endif()

# Find the fastjet-config script.
find_program( FASTJET_CONFIG_SCRIPT
   NAMES fastjet-config
   PATHS ${FASTJET_LCGROOT} PATH_SUFFIXES bin bin32 bin64
   DOC "Path to the fastjet-config script to use" )

# Extract the version of FastJet from the script.
if( FASTJET_CONFIG_SCRIPT )
   execute_process( COMMAND ${FASTJET_CONFIG_SCRIPT} --version
      OUTPUT_VARIABLE FASTJET_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( FastJet
   FOUND_VAR FASTJET_FOUND
   REQUIRED_VARS FASTJET_CONFIG_SCRIPT FASTJET_INCLUDE_DIR FASTJET_LIBRARIES
   VERSION_VAR FASTJET_VERSION )
mark_as_advanced( FASTJET_FOUND FASTJET_INCLUDE_DIR FASTJET_INCLUDE_DIRS
   FASTJET_LIBRARIES FASTJET_LIBRARY_DIRS FASTJET_CONFIG_SCRIPT )

# Set up the RPM dependency:
lcg_need_rpm( fastjet )
