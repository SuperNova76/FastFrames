from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp import ConfigReaderCppVariable
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderVariable:
    def __init__(self, variable_dict : dict):
        self.options_getter = BlockOptionsGetter(variable_dict)
        self.name = self.options_getter.get("name", None, [str])
        self.title = self.options_getter.get("title", "", [str])
        self.definition = self.options_getter.get("definition", None, [str])
        self.cpp_class = None
        self.__set_cpp_class()
        self.__read_binning(self.options_getter.get("binning", None, [dict]))
        self._check_unused_options()

    def __set_cpp_class(self):
        self.cpp_class = ConfigReaderCppVariable(self.name)
        self.cpp_class.setDefinition(self.definition)
        self.cpp_class.setTitle(self.title)

    def __read_binning(self, binning_dict : dict):
        if binning_dict is None:
            Logger.log_message("ERROR", "No binning specified for variable {}".format(self.name))
            exit(1)

        binning_options_getter = BlockOptionsGetter(binning_dict)
        binning_min = binning_options_getter.get("min", 0, [int, float]  )
        binning_max = binning_options_getter.get("max", 0, [int, float])
        binning_nbins = binning_options_getter.get("number_of_bins", 0, [int])
        binning_bin_edges = binning_options_getter.get("bin_edges", [], [list])

        unused = binning_options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("WARNING", "Key {} used in region block is not supported!".format(unused))

        regular_binning = binning_min < binning_max and binning_nbins > 0
        if not ((len(binning_bin_edges) != 0)  ^ regular_binning):
            Logger.log_message("ERROR","Could not read the binning, please specify only bin edges, or only range and nbins: " + str(binning_dict))
            exit(1)

        if len(binning_bin_edges) != 0:
            if len(binning_bin_edges) < 2:
                Logger.log_message("ERROR", "Binning for variable {} has less than 2 bin edges".format(self.name))
                exit(1)
            bin_edges_str = ",".join([str(x) for x in binning_bin_edges])
            self.cpp_class.setBinningIrregular(bin_edges_str)
        else:
            if binning_nbins < 1:
                Logger.log_message("ERROR", "Binning for variable {} has less than 1 bin".format(self.name))
                exit(1)
            if binning_min >= binning_max:
                Logger.log_message("ERROR", "Binning for variable {} has min >= max".format(self.name))
                exit(1)
            self.cpp_class.setBinningRegular(binning_min, binning_max, binning_nbins)

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in variable block is not supported!".format(unused))
            exit(1)
