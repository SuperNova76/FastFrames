# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around the CMake configuration that is packaged with the Vc
# installation.
#

# The LCG include(s).
include( LCGFunctions )

# Use the helper macro for the wrapping.
lcg_wrap_find_module( Vc )

# Set up the RPM dependency.
lcg_need_rpm( Vc )
