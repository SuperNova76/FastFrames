from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppVariable

class BlockReaderVariable:
    def __init__(self, variable_dict : dict):
        self.name = variable_dict.get("name")
        self.title = variable_dict.get("title", "")
        self.definition = variable_dict.get("definition")
        self.config_reader_cpp_variable = None
        self.__set_config_reader_cpp_variable()
        self.__read_binning(variable_dict.get("binning"))

    def __set_config_reader_cpp_variable(self):
        self.config_reader_cpp_variable = ConfigReaderCppVariable(self.name)
        self.config_reader_cpp_variable.setDefinition(self.definition)
        self.config_reader_cpp_variable.setTitle(self.title)

    def __read_binning(self, binning_dict : dict):
        binning_min = binning_dict.get("min", 0)
        binning_max = binning_dict.get("max", 0)
        binning_nbins = binning_dict.get("number_of_bins", 0)
        binning_bin_edges = binning_dict.get("bin_edges", [])

        regular_binning = binning_min < binning_max and binning_nbins > 0
        if not ((len(binning_bin_edges) != 0)  ^ regular_binning):
            raise ValueError("Could not read the binning, please specify only bin edges, or only range and nbins: " + str(binning_dict))

        if len(binning_bin_edges) != 0:
            bin_edges_str = ",".join([str(x) for x in binning_bin_edges])
            self.config_reader_cpp_variable.setBinningIrregular(bin_edges_str)
        else:
            self.config_reader_cpp_variable.setBinningRegular(binning_min, binning_max, binning_nbins)

