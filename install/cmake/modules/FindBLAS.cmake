# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around CMake's built-in FindBLAS.cmake module. Making sure that
# the found include path and library directory are set up in a relocatable
# way.
#

# The LCG include(s):
include( LCGFunctions )

# Hide possible messages about OpenMP not being found. See AGENE-2118.
set( OpenMP_FIND_QUIETLY TRUE )

# Use the helper macro for the wrapping:
lcg_wrap_find_module( BLAS NO_LIBRARY_DIRS )

# Set back the default behaviour of the OpenMP output visibility.
unset( OpenMP_FIND_QUIETLY )

# Set up the RPM dependency:
lcg_need_rpm( blas )
