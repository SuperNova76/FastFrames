from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SystematicWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderSystematic:
    def __init__(self, input_dict : dict, variation_type : str, block_reader_general : BlockReaderGeneral = None):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.name = self.options_getter.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for systematic: " + str(self.options_getter))
            exit(1)

        self.variation_type = variation_type.lower()
        if self.variation_type not in ["up", "down"]:
            Logger.log_message("ERROR", "Unknown variation type: {}".format(self.variation_type))
            exit(1)

        self.name += "_" + self.variation_type

        self.samples    = self.options_getter.get("samples",None)
        self.campaigns  = self.options_getter.get("campaigns",None)
        self.regions    = self.options_getter.get("regions",None)

        variations_dict = self.options_getter.get("variation",None)
        if variations_dict is None:
            Logger.log_message("ERROR", "No variations specified for systematic {}".format(self.name))
            exit(1)

        self.replacement_string = variations_dict.get(self.variation_type,None)
        if self.replacement_string is None:
            Logger.log_message("ERROR", "{} variation specified for systematic {}".format(self.variation_type, self.name))
            exit(1)

        self.sum_weights = variations_dict.get("sum_weights_" + self.replacement_string,None)
        if self.sum_weights is None:
            self.sum_weights = block_reader_general.default_sumweights

        self._check_unused_options()

    def adjust_regions(self, regions : dict) -> None:
        if self.regions is None: # if no regions are specified, take all regions
            self.regions = []
            for region in regions:
                self.regions.append(region.name)
        else:   # if regions are specified, check if they exist
            for region in self.regions:
                if region not in regions:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self.name))
                    exit(1)

    def check_samples_existence(self, sample_dict : dict) -> None:
        """
        Check if all samples specified for the systematic exist
        """
        for sample in self.samples:
            if sample not in sample_dict:
                Logger.log_message("ERROR", "Sample {} specified for systematic {} does not exist".format(sample, self.name))
                exit(1)

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("WARNING", "Key {} used in region block is not supported!".format(unused))

def read_systematics_variations(input_dict : dict, block_reader_general : BlockReaderGeneral = None) -> list:
    """
    Read list of systematic uncertainties from the input dictionary read from config. The result might have 1 (only up or only down) or 2 inputs (both up and down variations)
    """
    variations_dict = input_dict.get("variation",None)
    variations = []
    if "up" in variations_dict:
        variations.append("up")
    if "down" in variations_dict:
        variations.append("down")
    if len(variations) == 0:
        Logger.log_message("ERROR", "No variations specified for systematic {}".format(input_dict["name"]))
        exit(1)

    result = []
    for variation in variations:
        result.append(BlockReaderSystematic(input_dict, variation, block_reader_general))
    return result