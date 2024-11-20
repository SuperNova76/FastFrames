# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - MYSQL_FOUND
#  - MYSQL_INCLUDE_DIRS
#  - MYSQL_LIBRARY_DIRS
#  - MYSQL_<component>_FOUND
#  - MYSQL_<component>_LIBRARY
#  - MYSQL_LIBRARIES
#  - MYSQL_mysql_EXECUTABLE
#  - MYSQL_BINARY_PATH
#
# Can be steered by MYSQL_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME mysql
   INCLUDE_SUFFIXES include INCLUDE_NAMES mysql.h mysql/mysql.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS mysqlclient
   BINARY_NAMES mysql
   BINARY_SUFFIXES bin bin64 )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( mysql DEFAULT_MSG MYSQL_INCLUDE_DIR
   MYSQL_LIBRARIES MYSQL_BINARY_PATH )
mark_as_advanced( MYSQL_FOUND MYSQL_INCLUDE_DIR MYSQL_INCLUDE_DIRS
   MYSQL_LIBRARY_DIRS MYSQL_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( mysql )
