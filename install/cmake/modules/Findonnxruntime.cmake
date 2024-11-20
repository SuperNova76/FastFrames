# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
#
# Locate the onnxruntime external package.
#
# Defines:
#  ONNXRUNTIME_FOUND
#  ONNXRUNTIME_INCLUDE_DIR
#  ONNXRUNTIME_INCLUDE_DIRS
#  ONNXRUNTIME_LIBRARIES
#  ONNXRUNTIME_LIBRARY_DIRS
#

# Include the helper code:
include( AtlasInternals )

# Declare the module:
atlas_external_module( NAME onnxruntime
   INCLUDE_SUFFIXES include include/onnxruntime
   INCLUDE_NAMES core/common/version.h
                 core/session/onnxruntime_c_api.h
                 onnxruntime_c_api.h
                 onnxruntime_cxx_api.h
   LIBRARY_SUFFIXES lib lib32 lib64
   COMPULSORY_COMPONENTS onnxruntime )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( onnxruntime DEFAULT_MSG ONNXRUNTIME_INCLUDE_DIR
   ONNXRUNTIME_LIBRARIES )
mark_as_advanced( ONNXRUNTIME_FOUND ONNXRUNTIME_INCLUDE_DIR ONNXRUNTIME_INCLUDE_DIRS
   ONNXRUNTIME_LIBRARIES ONNXRUNTIME_LIBRARY_DIRS )
