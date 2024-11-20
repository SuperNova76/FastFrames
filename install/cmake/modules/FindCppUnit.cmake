# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# - Locate CppUnit library
# Defines:
#
#  CPPUNIT_FOUND
#  CPPUNIT_INCLUDE_DIR
#  CPPUNIT_INCLUDE_DIRS
#  CPPUNIT_LIBRARY
#  CPPUNIT_LIBRARIES
#  CPPUNIT_LIBRARY_DIR
#  CPPUNIT_LIBRARY_DIRS
#
# Can be steered by CPPUNIT_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME CppUnit
   INCLUDE_SUFFIXES include INCLUDE_NAMES cppunit/Test.h
   LIBRARY_SUFFIXES lib COMPULSORY_COMPONENTS cppunit )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CppUnit DEFAULT_MSG CPPUNIT_INCLUDE_DIR
   CPPUNIT_LIBRARIES )
mark_as_advanced( CPPUNIT_FOUND CPPUNIT_INCLUDE_DIR CPPUNIT_INCLUDE_DIRS
   CPPUNIT_LIBRARIES CPPUNIT_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( CppUnit )
