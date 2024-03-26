"""
@file Source file with BlockReaderGeneral class.
"""
from typing import Optional
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigSettingWrapper, RegionWrapper, SampleWrapper, NtupleWrapper, SystematicWrapper
from ConfigReaderCpp import StringVector, ptrVector
from python_wrapper.python.logger import Logger
from BlockOptionsGetter import BlockOptionsGetter
from CommandLineOptions import CommandLineOptions

def vector_to_list(cpp_vector) -> list:
    """!Convert C++ vector to python list
    @param cpp_vector: C++ vector
    @return python list
    """
    result = []
    for element in cpp_vector:
        result.append(element)
    return result

class BlockReaderGeneral:
    """!Python equivalent of C++ ConfigSetting class
    """

    def __init__(self, input_dict : dict, input_path: Optional[str] = None):
        """!Constructor of the BlockReaderGeneral class. It reads all the options from the general block, sets the properties of the C++ ConfigSetting class and check for user's errors
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)
        self._debug_level = self._options_getter.get("debug_level", "WARNING")
        Logger.set_log_level(self._debug_level)

        self._input_filelist_path = self._options_getter.get("input_filelist_path", None, [str])
        self._input_sumweights_path = self._options_getter.get("input_sumweights_path", None, [str])
        if (input_path is not None):
            Logger.log_message("DEBUG", f"Input path is given as {input_path}. Overwriting the config file input paths")
            self._input_filelist_path = input_path + "/filelist.txt"
            self._input_sumweights_path = input_path + "/sum_of_weights.txt"

        self._output_path_histograms = self._options_getter.get("output_path_histograms", "", [str])
        self._output_path_ntuples    = self._options_getter.get("output_path_ntuples", "", [str])
        self._custom_frame_name = self._options_getter.get("custom_frame_name", "", [str])
        self._create_tlorentz_vectors_for = self._options_getter.get("create_tlorentz_vectors_for", [], [list], [str])
        self._use_rvec = self._options_getter.get("use_rvec", False, [bool])
        self._number_of_cpus = self._options_getter.get("number_of_cpus", 1, [int])
        self._xsection_files = self._options_getter.get("xsection_files", ["data/XSection-MC16-13TeV.data"], [list], [str])
        self._luminosity_map = {}
        self._set_luminosity_map(self._options_getter.get("luminosity", None, [dict]))
        self._min_event = self._options_getter.get("min_event", None, [int])
        self._max_event = self._options_getter.get("max_event", None, [int])
        self._cap_acceptance_selection = self._options_getter.get("cap_acceptance_selection", True, [bool])
        self._custom_options = self._options_getter.get("custom_options", {}, [dict])

        ## Default value for sumweights -> can be overriden in sample block
        self.default_sumweights = self._options_getter.get("default_sumweights", "NOSYS", [str])

        ## Default value for event_weight -> can be overriden in sample block
        self.default_event_weights = self._options_getter.get("default_event_weights", None, [str])

        ## Default value for reco tree name -> can be overriden in sample block
        self.default_reco_tree_name = self._options_getter.get("default_reco_tree_name", None, [str])

        ## Default names and definitions of custom columns to define -> can be overriden in sample block
        self.define_custom_columns = self._options_getter.get("define_custom_columns", [], [list], [dict])

        ## Default indices for pairing truth with reco-level trees, can be overriden in sample block
        self.reco_to_truth_pairing_indices = self._options_getter.get("reco_to_truth_pairing_indices", ["eventNumber"], [list], [str])

        ## List of systematics regexes to exclude from automatic systematics -> can be overriden in sample block
        self.default_exclude_systematics = self._options_getter.get("exclude_systematics", [], [list], [str])

        ## Default automatic systematics option
        self.automatic_systematics = self._options_getter.get("automatic_systematics", False, [bool])

        ## Default nominal only option
        self.nominal_only = self._options_getter.get("nominal_only", False, [bool])

        ## Instance of the ConfigSettingsWrapper C++ class -> wrapper around C++ ConfigSetting class
        self.cpp_class = ConfigSettingWrapper()

        # Custom defininitions from config come after custom class definitions
        self.config_define_after_custom_class = self._options_getter.get("config_define_after_custom_class", False, [bool])

        self._set_custom_options()
        self._set_job_index_and_split_n_jobs()
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

    def _set_job_index_and_split_n_jobs(self) -> None:
        """
        Set job index and split_n_jobs, if they were specified in the command line
        """
        cli_split_n_jobs = CommandLineOptions().get_split_n_jobs()
        cli_job_index    = CommandLineOptions().get_job_index()
        if cli_split_n_jobs is not None:
            self.cpp_class.setTotalJobSplits(cli_split_n_jobs)
        if cli_job_index is not None:
            self.cpp_class.setCurrentJobIndex(cli_job_index)

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
        self.cpp_class.setCapAcceptanceSelection(self._cap_acceptance_selection)
        self.cpp_class.setConfigDefineAfterCustomClass(self.config_define_after_custom_class)

        for campaign, lumi_value in self._luminosity_map.items():
            self.cpp_class.setLuminosity(campaign, lumi_value, True)

        for xsection_file in self._xsection_files:
            self.cpp_class.addXsectionFile(xsection_file)

        for tlorentz_vector in self._create_tlorentz_vectors_for:
            self.cpp_class.addTLorentzVector(tlorentz_vector)

        self.cpp_class.setUseRVec(self._use_rvec)

        # min_event
        cli_min_event = CommandLineOptions().get_min_event()
        if cli_min_event:
            self.cpp_class.setMinEvent(cli_min_event)
        elif self._min_event:
            self.cpp_class.setMinEvent(self._min_event)

        # max_event
        cli_max_event = CommandLineOptions().get_max_event()
        if cli_max_event:
            self.cpp_class.setMaxEvent(cli_max_event)
        elif self._max_event:
            self.cpp_class.setMaxEvent(self._max_event)

    def add_region(self, region : RegionWrapper) -> None:
        """!Add region to the C++ ConfigSetting class
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
        """!Get list of cross section files
        @return list of cross section files
        """
        result = []
        vector_xsection_files = self.cpp_class.xSectionFiles()
        for xsection_file in vector_xsection_files:
            result.append(xsection_file)
        return result

    def get_regions_cpp_objects(self) -> list:
        """!Get list of regions cpp objects
        @return list of regions
        """
        result = []
        vector_regions = self.cpp_class.getRegionsSharedPtr()
        for region_ptr in vector_regions:
            region_cpp_object = RegionWrapper("")
            region_cpp_object.constructFromSharedPtr(region_ptr)
            result.append(region_cpp_object)
        return result

    def get_samples_objects(self) -> list:
        """!Get list of samples cpp objects
        @return list of samples
        """
        result = []
        vector_samples = self.cpp_class.getSamplesSharedPtr()
        for sample_ptr in vector_samples:
            sample_cpp_object = SampleWrapper("")
            sample_cpp_object.constructFromSharedPtr(sample_ptr)
            result.append(sample_cpp_object)
        return result

    def get_ntuple_object(self) -> NtupleWrapper:
        """!Get ntuple cpp object, return None if not defined
        """
        shared_ptr = self.cpp_class.getNtupleSharedPtr()
        if shared_ptr == 0:
            return None
        result = NtupleWrapper()
        result.constructFromSharedPtr(shared_ptr)
        return result

    def get_systematics_objects(self) -> list:
        """!Get list of systematics cpp objects
        @return list of systematics
        """
        result = []
        vector_systematics = self.cpp_class.getSystematicsSharedPtr()
        for systematic_ptr in vector_systematics:
            systematic_cpp_object = SystematicWrapper("")
            systematic_cpp_object.constructFromSharedPtr(systematic_ptr)
            result.append(systematic_cpp_object)
        return result

    def _set_custom_options(self)   -> None:
        """
        Set custom options
        """
        for key, value in self._custom_options.items():
            value_str = str(value)
            self.cpp_class.addCustomOption(key, value_str)

    def get_list_of_custom_keys(self) -> list:
        """!Get list of custom keys
        @return list of custom keys
        """
        result = []
        vector_keys = self.cpp_class.getCustomKeys()
        for key in vector_keys:
            result.append(key)
        return result

    def get_custom_option(self, key : str) -> str:
        """!Get custom option
        @param key: key of the option
        @return value of the option
        """
        return self.cpp_class.getCustomOption(key)