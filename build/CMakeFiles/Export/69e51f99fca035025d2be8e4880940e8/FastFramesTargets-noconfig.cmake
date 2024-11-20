#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FastFrames::FastFrames" for configuration ""
set_property(TARGET FastFrames::FastFrames APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FastFrames::FastFrames PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libFastFrames.so"
  IMPORTED_SONAME_NOCONFIG "libFastFrames.so"
  )

list(APPEND _cmake_import_check_targets FastFrames::FastFrames )
list(APPEND _cmake_import_check_files_for_FastFrames::FastFrames "${_IMPORT_PREFIX}/lib/libFastFrames.so" )

# Import target "FastFrames::ConfigReaderCpp" for configuration ""
set_property(TARGET FastFrames::ConfigReaderCpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FastFrames::ConfigReaderCpp PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Python3::Python;Boost::python;FastFrames::FastFrames"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/ConfigReaderCpp.so"
  IMPORTED_SONAME_NOCONFIG "ConfigReaderCpp.so"
  )

list(APPEND _cmake_import_check_targets FastFrames::ConfigReaderCpp )
list(APPEND _cmake_import_check_files_for_FastFrames::ConfigReaderCpp "${_IMPORT_PREFIX}/lib/ConfigReaderCpp.so" )

# Import target "FastFrames::cppLogger" for configuration ""
set_property(TARGET FastFrames::cppLogger APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FastFrames::cppLogger PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Python3::Python"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/cppLogger.so"
  IMPORTED_SONAME_NOCONFIG "cppLogger.so"
  )

list(APPEND _cmake_import_check_targets FastFrames::cppLogger )
list(APPEND _cmake_import_check_files_for_FastFrames::cppLogger "${_IMPORT_PREFIX}/lib/cppLogger.so" )

# Import target "FastFrames::fast-frames.exe" for configuration ""
set_property(TARGET FastFrames::fast-frames.exe APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FastFrames::fast-frames.exe PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/fast-frames.exe"
  )

list(APPEND _cmake_import_check_targets FastFrames::fast-frames.exe )
list(APPEND _cmake_import_check_files_for_FastFrames::fast-frames.exe "${_IMPORT_PREFIX}/bin/fast-frames.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
