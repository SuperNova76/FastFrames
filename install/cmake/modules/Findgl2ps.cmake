# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - GL2PS_FOUND
#  - GL2PS_INCLUDE_DIRS
#  - GL2PS_LIBRARY_DIRS
#  - GL2PS_LIBRARIES
#  - GL2PS_VERSION
#
# Can be steered by GL2PS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module.
lcg_external_module( NAME gl2ps
   INCLUDE_SUFFIXES include INCLUDE_NAMES gl2ps.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS gl2ps )

# Extract a version for the found path. (Stolen from Kitware's VTK code.)
if( GL2PS_INCLUDE_DIR )
   file( STRINGS "${GL2PS_INCLUDE_DIR}/gl2ps.h" _gl2ps_version_lines
      REGEX "#define[ \t]+GL2PS_(MAJOR|MINOR|PATCH)_VERSION[ \t]+" )
   string( REGEX REPLACE ".*GL2PS_MAJOR_VERSION *\([0-9]*\).*" "\\1"
      _gl2ps_version_major "${_gl2ps_version_lines}" )
   string( REGEX REPLACE ".*GL2PS_MINOR_VERSION *\([0-9]*\).*" "\\1"
      _gl2ps_version_minor "${_gl2ps_version_lines}" )
   string( REGEX REPLACE ".*GL2PS_PATCH_VERSION *\([0-9]*\).*" "\\1"
      _gl2ps_version_patch "${_gl2ps_version_lines}" )
   set( GL2PS_VERSION
      "${_gl2ps_version_major}.${_gl2ps_version_minor}.${_gl2ps_version_patch}" )
   unset(_gl2ps_version_major)
   unset(_gl2ps_version_minor)
   unset(_gl2ps_version_patch)
   unset(_gl2ps_version_lines)
endif()

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( gl2ps
   REQUIRED_VARS GL2PS_INCLUDE_DIR GL2PS_LIBRARIES
   VERSION_VAR GL2PS_VERSION )
mark_as_advanced( GL2PS_FOUND GL2PS_INCLUDE_DIR GL2PS_INCLUDE_DIRS
   GL2PS_LIBRARY_DIRS )

# Set up the RPM dependency.
lcg_need_rpm( gl2ps )
