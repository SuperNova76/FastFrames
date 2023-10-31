from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SampleWrapper
from BlockReaderGeneral import BlockReaderGeneral


class BlockReaderSample:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.name = input_dict.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for sample: " + str(input_dict))
            exit(1)

        self.simulation_type = input_dict.get("simulation_type",None)
        if self.simulation_type is None:
            Logger.log_message("ERROR", "No simulation_type specified for sample {}".format(self.name))
            exit(1)
        self.is_data = (self.simulation_type.upper() == "DATA")

        self.dsids = input_dict.get("dsids",None)
        if self.dsids is None and not self.is_data:
            Logger.log_message("ERROR", "No dsids specified for sample {}".format(self.name))
            exit(1)

        self.campaigns = input_dict.get("campaigns",None)
        if self.campaigns is None:
            Logger.log_message("ERROR", "No campaigns specified for sample {}".format(self.name))
            exit(1)

        self.selection = input_dict.get("selection","true")

        self.regions = input_dict.get("regions",None)

        self.event_weights = input_dict.get("event_weights",None)

        self.cpp_class = SampleWrapper(self.name)

        self._set_unique_samples_IDs()

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


    def adjust_regions(self, regions : list):
        """
        Set regions for the sample. If no regions are specified, take all regions.
        """
        if self.regions is None: # if no regions are specified, take all regions
            self.regions = []
            for region in regions:
                self.regions.append(region.name)
        else:   # if regions are specified, check if they exist
            for region in self.regions:
                if region not in regions:
                    Logger.log_message("ERROR", "Region {} specified for systematic {} does not exist".format(region, self.name))
                    exit(1)

        for region_name in self.regions:
            region_object = regions[region_name]
            self.cpp_class.addRegion(region_object.config_reader_cpp_region.getPtr())