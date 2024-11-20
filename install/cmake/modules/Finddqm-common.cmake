# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Legacy module, to keep old Athena configurations working.
#

# Tell the user that dqm-common is not available.
set( DQM-COMMON_FOUND FALSE )
message( WARNING "find_package(dqm-common) should not be used anymore!" )
