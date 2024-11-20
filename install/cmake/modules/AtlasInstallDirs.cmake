# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# This file sets up a number of cache variables that are used to set up the
# installation locations for a range of different files in a project. It is
# used similar to CMake's own GNUInstallDirs. And in fact, it largely depends
# on that helper module.
#

# Set the directories that GNUInstallDirs either doesn't include, or which
# we want to override from their GNUInstallDirs default values.
set( CMAKE_INSTALL_LIBDIR "lib" CACHE PATH
   "Library installation directory" )
set( CMAKE_INSTALL_INCDIR "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/include"
   CACHE PATH "Header installation directory" )
set( CMAKE_INSTALL_SRCDIR "src" CACHE PATH
   "Package source installation directory" )
set( CMAKE_INSTALL_PYTHONDIR "python" CACHE PATH
   "Python installation directory" )
set( CMAKE_INSTALL_DATADIR "data" CACHE PATH
   "Data installation directory" )
set( CMAKE_INSTALL_SCRIPTDIR "scripts" CACHE PATH
   "Script installation directory" )
set( CMAKE_INSTALL_JOBOPTDIR "jobOptions" CACHE PATH
   "JobOption installation directory" )
set( CMAKE_INSTALL_XMLDIR "XML" CACHE PATH
   "XML installation directory" )
set( CMAKE_INSTALL_SHAREDIR "share" CACHE PATH
   "Shared data file installation directory" )
set( CMAKE_INSTALL_DOCDIR "doc" CACHE PATH
   "Documentation file installation directory" )
set( CMAKE_INSTALL_CMAKEDIR "cmake" CACHE PATH
   "CMake file installation directory" )

# Now include GNUInstallDirs to set up the rest.
include( GNUInstallDirs )
