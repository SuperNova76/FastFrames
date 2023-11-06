from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SampleWrapper, TruthWrapper, StringVector
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderSampleTruth import BlockReaderSampleTruth


class BlockReaderSample:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.name = self.options_getter.get("name", None, [str])
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for sample: " + str(self.options_getter))
            exit(1)

        self.simulation_type = self.options_getter.get("simulation_type",None, [str])
        if self.simulation_type is None:
            Logger.log_message("ERROR", "No simulation_type specified for sample {}".format(self.name))
            exit(1)
        self.is_data = (self.simulation_type.upper() == "DATA")

        self.dsids = self.options_getter.get("dsids",None, [list])
        if self.dsids is None and not self.is_data:
            Logger.log_message("ERROR", "No dsids specified for sample {}".format(self.name))
            exit(1)

        self.campaigns = self.options_getter.get("campaigns",None, [list])
        if self.campaigns is None:
            Logger.log_message("ERROR", "No campaigns specified for sample {}".format(self.name))
            exit(1)

        # check if all campaigns are defined in general block
        if not self.is_data and self.campaigns != None and block_reader_general != None:
            for campaign in self.campaigns:
                if not block_reader_general.cpp_class.campaignIsDefined(campaign):
                    Logger.log_message("ERROR", "Unknown campaign {} specified for sample {}".format(campaign, self.name))
                    exit(1)

        self.selection_suffix = self.options_getter.get("selection","true", [str])

        self.regions = self.options_getter.get("regions",None, [list])
        self.exclude_regions = self.options_getter.get("exclude_regions",None, [list])
        if not self.regions is None and not self.exclude_regions is None:
            Logger.log_message("ERROR", "Both regions and exclude_regions specified for sample {}".format(self.name))
            exit(1)

        self.systematic = []

        self.event_weights = self.options_getter.get("event_weights", block_reader_general.default_event_weights if not self.is_data else "1", [str])
        self.weight_suffix = self.options_getter.get("weight_suffix", None, [str])

        self.reco_tree_name = self.options_getter.get("reco_tree_name", block_reader_general.default_reco_tree_name, [str])

        self.selection_suffix = self.options_getter.get("selection_suffix", "", [str])

        self.reco_to_truth_pairing_indices = self.options_getter.get("reco_to_truth_pairing_indices", block_reader_general.reco_to_truth_pairing_indices, [list])

        self.cpp_class = SampleWrapper(self.name)

        self.truth_dicts = self.options_getter.get("truth", None, [list])
        self.truths = []
        if self.truth_dicts is not None:
            reco_variables_from_regions = block_reader_general.cpp_class.getVariableNames()
            for truth_dict in self.truth_dicts:
                truth_object = BlockReaderSampleTruth(truth_dict)
                self.truths.append(truth_object)
                truth_object.check_reco_variables_existence(reco_variables_from_regions)
                self.cpp_class.addTruth(truth_object.cpp_class.getPtr())

        self._set_unique_samples_IDs()

        self._set_cpp_class()

        self._check_unused_options()


    def _set_unique_samples_IDs(self):
        """
        Set unique sample IDs for the sample. If no dsids are specified, take all dsids.
        """
        if self.is_data:
            for campaign in self.campaigns:
                self.cpp_class.addUniqueSampleID(0, campaign, "data")
        else:
            for campaign in self.campaigns:
                for dsid in self.dsids:
                    self.cpp_class.addUniqueSampleID(dsid, campaign, self.simulation_type)


    def adjust_regions(self, regions : dict):
        """
        Set regions for the sample. If no regions are specified, take all regions.
        """
        if self.regions is None: # if no regions are specified, take all regions
            self.regions = []
            for region_name in regions:
                if self.exclude_regions is not None and region_name in self.exclude_regions:
                    continue
                self.regions.append(region_name)
        else:   # if regions are specified, check if they exist
            for region in self.regions:
                if region not in regions:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self.name))
                    exit(1)

        for region_name in self.regions:
            region_object = regions[region_name]
            self.cpp_class.addRegion(region_object.cpp_class.getPtr())


    def adjust_systematics(self, systematics_all : dict):
        self.systematic = []
        for systematic_name, systematic in systematics_all.items():
            # check if systematics has explicit list of samples. If so, does it contain this sample?
            if systematic.samples is not None:
                if self.name not in systematic.samples:
                    continue

            # check if systematics has explicit list of exclude_samples. If so, does it contain this sample?
            if systematic.exclude_samples is not None:
                if self.name in systematic.exclude_samples:
                    continue

            # for data samples, we do not want to add systematics by default (other than nominal)
            if systematic.samples is None and self.is_data and not systematic.cpp_class.isNominal():
                continue

            self.cpp_class.addSystematic(systematic.cpp_class.getPtr())
            self.systematic.append(systematic_name)


    def _set_cpp_class(self) -> None:
        """
        Set the cpp class for the sample.
        """
        total_weight = self.event_weights
        if self.weight_suffix is not None:
            total_weight =  "(" + total_weight + ")*(" + self.weight_suffix + ")"
        self.cpp_class.setEventWeight(total_weight)

        self.cpp_class.setRecoTreeName(self.reco_tree_name)
        self.cpp_class.setSelectionSuffix(self.selection_suffix)

        vector_pairing_indices = StringVector()
        for reco_to_truth_pairing_index in self.reco_to_truth_pairing_indices:
            vector_pairing_indices.append(reco_to_truth_pairing_index)
        self.cpp_class.setRecoToTruthPairingIndices(vector_pairing_indices)

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in  sample block is not supported!".format(unused))
            exit(1)

    def get_truth_cpp_objects(self):
        #return [truth.cpp_class for truth in self.truths]
        vector_shared_ptr = self.cpp_class.getTruthPtrs()
        result = []
        for ptr in vector_shared_ptr:
            truth_cpp_object = TruthWrapper("")
            truth_cpp_object.constructFromSharedPtr(ptr)
            result.append(truth_cpp_object)
        return result

    def get_reco_to_truth_pairing_indices(self):
        indices_vector = self.cpp_class.recoToTruthPairingIndices()
        return [x for x in indices_vector]