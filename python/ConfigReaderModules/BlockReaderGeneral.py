"""
@file Source file with BlockReaderGeneral class.
"""
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigSettingWrapper, RegionWrapper, StringVector
from python_wrapper.python.logger import Logger
from BlockOptionsGetter import BlockOptionsGetter

class BlockReaderGeneral:
    """!Python equivalent of C++ ConfigSetting class
    """

    def __init__(self, input_dict : dict):
        """
        Constructor of the BlockReaderGeneral class. It reads all the options from the general block, sets the properties of the C++ ConfigSetting class and check for user's errors
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)
        self._debug_level = self._options_getter.get("debug_level", "WARNING")
        Logger.set_log_level(self._debug_level)
        self._input_filelist_path = self._options_getter.get("input_filelist_path", None, [str])
        self._input_sumweights_path = self._options_getter.get("input_sumweights_path", None, [str])
        self._output_path_histograms = self._options_getter.get("output_path_histograms", "", [str])
        self._output_path_ntuples    = self._options_getter.get("output_path_ntuples", "", [str])
        self._custom_frame_name = self._options_getter.get("custom_frame_name", "", [str])
        self._automatic_systematics = self._options_getter.get("automatic_systematics", False, [bool])
        self._nominal_only = self._options_getter.get("nominal_only", False, [bool])
        self._create_tlorentz_vectors_for = self._options_getter.get("create_tlorentz_vectors_for", [], [list])
        self._number_of_cpus = self._options_getter.get("number_of_cpus", 1, [int])
        self._xsection_files = self._options_getter.get("xsection_files", ["data/XSection-MC16-13TeV.data"], [list])
        self._luminosity_map = {}
        self._set_luminosity_map(self._options_getter.get("luminosity", None, [dict]))

        ## Default value for sumweights -> can be overriden in sample block
        self.default_sumweights = self._options_getter.get("default_sumweights", "NOSYS", [str])

        ## Default value for event_weight -> can be overriden in sample block
        self.default_event_weights = self._options_getter.get("default_event_weights", None, [str])

        ## Default value for reco tree name -> can be overriden in sample block
        self.default_reco_tree_name = self._options_getter.get("default_reco_tree_name", None, [str])

        ## Default names and definitions of custom columns to define -> can be overriden in sample block
        self.define_custom_columns = self._options_getter.get("define_custom_columns", [], [list])

        ## Default indices for pairing truth with reco-level trees, can be overriden in sample block
        self.reco_to_truth_pairing_indices = self._options_getter.get("reco_to_truth_pairing_indices", ["eventNumber"], [list])

        ## Instance of the ConfigSettingsWrapper C++ class -> wrapper around C++ ConfigSetting class
        self.cpp_class = ConfigSettingWrapper()

        self._set_config_reader_cpp()
        self._check_unused_options()

    def _set_luminosity_map(self, luminosity_map : dict) -> None:
        """
        Set luminosity values, given the luminosity dictionary read from config
        @param luminosity_map: dictionary of luminosity values - keys are campaign names (strings), values are luminosity values (float)
        """
        if luminosity_map is None:
            Logger.log_message("INFO", "No campaigns and luminosities defined in config. Using default values.")
            return
        for key, value in luminosity_map.items():
            self._luminosity_map[key] = float(value)

    def _set_config_reader_cpp(self):
        """
        Set all the properties of the C++ ConfigSetting class
        """
        self.cpp_class.setInputSumWeightsPath(self._input_sumweights_path)
        self.cpp_class.setOutputPathHistograms(self._output_path_histograms)
        self.cpp_class.setOutputPathNtuples(self._output_path_ntuples)
        self.cpp_class.setInputFilelistPath(self._input_filelist_path)
        self.cpp_class.setNumCPU(self._number_of_cpus)
        self.cpp_class.setCustomFrameName(self._custom_frame_name)
        self.cpp_class.setAutomaticSystematics(self._automatic_systematics)
        self.cpp_class.setNominalOnly(self._nominal_only)

        for campaign, lumi_value in self._luminosity_map.items():
            self.cpp_class.setLuminosity(campaign, lumi_value, True)

        for xsection_file in self._xsection_files:
            self.cpp_class.addXsectionFile(xsection_file)

        for tlorentz_vector in self._create_tlorentz_vectors_for:
            self.cpp_class.addTLorentzVector(tlorentz_vector)

    def add_region(self, region : RegionWrapper) -> None:
        """
        Add region to the C++ ConfigSetting class
        @param region: RegionWrapper object
        """
        self.cpp_class.addRegion(region.cpp_class.getPtr())

    def _check_unused_options(self) -> None:
        """
        Check if all options from the general block were read, if not, print error message and exit
        """
        unused = self._options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in general block is not supported!".format(unused))
            exit(1)

    def get_xsection_files(self) -> list:
        """
        Get list of cross section files
        @return list of cross section files
        """
        result = []
        vector_xsection_files = self.cpp_class.xSectionFiles()
        for xsection_file in vector_xsection_files:
            result.append(xsection_file)
        return result
