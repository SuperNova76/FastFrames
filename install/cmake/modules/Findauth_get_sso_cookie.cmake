# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  AUTH_GET_SSO_COOKIE_BINARY_PATH
#  AUTH_GET_SSO_COOKIE_PYTHON_PATH
#  AUTH_GET_SSO_COOKIE_auth-get-sso-cookie_EXECUTABLE
#
# Can be steered by AUTH_GET_SSO_COOKIE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module:
lcg_python_external_module( NAME auth_get_sso_cookie
   PYTHON_NAMES auth_get_sso_cookie/__init__.py auth_get_sso_cookie.py
   BINARY_NAMES auth-get-sso-cookie
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( auth_get_sso_cookie DEFAULT_MSG
   _AUTH_GET_SSO_COOKIE_PYTHON_PATH _AUTH_GET_SSO_COOKIE_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( auth_get_sso_cookie )
