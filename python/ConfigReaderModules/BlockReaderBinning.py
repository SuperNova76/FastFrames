from BlockReaderCommon import set_path_to_shared_lib
set_path_to_shared_lib()

from ConfigReaderCpp import ConfigReaderCppBinning

class BlockReaderBinning:
    def __init__(self, binning_dict : dict):
        self.min   = binning_dict.get("min", 0)
        self.max   = binning_dict.get("max", 0)
        self.nbins = binning_dict.get("number_of_bins", 0)
        self.bin_edges = binning_dict.get("bin_edges", [])

        uniform_binning = self.min < self.max and self.nbins > 0
        if not ((len(self.bin_edges) != 0)  ^ uniform_binning):
            raise ValueError("Could not read the binning, please specify only bin edges, or only range and nbins: " + str(binning_dict))

        self.config_reader_cpp_binning = None
        self.__set_config_reader_cpp_binning()

    def __set_config_reader_cpp_binning(self):
        self.config_reader_cpp_binning = ConfigReaderCppBinning()
        if len(self.bin_edges) != 0:
            self.config_reader_cpp_binning.setBinningIrregular(str(self.bin_edges))
        else:
            self.config_reader_cpp_binning.setBinningRegular(self.min, self.max, self.nbins)

    def get_bin_edges_from_cpp_part(self):
        str_edges =  self.config_reader_cpp_binning.binEdges()
        return [float(edge) for edge in str_edges.split(",")]