# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the SoQt package
# Defines:
#  SOQT_FOUND
#  SOQT_INCLUDE_DIR
#  SOQT_INCLUDE_DIRS
#  SOQT_<component>_FOUND
#  SOQT_<component>_LIBRARY
#  SOQT_LIBRARIES
#  SOQT_LIBRARY_DIRS
#
# Can be steered by SOQT_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME SoQt
   INCLUDE_SUFFIXES include INCLUDE_NAMES Inventor/Qt/SoQt.h
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS SoQt )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( SoQt DEFAULT_MSG SOQT_INCLUDE_DIR
   SOQT_LIBRARIES )
mark_as_advanced( SOQT_FOUND SOQT_INCLUDE_DIR SOQT_INCLUDE_DIRS
   SOQT_LIBRARIES SOQT_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( soqt )
