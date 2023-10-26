from sys import path

path.append("../build/lib")
path.append("../../build/lib")
path.append("../bin/lib")
path.append("../../bin/lib")
path.append("../../")

from ConfigReaderCpp import ConfigReaderCpp
from python_wrapper.python.logger import Logger

class BlockReaderGeneral:
    def __init__(self, input_dict : dict):
        self.debug_level = input_dict.get("debug_level", "WARNING")
        self.input_filelist_path = input_dict.get("input_filelist_path")
        self.input_sumweights_path = input_dict.get("input_sumweights_path")
        self.output_path = input_dict.get("output_path")
        self.default_sumweights = input_dict.get("default_sumweights", "NOSYS")
        self.default_event_weights = input_dict.get("default_event_weights")
        self.__set_luminosity_map(input_dict.get("luminosity"))
        Logger.set_log_level(self.debug_level)
        self.config_reader_cpp = ConfigReaderCpp()
        self.__set_config_reader_cpp()

    def __set_luminosity_map(self, luminosity_map : dict) -> None:
        self.luminosity_map = {}
        for key, value in luminosity_map.items():
            self.luminosity_map[key] = float(value)

    def __set_config_reader_cpp(self):
        self.config_reader_cpp.setInputPath(self.input_filelist_path)
        self.config_reader_cpp.setInputSumWeightsPath(self.input_sumweights_path)
        self.config_reader_cpp.setOutputPath(self.output_path)
        self.config_reader_cpp.setInputFilelistPath(self.input_filelist_path)

        for campaign, lumi_value in self.luminosity_map.items():
            self.config_reader_cpp.setLuminosity(campaign, lumi_value)
