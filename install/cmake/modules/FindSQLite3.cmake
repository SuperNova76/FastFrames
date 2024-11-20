# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around CMake's built-in FindSQLite3.cmake module. Making sure that
# the found include path and library directory are set up in a relocatable
# way.
#

# The LCG include(s).
include( LCGFunctions )

# Use the helper macro for the wrapping.
lcg_wrap_find_module( SQLite3 NO_LIBRARY_DIRS )

# Add the binary path, 
# to make the `sqlite3` executable accessible at runtime.
# This is important, for example, for GeoModelIO within Athena
find_path( SQLite3_BINARY_PATH
    NAMES sqlite3
    PATH_SUFFIXES bin bin64
    PATHS ${SQLITE_LCGROOT} )

# Set up the RPM dependency.
lcg_need_rpm( sqlite FOUND_NAME SQLite3 VERSION_NAME SQLITE )
