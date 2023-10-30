from BlockReaderCommon import set_path_to_shared_lib
set_path_to_shared_lib()

from BlockReaderBinning import BlockReaderBinning
from ConfigReaderCpp import ConfigReaderCppVariable

class BlockReaderVariable:
    def __init__(self, variable_dict : dict):
        self.name = variable_dict.get("name")
        self.title = variable_dict.get("title", "")
        self.definition = variable_dict.get("definition")
        self.config_reader_cpp_variable = None
        self.__set_config_reader_cpp_variable()

        self.binning = BlockReaderBinning(variable_dict.get("binning"))

    def __set_config_reader_cpp_variable(self):
        self.config_reader_cpp_variable = ConfigReaderCppVariable(self.name)
        self.config_reader_cpp_variable.setDefinition(self.definition)
        self.config_reader_cpp_variable.setTitle(self.title)
        #self.config_reader_cpp_variable.setBinning(self.binning.config_reader_cpp_binning)