# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - QD_FOUND
#  - QD_INCLUDE_DIR
#  - QD_INCLUDE_DIRS
#  - QD_<component>_FOUND
#  - QD_<component>_LIBRARY
#  - QD_LIBRARIES
#  - QD_LIBRARY_DIR
#  - QD_LIBRARY_DIRS
#
# The script can be steered by QD_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME qd
   INCLUDE_SUFFIXES include INCLUDE_NAMES qd/qd_config.h
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS qd )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( qd DEFAULT_MSG QD_INCLUDE_DIR
   QD_LIBRARIES )
mark_as_advanced( QD_FOUND QD_INCLUDE_DIR QD_INCLUDE_DIRS
   QD_LIBRARIES QD_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( qd )
