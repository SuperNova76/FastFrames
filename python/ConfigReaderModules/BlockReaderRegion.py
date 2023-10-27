from BlockReaderCommon import set_path_to_shared_lib
set_path_to_shared_lib()

from ConfigReaderCpp import ConfigReaderCppRegion

from BlockReaderVariable import BlockReaderVariable
from BlockReaderGeneral import BlockReaderGeneral


class BlockReaderRegion:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.name = input_dict.get("name")
        self.selection = input_dict.get("selection")
        self.variables = []

        if block_reader_general is not None:
            self.__merge_settings(block_reader_general)

        for variable in input_dict.get("variables"):
            self.variables.append(BlockReaderVariable(variable))

        self.__set_config_reader_cpp_region = None
        self.__set_config_reader_cpp()


    def __set_config_reader_cpp(self):
        self.config_reader_cpp_region = ConfigReaderCppRegion(self.name)
        self.config_reader_cpp_region.setSelection(self.selection)

    def __merge_settings(self, block_reader_general) -> list:
        if block_reader_general is None:
            return

        # if in future we need to merge settings from general block to region block, it will be here