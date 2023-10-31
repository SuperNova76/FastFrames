from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp import ConfigReaderCppVariable
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderVariable:
    def __init__(self, variable_dict : dict):
        self.options_getter = BlockOptionsGetter(variable_dict)
        self.name = self.options_getter.get("name")
        self.title = self.options_getter.get("title", "")
        self.definition = self.options_getter.get("definition")
        self.cpp_class = None
        self.__set_cpp_class()
        self.__read_binning(self.options_getter.get("binning"))
        self._check_unused_options()

    def __set_cpp_class(self):
        self.cpp_class = ConfigReaderCppVariable(self.name)
        self.cpp_class.setDefinition(self.definition)
        self.cpp_class.setTitle(self.title)

    def __read_binning(self, binning_dict : dict):
        binning_options_getter = BlockOptionsGetter(binning_dict)
        binning_min = binning_options_getter.get("min", 0)
        binning_max = binning_options_getter.get("max", 0)
        binning_nbins = binning_options_getter.get("number_of_bins", 0)
        binning_bin_edges = binning_options_getter.get("bin_edges", [])

        unused = binning_options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("WARNING", "Key {} used in region block is not supported!".format(unused))

        regular_binning = binning_min < binning_max and binning_nbins > 0
        if not ((len(binning_bin_edges) != 0)  ^ regular_binning):
            raise ValueError("Could not read the binning, please specify only bin edges, or only range and nbins: " + str(binning_dict))

        if len(binning_bin_edges) != 0:
            bin_edges_str = ",".join([str(x) for x in binning_bin_edges])
            self.cpp_class.setBinningIrregular(bin_edges_str)
        else:
            self.cpp_class.setBinningRegular(binning_min, binning_max, binning_nbins)

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("WARNING", "Key {} used in region block is not supported!".format(unused))
