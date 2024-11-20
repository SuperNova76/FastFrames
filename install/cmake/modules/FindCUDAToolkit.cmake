# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around CMake's built-in FindCUDAToolkit.cmake module. Making sure that
# the found include path and library directory are set up in a relocatable
# way.
#

# Minimum CMake requirement.
cmake_minimum_required( VERSION 3.17 )

# The LCG include(s).
include( LCGFunctions )

# Use the helper macro for the wrapping.
lcg_wrap_find_module( CUDAToolkit )

# Make the CUDA binaries available in the runtime environment.
set( CUDAToolkit_BINARY_PATH "${CUDAToolkit_BIN_DIR}" )

# Set up a default value for the CUDACXX environment variable.
set( CUDAToolkit_ENVIRONMENT
   SET CUDACXX "${CUDAToolkit_NVCC_EXECUTABLE}" )

# Set up the RPM dependency.
lcg_need_rpm( cuda FOUND_NAME CUDAToolkit VERSION_NAME CUDA )
