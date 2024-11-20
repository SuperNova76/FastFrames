# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Module looking for a cuDNN installation.
#
# Sets:
#  - CUDNN_FOUND
#  - CUDNN_INCLUDE_DIR
#  - CUDNN_INCLUDE_DIRS
#  - CUDNN_<component>_LIBRARY
#  - CUDNN_<component>_FOUND
#  - CUDNN_LIBRARIES
#  - CUDNN_LIBRARY_DIRS
#
# The script can be guided using CUDNN_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module.
lcg_external_module( NAME cuDNN
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES cudnn.h cudnn_version.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS cudnn
   SEARCH_PATHS ${CUDAToolkit_LIBRARY_ROOT} )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cuDNN DEFAULT_MSG CUDNN_INCLUDE_DIR
   CUDNN_LIBRARIES )

# Set up the RPM dependency.
lcg_need_rpm( cudnn )
