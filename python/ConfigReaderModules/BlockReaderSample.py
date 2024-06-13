"""
@file Source file with BlockReaderSample class.
"""
from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SampleWrapper, TruthWrapper, StringVector, RegionWrapper, SystematicWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderSampleTruth import BlockReaderSampleTruth

import re

def string_matches_some_regex(string : list, regex_list : list) -> bool:
    for regex in regex_list:
        if re.match(regex, string):
            return True
    return False

class BlockReaderSample:
    """!Class for reading sample block of the config, equivalent of C++ class Sample
    """
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        """!Constructor of the BlockReaderSample class
        @param input_dict: dictionary with options from the config file
        @param block_reader_general: BlockReaderGeneral object with general options from the config file - this is there to get default values
        """
        self._options_getter = BlockOptionsGetter(input_dict)

        self._name = self._options_getter.get("name", None, [str])
        if self._name is None:
            Logger.log_message("ERROR", "No name specified for sample: " + str(self._options_getter))
            exit(1)

        self._simulation_type = self._options_getter.get("simulation_type",None, [str])
        if self._simulation_type is None:
            Logger.log_message("ERROR", "No simulation_type specified for sample {}".format(self._name))
            exit(1)
        self._is_data = (self._simulation_type.upper() == "DATA")

        self._dsids = self._options_getter.get("dsids",None, [list], [int])
        if self._dsids is None and not self._is_data:
            Logger.log_message("ERROR", "No dsids specified for sample {}".format(self._name))
            exit(1)

        self._campaigns = self._options_getter.get("campaigns",None, [list], [str])
        if self._campaigns is None:
            Logger.log_message("ERROR", "No campaigns specified for sample {}".format(self._name))
            exit(1)

        # check if all campaigns are defined in general block
        if not self._is_data and self._campaigns != None and block_reader_general != None:
            for campaign in self._campaigns:
                if not block_reader_general.cpp_class.campaignIsDefined(campaign):
                    Logger.log_message("ERROR", "Unknown campaign {} specified for sample {}".format(campaign, self._name))
                    exit(1)

        self._selection_suffix = self._options_getter.get("selection","true", [str])

        self._regions = self._options_getter.get("regions",None, [list], [str])
        self._exclude_regions = self._options_getter.get("exclude_regions",None, [list], [str])
        if not self._regions is None and not self._exclude_regions is None:
            Logger.log_message("ERROR", "Both regions and exclude_regions specified for sample {}".format(self._name))
            exit(1)

        self._systematic = []

        self._event_weights = self._options_getter.get("event_weights", block_reader_general.default_event_weights if not self._is_data else "1.", [str])
        self._weight_suffix = self._options_getter.get("weight_suffix", None, [str])

        self._reco_tree_name = self._options_getter.get("reco_tree_name", block_reader_general.default_reco_tree_name, [str])

        self._selection_suffix = self._options_getter.get("selection_suffix", "", [str])

        self._reco_to_truth_pairing_indices = self._options_getter.get("reco_to_truth_pairing_indices", block_reader_general.reco_to_truth_pairing_indices, [list], [str])

        self._define_custom_columns = self._options_getter.get("define_custom_columns", block_reader_general.define_custom_columns, [list], [dict])

        self._define_custom_columns_truth = self._options_getter.get("define_custom_columns_truth", block_reader_general.define_custom_columns_truth, [list], [dict])

        self._exclude_systematics = self._options_getter.get("exclude_systematics", block_reader_general.default_exclude_systematics, [list], [str])

        self._automatic_systematics = self._options_getter.get("automatic_systematics", block_reader_general.automatic_systematics, [bool])

        self._nominal_only = self._options_getter.get("nominal_only", block_reader_general.nominal_only, [bool])

        if self._automatic_systematics and self._nominal_only:
            Logger.log_message("ERROR", "Both automatic_systematics and nominal_only specified for sample {}. Only one of these options can be True".format(self._name))
            exit(1)

        self._sum_weights = self._options_getter.get("sum_weights", block_reader_general.default_sumweights, [str])

        ## Instance of the SampleWrapper C++ class -> wrapper around C++ Sample class
        self.cpp_class = SampleWrapper(self._name)

        self._truth_dicts = self._options_getter.get("truth", None, [list], [dict])
        self._truths = []
        if self._truth_dicts is not None:
            reco_variables_from_regions = block_reader_general.cpp_class.getVariableNames()
            for truth_dict in self._truth_dicts:
                truth_object = BlockReaderSampleTruth(truth_dict)
                self._truths.append(truth_object)
                truth_object.check_reco_variables_existence(reco_variables_from_regions)
                self.cpp_class.addTruth(truth_object.cpp_class.getPtr())


        self.variables          = self._options_getter.get("variables", None, [list], [str])
        self.exclude_variables  = self._options_getter.get("exclude_variables", None, [list], [str])
        if self.variables is not None and self.exclude_variables is not None:
            Logger.log_message("ERROR", "Both variables and exclude_variables specified for sample {}".format(self._name))
            exit(1)


        self._set_unique_samples_IDs()

        self._set_cpp_class()

        self._check_unused_options()


    def _set_unique_samples_IDs(self):
        """!Set unique sample IDs for the sample. If no dsids are specified, take all dsids.
        """
        if self._is_data:
            for campaign in self._campaigns:
                self.cpp_class.addUniqueSampleID(0, campaign, "data")
        else:
            for campaign in self._campaigns:
                for dsid in self._dsids:
                    self.cpp_class.addUniqueSampleID(dsid, campaign, self._simulation_type)


    def adjust_regions(self, regions : dict) -> None:
        """!Set regions for the sample. If no regions are specified, take all regions, if exclude_regions are specified, remove them from the list of regions.
        @param regions: dictionary with all regions (keys are region names, values are BlockReaderRegion objects)
        """
        if self._exclude_regions is not None:
            for region_name in self._exclude_regions:
                if not string_matches_some_regex(region_name, regions):
                    Logger.log_message("ERROR", "Region {} specified for sample {} does not exist".format(region_name, self._name))
                    exit(1)

        if self._regions is None: # if no regions are specified, take all regions
            self._regions = []
            for region_name in regions:
                if self._exclude_regions is not None and string_matches_some_regex(region_name, self._exclude_regions):
                    continue
                self._regions.append(region_name)
        else:   # if regions are specified, check if they exist
            for region in self._regions:
                if not string_matches_some_regex(region, regions):
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self._name))
                    exit(1)

        for region_name in self._regions:
            region_object = regions[region_name]
            self.cpp_class.addRegion(region_object.cpp_class.getPtr())


    def adjust_systematics(self, systematics_all : dict) -> None:
        """!Set systematics for the sample. For each systematics check if it has explicit list of samples (if not, add all). If sample_exclude is defined for it, check if this sample is not there.
        @param systematics_all: dictionary with all systematics (keys are systematic names, values are BlockReaderSystematic objects)
        """
        self._systematic = []
        for systematic_name, systematic in systematics_all.items():
            # check if systematics has explicit list of samples. If so, does it contain this sample?
            if systematic.samples is not None:
                if self._name not in systematic.samples:
                    continue

            # check if systematics has explicit list of exclude_samples. If so, does it contain this sample?
            if systematic.exclude_samples is not None:
                if self._name in systematic.exclude_samples:
                    continue

            # for data samples, we do not want to add systematics by default (other than nominal)
            if systematic.samples is None and self._is_data and not systematic.cpp_class.isNominal():
                continue

            self.cpp_class.addSystematic(systematic.cpp_class.getPtr())
            self._systematic.append(systematic_name)

    def resolve_variables(self) -> None:
        variables_defined_for_sample = []
        regions_ptrs = self.cpp_class.regions()
        for region_ptr in regions_ptrs:
            region_object = RegionWrapper("")
            region_object.constructFromSharedPtr(region_ptr)
            variables_in_region = region_object.getVariableNames()
            for variable in variables_in_region:
                if variable not in variables_defined_for_sample:
                    variables_defined_for_sample.append(variable)

        variables_to_keep = []

        if self.variables is None and self.exclude_variables is None:
            variables_to_keep = variables_defined_for_sample
        elif self.variables is not None:
            for variable in variables_defined_for_sample:
                if variable in self.variables:
                    variables_to_keep.append(variable)
                elif string_matches_some_regex(variable, self.variables):
                    variables_to_keep.append(variable)
        elif self.exclude_variables is not None:
            for variable in variables_defined_for_sample:
                if variable not in self.exclude_variables and not string_matches_some_regex(variable, self.exclude_variables):
                    variables_to_keep.append(variable)

        for variable in variables_to_keep:
            self.cpp_class.addVariable(variable)


    def _set_cpp_class(self) -> None:
        """!Set the cpp class for the sample.
        """
        total_weight = self._event_weights
        if self._weight_suffix is not None:
            total_weight =  "(" + total_weight + ")*(" + self._weight_suffix + ")"
        self.cpp_class.setEventWeight(total_weight)

        self.cpp_class.setRecoTreeName(self._reco_tree_name)
        self.cpp_class.setSelectionSuffix(self._selection_suffix)

        vector_pairing_indices = StringVector()
        for reco_to_truth_pairing_index in self._reco_to_truth_pairing_indices:
            vector_pairing_indices.append(reco_to_truth_pairing_index)
        self.cpp_class.setRecoToTruthPairingIndices(vector_pairing_indices)

        if self._define_custom_columns:
            for custom_column_dict in self._define_custom_columns:
                custom_column_opts = BlockOptionsGetter(custom_column_dict)
                name        = custom_column_opts.get("name", None, [str])
                definition  = custom_column_opts.get("definition", None, [str])
                if name is None or definition is None:
                    Logger.log_message("ERROR", "Invalid custom column definition for sample {}".format(self._name))
                    exit(1)
                self.cpp_class.addCustomDefine(name, definition)
                if custom_column_opts.get_unused_options():
                    Logger.log_message("ERROR", "Invalid custom column definition for sample {}".format(self._name))
                    exit(1)

        if self._define_custom_columns_truth and self._truths:
            for custom_column_dict in self._define_custom_columns_truth:
                custom_column_opts = BlockOptionsGetter(custom_column_dict)
                name        = custom_column_opts.get("name", None, [str])
                definition  = custom_column_opts.get("definition", None, [str])
                truth_tree  = custom_column_opts.get("truth_tree", None, [str])
                if name is None or definition is None or truth_tree is None:
                    Logger.log_message("ERROR", "Invalid custom truth column definition for sample {}".format(self._name))
                    exit(1)
                self.cpp_class.addCustomTruthDefine(name, definition, truth_tree)

                if custom_column_opts.get_unused_options():
                    Logger.log_message("ERROR", "Invalid custom truth column definition for sample {}".format(self._name))
                    exit(1)

        for syst_regex in self._exclude_systematics:
            self.cpp_class.addExcludeAutomaticSystematic(syst_regex)


        self.cpp_class.setAutomaticSystematics(self._automatic_systematics)
        self.cpp_class.setNominalOnly(self._nominal_only)

        self.cpp_class.setNominalSumWeights(self._sum_weights)

    def _check_unused_options(self):
        unused = self._options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in  sample block is not supported!".format(unused))
            exit(1)

    def get_truth_cpp_objects(vector_shared_ptr) -> list:
        """!Get list of truth cpp objects defined in the sample
        """
        result = []
        for ptr in vector_shared_ptr:
            truth_cpp_object = TruthWrapper("")
            truth_cpp_object.constructFromSharedPtr(ptr)
            result.append(truth_cpp_object)
        return result

    def is_data_sample(sample_cpp_object) -> bool:
        """!Check if the sample is data sample
        """
        n_unique_ids = sample_cpp_object.nUniqueSampleIDs()
        for i in range(n_unique_ids):
            id_string = sample_cpp_object.uniqueSampleIDstring(i)[1:-1]
            sample_type = id_string.split(",")[2].strip()
            if sample_type.lower() != "data":
                return False
        return True

    def get_total_luminosity(general_block_cpp_object, samples : list[SampleWrapper]) -> float:
        """! Loop over all MC samples, indentify all MC campaign and sum up their luminosity
        """
        result = 0
        campaigns = []
        for sample in samples:
            if BlockReaderSample.is_data_sample(sample):
                continue
            n_unique_samples = sample.nUniqueSampleIDs()
            for i_unique_id in range(n_unique_samples):
                unique_sample_string = sample.uniqueSampleIDstring(i_unique_id).split(",")
                campaign = unique_sample_string[1].strip()
                if campaign not in campaigns:
                    campaigns.append(campaign)
                    result += general_block_cpp_object.getLuminosity(campaign)
        return result

    def get_systematics_objects(cpp_class) -> list:
        """!Get list of systematics cpp objects
        @return list of systematics
        """
        result = []
        vector_systematics = cpp_class.getSystematicsSharedPtr()
        for systematic_ptr in vector_systematics:
            systematic_cpp_object = SystematicWrapper("")
            systematic_cpp_object.constructFromSharedPtr(systematic_ptr)
            result.append(systematic_cpp_object)
        return result
