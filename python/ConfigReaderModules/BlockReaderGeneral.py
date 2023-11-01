from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppGeneral, ConfigReaderCppRegion
from python_wrapper.python.logger import Logger
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderGeneral:
    def __init__(self, input_dict : dict):
        self.options_getter = BlockOptionsGetter(input_dict)
        self.debug_level = self.options_getter.get("debug_level", "WARNING")
        Logger.set_log_level(self.debug_level)
        self.input_filelist_path = self.options_getter.get("input_filelist_path")
        self.input_sumweights_path = self.options_getter.get("input_sumweights_path")
        self.output_path = self.options_getter.get("output_path")
        self.default_sumweights = self.options_getter.get("default_sumweights", "NOSYS")
        self.default_event_weights = self.options_getter.get("default_event_weights")
        self.default_reco_tree_name = self.options_getter.get("default_reco_tree_name")
        self.custom_frame_name = self.options_getter.get("custom_frame_name", "")
        self.number_of_cpus = self.options_getter.get("number_of_cpus", 1)
        self.__set_luminosity_map(self.options_getter.get("luminosity"))
        self.cpp_class = ConfigReaderCppGeneral()
        self.__set_config_reader_cpp()
        self._check_unused_options()

    def __set_luminosity_map(self, luminosity_map : dict) -> None:
        self.luminosity_map = {}
        for key, value in luminosity_map.items():
            self.luminosity_map[key] = float(value)

    def __set_config_reader_cpp(self):
        self.cpp_class.setInputSumWeightsPath(self.input_sumweights_path)
        self.cpp_class.setOutputPath(self.output_path)
        self.cpp_class.setInputFilelistPath(self.input_filelist_path)
        self.cpp_class.setNumCPU(self.number_of_cpus)
        self.cpp_class.setCustomFrameName(self.custom_frame_name)

        for campaign, lumi_value in self.luminosity_map.items():
            self.cpp_class.setLuminosity(campaign, lumi_value)

    def add_region(self, region):
        self.cpp_class.addRegion(region.cpp_class.getPtr())

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("WARNING", "Key {} used in general block is not supported!".format(unused))
