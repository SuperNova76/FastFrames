"""
@file Source file with BlockReaderRegion class.
"""
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import RegionWrapper, VariableWrapper

from BlockReaderVariable import BlockReaderVariable
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter
from python_wrapper.python.logger import Logger


class BlockReaderRegion:
    """!Class for reading region block of the config. Equivalent of C++ class Region
    """

    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        """
        Constructor of the BlockReaderRegion class
        @param input_dict: dictionary with options from the config file
        @param block_reader_general: BlockReaderGeneral object with general options from the config file - this is there to get default values
        """
        self.options_getter = BlockOptionsGetter(input_dict)
        self.name = self.options_getter.get("name", None, [str])
        self.selection = self.options_getter.get("selection", None, [str])
        self.variables = []

        if block_reader_general is not None:
            self.__merge_settings(block_reader_general)

        for variable_dict in self.options_getter.get("variables", [], [list]):
            variable = BlockReaderVariable(variable_dict)
            self.variables.append(variable)

        self.histograms_2d = self.options_getter.get("histograms_2d", [], [list])

        self.__set_cpp_class = None
        self.__set_config_reader_cpp()
        self._check_unused_options()

    def __set_config_reader_cpp(self):
        variables_names = []
        self.cpp_class = RegionWrapper(self.name)
        self.cpp_class.setSelection(self.selection)
        for variable in self.variables:
            ptr = variable.cpp_class.getPtr()
            self.cpp_class.addVariable(variable.cpp_class.getPtr())
            variables_names.append(variable.cpp_class.name())

        for histogram_2d in self.histograms_2d:
            options_getter = BlockOptionsGetter(histogram_2d)
            x = options_getter.get("x", None, [str])
            y = options_getter.get("y", None, [str])
            if x is None or y is None:
                Logger.log_message("ERROR", "histograms_2d in region {} does not have x or y specified".format(self.name))
                exit(1)
            if x not in variables_names:
                Logger.log_message("ERROR", "histograms_2d in region {} has x variable {} which is not defined".format(self.name, x))
                exit(1)
            if y not in variables_names:
                Logger.log_message("ERROR", "histograms_2d in region {} has y variable {} which is not defined".format(self.name, y))
                exit(1)
            self.cpp_class.addVariableCombination(x, y)

    def __merge_settings(self, block_reader_general) -> list:
        if block_reader_general is None:
            return

        # if in future we need to merge settings from general block to region block, it will be here

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in region block is not supported!".format(unused))
            exit(1)

    def get_variable_cpp_objects(self) -> list:
        """
        Get list of variables (cpp objects) defined in the region
        """
        variable_ptrs = self.cpp_class.getVariableRawPtrs()
        result = []
        for variable_ptr in variable_ptrs:
            variable_cpp_object = VariableWrapper("")
            variable_cpp_object.constructFromRawPtr(variable_ptr)
            result.append(variable_cpp_object)
        return result

    def get_2d_combinations(self) -> list:
        """
        Get list of 2D variable combinations defined in the region
        """
        """
        result = []
        vector_combinations = self.cpp_class.variableCombinations()
        for combination in vector_combinations:
            result.append(combination)
        return result
