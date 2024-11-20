# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around the nlohmann_jsonConfig.cmake file packaged with the
# nlohmann_json installation itself. (This file is only needed to set up the
# correct RPM dependency.)
#

# The LCG include(s).
include( LCGFunctions )

# Use the helper macro for the wrapping.
lcg_wrap_find_module( nlohmann_json )

# Manually set the nlohmann_json_INCLUDE_DIRS variable, to make the runtime
# environment generation correct.
if( nlohmann_json_FOUND )
   get_target_property( nlohmann_json_INCLUDE_DIRS
      nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES )
endif()

# Set up the RPM dependency.
lcg_need_rpm( jsonmcpp FOUND_NAME nlohmann_json VERSION_NAME JSONMCPP )
