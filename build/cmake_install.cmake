# Install script for directory: /afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/afs/cern.ch/user/n/nsangwen/FastFrames/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.40-acaab/x86_64-el9/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE DIRECTORY FILES "/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/cmake/modules" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE REGEX "/[^/]*\\~$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/cmake/LCGConfig.cmake"
    "/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/cmake/LCGConfig-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so"
         RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so"
         OLD_RPATH "/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/lib:"
         NEW_RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.40-acaab/x86_64-el9/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastFrames.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FastFrames" TYPE FILE FILES
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Binning.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/ConfigDefine.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/ConfigSetting.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/CustomOptions.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Cutflow.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/CutflowContainer.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/DefineHelpers.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/DefineHelpersDict.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/FastFramesExecutor.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/HistoContainer.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Logger.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/MainFrame.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Metadata.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/MetadataManager.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Ntuple.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/ONNXWrapper.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/ObjectCopier.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Region.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Sample.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/SimpleONNXInference.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/StringOperations.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Systematic.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/SystematicReplacer.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Truth.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/UniqueSampleID.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Utils.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/Variable.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/VariableMacros.h"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames/FastFrames/XSectionManager.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so"
         RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/ConfigReaderCpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so"
         OLD_RPATH "/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/lib:/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib:"
         NEW_RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.40-acaab/x86_64-el9/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/ConfigReaderCpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so"
         RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/cppLogger.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.40-acaab/x86_64-el9/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cppLogger.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames/FastFramesTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames/FastFramesTargets.cmake"
         "/afs/cern.ch/user/n/nsangwen/FastFrames/build/CMakeFiles/Export/69e51f99fca035025d2be8e4880940e8/FastFramesTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames/FastFramesTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames/FastFramesTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames" TYPE FILE FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/CMakeFiles/Export/69e51f99fca035025d2be8e4880940e8/FastFramesTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames" TYPE FILE FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/CMakeFiles/Export/69e51f99fca035025d2be8e4880940e8/FastFramesTargets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FastFrames" TYPE FILE FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/FastFramesConfig.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe"
         RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/afs/cern.ch/user/n/nsangwen/FastFrames/build/bin/fast-frames.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe"
         OLD_RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib:/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/lib:"
         NEW_RPATH "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.40-acaab/x86_64-el9/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fast-frames.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libraries" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames_rdict.pcm;/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames.rootmap")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib" TYPE FILE FILES
    "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames_rdict.pcm"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames.rootmap"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib/libFastFrames_rdict.pcm;/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib/libFastFrames.rootmap")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/afs/cern.ch/user/n/nsangwen/FastFrames/install/lib" TYPE FILE FILES
    "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames_rdict.pcm"
    "/afs/cern.ch/user/n/nsangwen/FastFrames/build/lib/libFastFrames.rootmap"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/afs/cern.ch/user/n/nsangwen/FastFrames/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
