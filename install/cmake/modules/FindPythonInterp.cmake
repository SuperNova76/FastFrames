# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(PythonInterp) calls, and extend
# the environment setup file of the project with the correct Python paths.
#
# This module is deprecated, use FindPython instead.
#

# This module requires CMake 3.12.
cmake_minimum_required( VERSION 3.12 )

message( SEND_ERROR "FindPythonInterp is obsolete. Use FindPython instead." )
