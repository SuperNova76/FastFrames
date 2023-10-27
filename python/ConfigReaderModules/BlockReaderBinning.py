class BlockReaderBinning:
    def __init__(self, binning_dict : dict):
        self.min   = binning_dict.get("min", 0)
        self.max   = binning_dict.get("max", 0)
        self.nbins = binning_dict.get("number_of_bins", 0)
        self.bin_edges = binning_dict.get("bin_edges", [])

        uniform_binning = self.min < self.max and self.nbins > 0
        if not ((len(self.bin_edges) != 0)  ^ uniform_binning):
            raise ValueError("Could not read the binning: " + str(binning_dict))