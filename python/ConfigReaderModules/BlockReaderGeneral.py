from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigSettingWrapper, RegionWrapper, StringVector
from python_wrapper.python.logger import Logger
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderGeneral:
    def __init__(self, input_dict : dict):
        self.options_getter = BlockOptionsGetter(input_dict)
        self.debug_level = self.options_getter.get("debug_level", "WARNING")
        Logger.set_log_level(self.debug_level)
        self.input_filelist_path = self.options_getter.get("input_filelist_path", None, [str])
        self.input_sumweights_path = self.options_getter.get("input_sumweights_path", None, [str])
        self.output_path_histograms = self.options_getter.get("output_path_histograms", "", [str])
        self.output_path_ntuples    = self.options_getter.get("output_path_ntuples", "", [str])
        self.default_sumweights = self.options_getter.get("default_sumweights", "NOSYS", [str])
        self.default_event_weights = self.options_getter.get("default_event_weights", None, [str])
        self.default_reco_tree_name = self.options_getter.get("default_reco_tree_name", None, [str])
        self.custom_frame_name = self.options_getter.get("custom_frame_name", "", [str])
        self.automatic_systematics = self.options_getter.get("automatic_systematics", False, [bool])
        self.nominal_only = self.options_getter.get("nominal_only", False, [bool])
        self.create_tlorentz_vectors_for = self.options_getter.get("create_tlorentz_vectors_for", [], [list])
        self.number_of_cpus = self.options_getter.get("number_of_cpus", 1, [int])
        self.xsection_files = self.options_getter.get("xsection_files", ["data/XSection-MC16-13TeV.data"], [list])
        self.reco_to_truth_pairing_indices = self.options_getter.get("reco_to_truth_pairing_indices", ["eventNumber"], [list])
        self.__set_luminosity_map(self.options_getter.get("luminosity", None, [dict]))
        self.define_custom_columns = self.options_getter.get("define_custom_columns", [], [list])
        self.cpp_class = ConfigSettingWrapper()
        self.__set_config_reader_cpp()
        self._check_unused_options()

    def __set_luminosity_map(self, luminosity_map : dict) -> None:
        self.luminosity_map = {}
        if luminosity_map is None:
            Logger.log_message("INFO", "No campaigns and luminosities defined in config. Using default values.")
            return
        for key, value in luminosity_map.items():
            self.luminosity_map[key] = float(value)

    def __set_config_reader_cpp(self):
        self.cpp_class.setInputSumWeightsPath(self.input_sumweights_path)
        self.cpp_class.setOutputPathHistograms(self.output_path_histograms)
        self.cpp_class.setOutputPathNtuples(self.output_path_ntuples)
        self.cpp_class.setInputFilelistPath(self.input_filelist_path)
        self.cpp_class.setNumCPU(self.number_of_cpus)
        self.cpp_class.setCustomFrameName(self.custom_frame_name)
        self.cpp_class.setAutomaticSystematics(self.automatic_systematics)
        self.cpp_class.setNominalOnly(self.nominal_only)

        for campaign, lumi_value in self.luminosity_map.items():
            self.cpp_class.setLuminosity(campaign, lumi_value, True)

        for xsection_file in self.xsection_files:
            self.cpp_class.addXsectionFile(xsection_file)

        for tlorentz_vector in self.create_tlorentz_vectors_for:
            self.cpp_class.addTLorentzVector(tlorentz_vector)

    def add_region(self, region):
        self.cpp_class.addRegion(region.cpp_class.getPtr())

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in general block is not supported!".format(unused))
            exit(1)

    def get_xsection_files(self):
        result = []
        vector_xsection_files = self.cpp_class.xSectionFiles()
        for xsection_file in vector_xsection_files:
            result.append(xsection_file)
        return result
