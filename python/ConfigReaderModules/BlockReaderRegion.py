from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppRegion

from BlockReaderVariable import BlockReaderVariable
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter
from python_wrapper.python.logger import Logger


class BlockReaderRegion:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.options_getter = BlockOptionsGetter(input_dict)
        self.name = self.options_getter.get("name")
        self.selection = self.options_getter.get("selection")
        self.variables = []

        if block_reader_general is not None:
            self.__merge_settings(block_reader_general)

        for variable_dict in self.options_getter.get("variables"):
            variable = BlockReaderVariable(variable_dict)
            self.variables.append(variable)

        self.__set_cpp_class = None
        self.__set_config_reader_cpp()
        self._check_unused_options()

    def __set_config_reader_cpp(self):
        self.cpp_class = ConfigReaderCppRegion(self.name)
        self.cpp_class.setSelection(self.selection)
        for variable in self.variables:
            ptr = variable.cpp_class.getPtr()
            self.cpp_class.addVariable(variable.cpp_class.getPtr())

    def __merge_settings(self, block_reader_general) -> list:
        if block_reader_general is None:
            return

        # if in future we need to merge settings from general block to region block, it will be here

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in region block is not supported!".format(unused))
            exit(1)