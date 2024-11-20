# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - LIBAIO_FOUND
#  - LIBAIO_INCLUDE_DIRS
#  - LIBAIO_LIBRARY_DIRS
#  - LIBAIO_<component>_FOUND
#  - LIBAIO_<component>_LIBRARY
#  - LIBAIO_LIBRARIES
#
# Can be steered by LIBAIO_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module.
lcg_external_module( NAME libaio
   INCLUDE_SUFFIXES include INCLUDE_NAMES libaio.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS aio )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( libaio DEFAULT_MSG LIBAIO_INCLUDE_DIR
   LIBAIO_LIBRARIES )
mark_as_advanced( LIBAIO_FOUND LIBAIO_INCLUDE_DIR LIBAIO_INCLUDE_DIRS
   LIBAIO_LIBRARY_DIRS )

# Set up the RPM dependency.
lcg_need_rpm( libaio )
