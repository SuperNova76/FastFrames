# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around CMake's built-in FindProtobuf.cmake module. Making sure that
# the found include path and library directory are set up in a relocatable
# way.
#
# In addition to what CMake's module does, it also sets up Protobuf's Python
# bindings for the runtime environment, if those bindings exist.
#
# Sets:
#  PROTOBUF_BINARY_PATH
#  PROTOBUF_PYTHON_PATH
#  All the things provided by CMake's FindProtobuf.cmake module.
#
# Can be steered by PROTOBUF_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# CMake's module tries to execute the protoc executable as part of finding the
# package. To make this possible with an LCG release that has not been set up
# in the runtime environment yet, let's add the path explicitly to CMake's
# environment...
if( IS_DIRECTORY "${PROTOBUF_LCGROOT}/lib" )
   if( NOT "$ENV{LD_LIBRARY_PATH}" MATCHES "${PROTOBUF_LCGROOT}/lib" )
      set( ENV{LD_LIBRARY_PATH}
         "${PROTOBUF_LCGROOT}/lib:$ENV{LD_LIBRARY_PATH}" )
   endif()
   if( NOT "$ENV{DYLD_LIBRARY_PATH}" MATCHES "${PROTOBUF_LCGROOT}/lib" )
      set( ENV{DYLD_LIBRARY_PATH}
         "${PROTOBUF_LCGROOT}/lib:$ENV{DYLD_LIBRARY_PATH}" )
   endif()
endif()

# Use the helper macro for the wrapping CMake's FindProtobuf.cmake module.
lcg_wrap_find_module( Protobuf NO_LIBRARY_DIRS )

# Find the Python binding and the executable's path, if they are available. Note
# however that not finding the binding/executable does not cause the module to
# report not finding Protobuf.
lcg_python_external_module( NAME Protobuf
   PYTHON_NAMES google/protobuf/__init__.py google/protobuf.py
   BINARY_NAMES protoc
   BINARY_SUFFIXES bin )

# Set up the RPM dependency.
lcg_need_rpm( protobuf )
