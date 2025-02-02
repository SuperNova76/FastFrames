# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration

# Atlas helper cpack script
#set( CPACK_SET_DESTDIR FALSE )
if( CPACK_SOURCE_PACKAGE_FILE_NAME )
   message( STATUS
      "Setting CPACK_INSTALLED_DIRECTORIES to "
      "CPACK_SOURCE_INSTALLED_DIRECTORIES" )
   set( CPACK_INSTALLED_DIRECTORIES "${CPACK_SOURCE_INSTALLED_DIRECTORIES}" )
   set( CPACK_RPM_PACKAGE_ARCHITECTURE
      "${CPACK_SOURCE_RPM_PACKAGE_ARCHITECTURE}" )
   set( CPACK_RPM_PACKAGE_NAME "${CPACK_SOURCE_RPM_PACKAGE_NAME}" )
endif()
