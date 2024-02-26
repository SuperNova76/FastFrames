"""
@file Source file with BlockReaderSystematic class and read_systematics_variations function.
"""
from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SystematicWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter, VariationsOptionsGetter
from CommandLineOptions import CommandLineOptions

class BlockReaderSystematic:
    """!Class for reading systematic block from config file, equivalent of C++ class Systematic
    """

    def __init__(self, input_dict : dict, variation_type : str, block_reader_general : BlockReaderGeneral = None, replacement_tuple : tuple = None):
        """!Constructor of the BlockReaderSystematic class
        @param input_dict: dictionary with options from the config file
        @param variation_type: type of variation (up or down)
        @param block_reader_general: BlockReaderGeneral object with general options from the config file - this is there to get default values
        """
        self._options_getter = BlockOptionsGetter(input_dict)

        variations_dict = self._options_getter.get("variation",None, [dict])
        variations_opts_getter = VariationsOptionsGetter(variations_dict)
        if variations_dict is None:
            Logger.log_message("ERROR", "No variations specified for systematic {}".format(self._options_getter))
            exit(1)

        self._variation_type = variation_type.lower()
        if self._variation_type not in ["up", "down"]:
            Logger.log_message("ERROR", "Unknown variation type: {}".format(self._variation_type))
            exit(1)

        self._name = variations_opts_getter.get("", self._variation_type,None, [str])
        if self._name is None:
            Logger.log_message("ERROR", "{} variation specified for systematic {}".format(self._variation_type, self._options_getter))
            exit(1)

        self._weight_suffix = variations_opts_getter.get("weight_suffix", self._variation_type,"", [str])

        self._sum_weights = variations_opts_getter.get("sum_weights", self._variation_type,None, [str])
        if self._sum_weights is None:
            self._sum_weights = block_reader_general.default_sumweights

        if replacement_tuple is not None:
            self._name          = self._name.replace(replacement_tuple[0], replacement_tuple[1])
            self._weight_suffix = self._weight_suffix.replace(replacement_tuple[0], replacement_tuple[1])
            self._sum_weights   = self._sum_weights.replace(replacement_tuple[0], replacement_tuple[1])
            self._options_getter.get("numbering_sequence")

        BlockReaderSystematic._check_unused_variation_options(variations_opts_getter)

        self.samples         = self._options_getter.get("samples",None, [list], [str])
        self.exclude_samples = self._options_getter.get("exclude_samples",None, [list], [str])
        CommandLineOptions().keep_only_selected_samples(self.samples)
        CommandLineOptions().keep_only_selected_samples(self.exclude_samples)
        if not self.samples is None and not self.exclude_samples is None:
            Logger.log_message("ERROR", "Both samples and exclude_samples specified for systematic {}".format(self._name))
            exit(1)


        self._campaigns  = self._options_getter.get("campaigns",None, [list], [str])

        self._regions           = self._options_getter.get("regions",None, [list], [str])
        self._exclude_regions   = self._options_getter.get("exclude_regions",None, [list], [str])
        if not self._regions is None and not self._exclude_regions is None:
            Logger.log_message("ERROR", "Both regions and exclude_regions specified for systematic {}".format(self._name))
            exit(1)

        self._check_unused_options()

        ## Instance of the SystematicWrapper C++ class -> wrapper around C++ Systematic class
        self.cpp_class = SystematicWrapper(self._name)

        self.cpp_class.setSumWeights(self._sum_weights)
        self.cpp_class.setWeightSuffix(self._weight_suffix)

    def adjust_regions(self, regions : dict) -> None:
        """!Resolve list of regions where the systematic should be used. If regions are specified, check if they exist. If no regions are specified, take all regions. If exclude_regions is specified, remove them from the list of regions.
        @param regions: dictionary with all regions (keys are region names, values are BlockReaderRegion objects)
        """
        if self._regions is None: # if no regions are specified, take all regions
            self._regions = []
            for region_name in regions:
                if self._exclude_regions is None or region_name not in self._exclude_regions:
                    self._regions.append(region_name)
        else:   # if regions are specified, check if they exist
            for region in self._regions:
                if region not in regions:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self._name))
                    exit(1)

        for region_name in self._regions:
            self.cpp_class.addRegion(regions[region_name].cpp_class.getPtr())

    def check_samples_existence(self, sample_dict : dict) -> None:
        """!Check if all samples specified for the systematic exist
        @param sample_dict: dictionary with all samples (keys are sample names)
        """
        if not self.samples is None:
            for sample in self.samples:
                if sample not in sample_dict:
                    Logger.log_message("ERROR", "Sample {} specified for systematic {} does not exist".format(sample, self._name))
                    exit(1)
        if not self.exclude_samples is None:
            for sample in self.exclude_samples:
                if sample not in sample_dict:
                    Logger.log_message("ERROR", "Sample {} specified for systematic {} does not exist".format(sample, self._name))
                    exit(1)

    def check_regions_existence(self, region_dict : dict) -> None:
        """!Check if all regions specified for the systematic exist
        @param region_dict: dictionary with all regions (keys are region names)
        """
        if not self._regions is None:
            for region in self._regions:
                if region not in region_dict:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self._name))
                    exit(1)
        if not self._exclude_regions is None:
            for region in self._exclude_regions:
                if region not in region_dict:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self._name))
                    exit(1)

    def get_regions_names(self) -> list:
        """!Get list of regions where the systematic should be used
        """
        return [x for x in self.cpp_class.regionsNames()]

    def _check_unused_variation_options(variations_opts_getter : VariationsOptionsGetter) -> None:
        unused = variations_opts_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in systematic variation block is not supported!".format(unused))
            exit(1)

    def _check_unused_options(self):
        unused = self._options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in systematic block is not supported!".format(unused))
            exit(1)

def read_systematics_variations(input_dict : dict, block_reader_general : BlockReaderGeneral = None, systematics_pairs_list : list[dict] = None) -> list:
    """!Read list of systematic uncertainties from the input dictionary read from config. The result might have 1 (only up or only down) or 2 inputs (both up and down variations)
    """
    input_dict_getter = BlockOptionsGetter(input_dict)
    variations_dict = input_dict_getter.get("variation",None)
    variations = []
    if "up" in variations_dict:
        variations.append("up")
    if "down" in variations_dict:
        variations.append("down")
    if len(variations) == 0:
        Logger.log_message("ERROR", "No variations specified for systematic {}".format(input_dict_getter["name"]))
        exit(1)

    result = []
    dictionary = {}
    for variation in variations:
        result.append(BlockReaderSystematic(input_dict, variation, block_reader_general))
        dictionary[variation] = result[-1].cpp_class
    if not systematics_pairs_list is None:
        systematics_pairs_list.append(dictionary)
    return result
