# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  GPERFTOOLS_FOUND
#  GPERFTOOLS_INCLUDE_DIRS
#  GPERFTOOLS_<component>_FOUND
#  GPERFTOOLS_<component>_LIBRARY
#  GPERFTOOLS_LIBRARIES
#  GPERFTOOLS_pprof_EXECUTABLE
#  GPERFTOOLS_BINARY_PATH
#
# Can be steered using GPERFTOOLS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the module.
lcg_external_module( NAME gperftools
   INCLUDE_SUFFIXES include INCLUDE_NAMES gperftools/tcmalloc.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS tcmalloc
   BINARY_NAMES pprof
   BINARY_SUFFIXES bin bin64 )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( gperftools DEFAULT_MSG
   GPERFTOOLS_INCLUDE_DIR GPERFTOOLS_INCLUDE_DIRS GPERFTOOLS_LIBRARIES )
mark_as_advanced( GPERFTOOLS_FOUND GPERFTOOLS_INCLUDE_DIR
   GPERFTOOLS_INCLUDE_DIRS GPERFTOOLS_LIBRARIES GPERFTOOLS_LIBRARY_DIRS )

# Set up the RPM dependency.
lcg_need_rpm( gperftools )
