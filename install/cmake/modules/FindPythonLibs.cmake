# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(PythonLibs) calls, and
# massage the paths produced by the system module, to make them relocatable.
#
# This module is deprecated, use FindPython instead.
#

# This module requires CMake 3.12.
cmake_minimum_required( VERSION 3.12 )

message( SEND_ERROR "FindPythonLibs is obsolete. Use FindPython instead." )
