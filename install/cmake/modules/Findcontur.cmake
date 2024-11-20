# Sets:
#  CONTUR_PYTHON_PATH
#  CONTUR_BINARY_PATH
#  CONTUR_contur_EXECUTABLE
#
# Can be steered by CONTUR_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME contur
   PYTHON_NAMES contur/__init__.py contur.py 
   BINARY_NAMES contur contur-plot contur-mkhtml 
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( contur DEFAULT_MSG
   _CONTUR_PYTHON_PATH _CONTUR_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( contur )

