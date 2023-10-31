from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SystematicWrapper
from BlockReaderGeneral import BlockReaderGeneral

class BlockReaderSystematic:
    def __init__(self, input_dict : dict, variation_type : str, block_reader_general : BlockReaderGeneral = None):
        self.name = input_dict.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for systematic: " + str(input_dict))
            exit(1)

        self.variation_type = variation_type.lower()
        if self.variation_type not in ["up", "down"]:
            Logger.log_message("ERROR", "Unknown variation type: {}".format(self.variation_type))
            exit(1)

        self.name += "_" + self.variation_type

        self.samples    = input_dict.get("samples",None)
        self.campaigns  = input_dict.get("campaigns",None)
        self.regions    = input_dict.get("regions",None)

        variations_dict = input_dict.get("variation",None)
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

    def adjust_regions(self, regions : dict):
        if self.regions is None: # if no regions are specified, take all regions
            self.regions = []
            for region in regions:
                self.regions.append(region.name)
        else:   # if regions are specified, check if they exist
            for region in self.regions:
                if region not in regions:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self.name))
                    exit(1)


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