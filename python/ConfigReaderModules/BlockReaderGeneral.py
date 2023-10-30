from BlockReaderCommon import set_path_to_shared_lib
set_path_to_shared_lib()

from ConfigReaderCpp import ConfigReaderCppGeneral
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
        self.config_reader_cpp_general = ConfigReaderCppGeneral()
        self.__set_config_reader_cpp()

    def __set_luminosity_map(self, luminosity_map : dict) -> None:
        self.luminosity_map = {}
        for key, value in luminosity_map.items():
            self.luminosity_map[key] = float(value)

    def __set_config_reader_cpp(self):
        self.config_reader_cpp_general.setInputPath(self.input_filelist_path)
        self.config_reader_cpp_general.setInputSumWeightsPath(self.input_sumweights_path)
        self.config_reader_cpp_general.setOutputPath(self.output_path)
        self.config_reader_cpp_general.setInputFilelistPath(self.input_filelist_path)
        self.config_reader_cpp_general.setNumCPU(self.number_of_cpus)
        self.config_reader_cpp_general.setCustomFrameName(self.custom_frame_name)

        for campaign, lumi_value in self.luminosity_map.items():
            self.config_reader_cpp_general.setLuminosity(campaign, lumi_value)
