from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppGeneral, ConfigReaderCppRegion
from python_wrapper.python.logger import Logger

class BlockReaderGeneral:
    def __init__(self, input_dict : dict):
        self.debug_level = input_dict.get("debug_level", "WARNING")
        self.input_filelist_path = input_dict.get("input_filelist_path")
        self.input_sumweights_path = input_dict.get("input_sumweights_path")
        self.output_path = input_dict.get("output_path")
        self.default_sumweights = input_dict.get("default_sumweights", "NOSYS")
        self.default_event_weights = input_dict.get("default_event_weights")
        self.default_reco_tree_name = input_dict.get("default_reco_tree_name")
        self.custom_frame_name = input_dict.get("custom_frame_name", "")
        self.number_of_cpus = input_dict.get("number_of_cpus", 1)
        self.__set_luminosity_map(input_dict.get("luminosity"))
        Logger.set_log_level(self.debug_level)
        self.cpp_class = ConfigReaderCppGeneral()
        self.__set_config_reader_cpp()

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
