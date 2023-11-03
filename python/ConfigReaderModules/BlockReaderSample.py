from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SampleWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter


class BlockReaderSample:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.name = self.options_getter.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for sample: " + str(self.options_getter))
            exit(1)

        self.simulation_type = self.options_getter.get("simulation_type",None)
        if self.simulation_type is None:
            Logger.log_message("ERROR", "No simulation_type specified for sample {}".format(self.name))
            exit(1)
        self.is_data = (self.simulation_type.upper() == "DATA")

        self.dsids = self.options_getter.get("dsids",None)
        if self.dsids is None and not self.is_data:
            Logger.log_message("ERROR", "No dsids specified for sample {}".format(self.name))
            exit(1)

        self.campaigns = self.options_getter.get("campaigns",None)
        if self.campaigns is None:
            Logger.log_message("ERROR", "No campaigns specified for sample {}".format(self.name))
            exit(1)

        # check if all campaigns are defined in general block
        if not self.is_data and self.campaigns != None and block_reader_general != None:
            for campaign in self.campaigns:
                if not block_reader_general.cpp_class.campaignIsDefined(campaign):
                    Logger.log_message("ERROR", "Unknown campaign {} specified for sample {}".format(campaign, self.name))
                    exit(1)

        self.selection_suffix = self.options_getter.get("selection","true")

        self.regions = self.options_getter.get("regions",None)

        self.systematic = []

        self.event_weights = self.options_getter.get("event_weights", block_reader_general.default_event_weights if not self.is_data else "1")
        self.weight_suffix = self.options_getter.get("weight_suffix", None)

        self.reco_tree_name = self.options_getter.get("reco_tree_name", block_reader_general.default_reco_tree_name)

        self.selection_suffix = self.options_getter.get("selection_suffix", "")

        self.cpp_class = SampleWrapper(self.name)

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
            # check if system has explicit list of samples. If so, does it contain this sample?
            if systematic.samples is not None:
                if self.name not in systematic.samples:
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

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in  sample block is not supported!".format(unused))
            exit(1)