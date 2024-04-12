
import os
import sys
from copy import deepcopy
import re

from ROOT import TFile

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir + "../")

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from BlockReaderGeneral import vector_to_list
from ConfigReaderCpp import VariableWrapper
from ConfigReader import ConfigReader


import yaml

def get_strings_common_part(str1 : str, str2 : str) -> str:
    result = ""
    for i in range(min(len(str1), len(str2))):
        if str1[i] == str2[i]:
            result += str1[i]
        else:
            return result

def remove_items(dict_to_use : dict, key : str, items_to_remove : list[str]) -> None:
    if key in dict_to_use:
        items_list = dict_to_use.get(key, None)
        if items_list == None:
            return
        if items_to_remove == None:
            return
        items_list = items_list.strip("'\"")
        items_list = items_list.split(",")
        for sample in items_to_remove:
            if sample in items_list:
                items_list.remove(sample)
        if items_list:
            dict_to_use[key] = ",".join(items_list)
        else:
            del dict_to_use[key]

class TrexSettingsGetter:
    def __init__(self, config_reader : ConfigReader, trex_settings_yaml : str = "", unfolding_tuple : tuple[str,str,str,str] = None, regions : list[str] = [".*"]):
        self.trex_settings_dict = None
        if trex_settings_yaml:
            with open(trex_settings_yaml, "r") as f:
                try:
                    self.trex_settings_dict = yaml.load(f, Loader=yaml.FullLoader)
                except yaml.scanner.ScannerError:
                    Logger.log_message("ERROR", "Cannot parse yaml file {}".format(trex_settings_yaml))
                    exit(1)
        self._sample_color_counter = 2

        self.unfolding_sample = ""
        self.unfolding_level = ""
        self.unfolding_variable_truth = ""
        self.unfolding_variable_reco = ""
        self.run_unfolding = False

        # TODO: clean this up
        histo_path = config_reader.block_general.cpp_class.outputPathHistograms()
        if histo_path == "":
            histo_path = "."
        self._files_path = os.path.abspath(histo_path)


        self._set_unfolding_settings(unfolding_tuple)
        self._region_variable_regexes = deepcopy(regions)

        self.config_reader = config_reader

        self.unfolding_n_bins = None

        self._all_MC_samples = None
        self._unfolding_MC_samples_names = None
        self._inclusive_MC_samples_names = None
        self._truth_samples_blocks = []
        self._nominal_truth_sample = None

        self._regions_map = {}       # key = region name, value = region C++ object
        self._region_blocks = []    # list[tuple[str,str,dict]] -> region blocks for trex-fitter config
        self._region_variable_combinations_wo_unfolding = [] # list[str] list of region_variable combinations used in the unfolding
        self._initialize_region_variables()

        self._unfolding_samples_blocks = [] # list[tuple[str,str,dict]] -> unfolding samples blocks for trex-fitter config
        self._inclusive_samples_blocks = [] # list[tuple[str,str,dict]] -> samples blocks for trex-fitter config
        self._initialize_sample_variables()

        self._systematics_blocks = [] # list[tuple[str,str,dict]] -> systematics blocks for trex-fitter config
        self._initialize_systematics_variables()

        self._total_lumi = BlockReaderSample.get_total_luminosity(config_reader.block_general.cpp_class, config_reader.block_general.get_samples_objects())

    def get_region_blocks(self) -> list[tuple[str,str,dict]]:
        return self._region_blocks

    def _match_selected_regions(self, region_and_variable : str) -> bool:
        for regex in self._region_variable_regexes:
            if re.match(regex, region_and_variable):
                return True
        return False

    def _initialize_region_variables(self) -> None:
        regions = self.config_reader.block_general.get_regions_cpp_objects()
        for region in regions:
            region_name = region.name()
            region_selected = False
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                region_variable = region_name + "_" + variable_cpp_object.name()
                if not self._match_selected_regions(region_variable):
                    continue
                region_selected = True
                if self.run_unfolding:
                    if variable_cpp_object.name() != self.unfolding_variable_reco:
                        self._region_variable_combinations_wo_unfolding.append(region_variable)
                        continue
                region_tuple = self._get_region_tuple(region,variable_cpp_object)
                self._region_blocks.append(region_tuple)
            if region_selected:
                self._regions_map[region_name] = region

    def _get_region_tuple(self, region, variable) -> tuple[str,str,dict]:
        dictionary = {}
        region_dict_settings = {}
        if self.trex_settings_dict:
            region_dict_settings = self.trex_settings_dict.get("Regions", {})
        variable_name = variable.name().replace("_NOSYS","")
        region_name = region.name() + "_" + variable_name
        dictionary["Type"] = "SIGNAL"
        for region_dict in region_dict_settings:
            if re.match(region_dict["name"],region_name):
                dictionary["Type"] = region_dict.get("Type","SIGNAL")
        dictionary["VariableTitle"] = variable_name # TODO: proper title
        dictionary["HistoName"] = "NOSYS/" + variable_name + "_" + region.name()
        dictionary["Label"] = region_name       # TODO: proper label
        dictionary["ShortLabel"] = region_name  # TODO: proper label
        if self.run_unfolding:
            dictionary["NumberOfRecoBins"] = variable.axisNbins()
            dictionary["AcceptanceNameSuff"] = "_" + region.name()
            dictionary["SelectionEffNameSuff"] = "_" + region.name()
            dictionary["MigrationNameSuff"] = "_" + region.name()

        return "Region", region_name, dictionary

    def get_unfolding_samples_blocks(self) -> list[tuple[str,str,dict]]:
        return self._unfolding_samples_blocks

    def get_truth_samples_blocks(self) -> list[tuple[str,str,dict]]:
        return self._truth_samples_blocks

    def get_samples_blocks(self) -> list[tuple[str,str,dict]]:
        return self._inclusive_samples_blocks

    def _initialize_sample_variables(self) -> None:
        self._unfolding_MC_samples_names = []
        if self.run_unfolding:
            self._initialize_unfolding_and_truth_samples_blocks()
        self._inclusive_samples_blocks = self._get_samples_blocks()

        if not self._all_MC_samples:
            Logger.log_message("ERROR", "No MC samples have been defined. Cannot produce trex-fitter config.")
            exit(1)

    def _initialize_unfolding_and_truth_samples_blocks(self) -> None:
        if not self.run_unfolding:
            return
        samples_cpp_objects = self.config_reader.block_general.get_samples_objects()
        self._unfolding_samples_blocks = []
        self._truth_samples_blocks = []
        unfolding_samples_cpp_objects = []
        for sample in samples_cpp_objects:
            sample_setting_dict = self._get_sample_dict(sample.name())
            truth_objects = BlockReaderSample.get_truth_cpp_objects(sample.getTruthSharedPtrs())
            for truth_object in truth_objects:
                level = truth_object.name()
                if level != self.unfolding_level:
                    continue
                variable_raw_ptrs = truth_object.getVariableRawPtrs()
                for variable_ptr in variable_raw_ptrs:
                    truth_variable = VariableWrapper("")
                    truth_variable.constructFromRawPtr(variable_ptr)
                    if truth_variable.name() != self.unfolding_variable_truth:
                        continue

                    if self.unfolding_n_bins is None:
                        self.unfolding_n_bins = truth_variable.axisNbins()
                    unfolding_sample_dict = {}
                    sample_name = sample.name()
                    if sample_name != self.unfolding_sample:
                        unfolding_sample_dict["Type"] = "GHOST"

                    sample_color = self.get_sample_color()
                    unfolding_sample_dict["FillColor"] = sample_setting_dict.get("FillColor", sample_color)
                    unfolding_sample_dict["LineColor"] = sample_setting_dict.get("LineColor", sample_color)
                    unfolding_sample_dict["Title"] = sample_setting_dict.get("Title", sample.name())
                    if "Type" in sample_setting_dict:
                        unfolding_sample_dict["Type"] = sample_setting_dict["Type"]

                    unfolding_sample_dict["AcceptanceFile"] =  sample_name
                    unfolding_sample_dict["MigrationFile"] =  sample_name
                    unfolding_sample_dict["SelectionEffFile"] =  sample_name

                    unfolding_sample_dict["AcceptanceName"]   = "NOSYS/acceptance_" + level + "_" + self.unfolding_variable_reco
                    unfolding_sample_dict["SelectionEffName"] = "NOSYS/selection_eff_" + level + "_" + truth_variable.name()
                    unfolding_sample_dict["MigrationName"]    = "NOSYS/" + self.unfolding_variable_reco + "_vs_" + level + "_" + truth_variable.name()

                    self._unfolding_samples_blocks.append(("UnfoldingSample", sample.name(), unfolding_sample_dict))
                    unfolding_samples_cpp_objects.append(sample.name())

                    truth_sample_dict = {
                        "LineColor" : sample_setting_dict.get("LineColor", sample_color),
                        "Title" : sample_setting_dict.get("Title", sample_name),
                        "TruthDistributionFile" : sample_name,
                        "TruthDistributionName" : self.unfolding_level + "_" + truth_variable.name(),
                        "TruthDistributionPath": self._files_path
                    }

                    if not self._nominal_truth_sample:
                        self._nominal_truth_sample = "truth_sample_" + sample.name()
                    self._truth_samples_blocks.append(("TruthSample", "truth_sample_" + sample.name(), truth_sample_dict))
        self._unfolding_MC_samples_names = unfolding_samples_cpp_objects

    def _get_samples_blocks(self) -> list[tuple[str,str,dict]]:
        all_samples = self.config_reader.block_general.get_samples_objects()
        if self.trex_settings_dict:
            general_dict = self.trex_settings_dict.get("General", {})
            if "exclude_samples" in general_dict:
                # for excluded_sample in general_dict["exclude_samples"]:
                #     all_samples = [sample for sample in all_samples if sample.name() != excluded_sample ]
                for regex in general_dict["exclude_samples"]:
                    all_samples = [sample for sample in all_samples if not re.match(regex,sample.name())]
        result = []
        self._inclusive_MC_samples_names = []
        for sample in all_samples:
            if self._unfolding_MC_samples_names:
                if sample.name() in self._unfolding_MC_samples_names:
                    continue
            self._inclusive_MC_samples_names.append(sample.name())
            sample_tuple = self._get_sample_tuple(sample)
            result.append(sample_tuple)
        self._all_MC_samples = [sample for sample in all_samples if not BlockReaderSample.is_data_sample(sample)]
        return result

    def _get_sample_tuple(self, sample) -> tuple[str,str,dict]:
        dictionary = {}
        is_data = BlockReaderSample.is_data_sample(sample)
        sample_setting_dict = self._get_sample_dict(sample.name())
        dictionary["Type"] =  sample_setting_dict.get("Type", "BACKGROUND" if not is_data else "DATA")
        dictionary["Title"] = sample_setting_dict.get("Title", sample.name())
        dictionary["HistoFile"] = sample.name()
        sample_color = sample_setting_dict.get("Color", self.get_sample_color())
        dictionary["FillColor"] = sample_setting_dict.get("FillColor", sample_color) # TODO: FillColor
        dictionary["LineColor"] = sample_setting_dict.get("LineColor", sample_color)  # TODO: LineColor
        if "Template" in sample_setting_dict:
            dictionary["Template"] = sample_setting_dict["Template"]

        region_names = vector_to_list(sample.regionsNames())
        selected_regions = []
        all_regions_list = []
        variable_names_defined_for_sample = vector_to_list(sample.variables())
        for region_name in region_names:
            if not region_name in self._regions_map:
                continue
            region = self._regions_map[region_name]
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable in variable_cpp_objects:
                variable_name = variable.name()
                region_variable = region.name() + "_" + variable_name.replace("_NOSYS","")
                if not self._match_selected_regions(region_variable):
                    continue
                all_regions_list.append(region_variable)
                if not variable_name in variable_names_defined_for_sample:
                    continue
                selected_regions.append(region_variable)

        # check if there is more included or excluded regions and use the shorter list of these two
        if (len(selected_regions) > 0.5*len(all_regions_list)):
            excluded_regions = []
            for region in all_regions_list:
                if not region in selected_regions:
                    excluded_regions.append(region)
            if excluded_regions:
                dictionary["Exclude"] = ",".join(excluded_regions)
        else:
            dictionary["Regions"] = ",".join(selected_regions)

        result = ("Sample", sample.name(), dictionary)
        self.remove_regions_wo_unfolding(result)
        return result

    def get_systematics_blocks(self) -> list[tuple[str,str,dict]]:
        return self._systematics_blocks

    def _initialize_systematics_variables(self ) -> None:
        trex_only_systemaics = self.get_trex_only_systematics_blocks()

        automatic_syst_dict = self.get_automatic_systematics_blocks(self.config_reader.block_general.cpp_class.outputPathHistograms())
        config_based_syst_dict = self._get_systematics_blocks_from_config_wo_samples_resolving() #list[tuple[str,str,dict]]
        all_systematics = self._merge_automatic_and_config_based_systematics_and_resolve_samples_list_and_excludes(automatic_syst_dict, config_based_syst_dict)

        all_systematics = all_systematics + trex_only_systemaics
        for syst_tuple in all_systematics:
            syst_inclusive, syst_unfolding = self.split_systematics_into_inclusive_and_unfolding(syst_tuple)
            if syst_inclusive:
                self.remove_regions_wo_unfolding(syst_inclusive)
                self._systematics_blocks.append(syst_inclusive)
            if syst_unfolding:
                self.remove_regions_wo_unfolding(syst_unfolding)
                self._systematics_blocks.append(syst_unfolding)


    def remove_regions_wo_unfolding(self, block : tuple[str,str,dict]) -> None:   # TODO: make it private
        remove_items(block[2], "Regions", self._region_variable_combinations_wo_unfolding)
        remove_items(block[2], "Exclude", self._region_variable_combinations_wo_unfolding)

    def _set_unfolding_settings(self, unfolding_settings_tuple : tuple[str,str,str,str]) -> None:
        if unfolding_settings_tuple:
            self.run_unfolding = True
            self.unfolding_sample = unfolding_settings_tuple[0]
            self.unfolding_level = unfolding_settings_tuple[1]
            self.unfolding_variable_truth = unfolding_settings_tuple[2]
            self.unfolding_variable_reco = unfolding_settings_tuple[3]
        else:
            self.run_unfolding = False

    def _get_sample_dict(self, sample_name : str) -> dict:
        if not self.trex_settings_dict:
            return {}
        samples_dict = self.trex_settings_dict.get("samples", [])
        for sample_dict in samples_dict:
            if sample_dict["name"] == sample_name:
                return sample_dict
        return {}

    def _get_fit_dict(self) -> dict:
        if not self.trex_settings_dict:
            return {}
        return self.trex_settings_dict.get("Fit", {})

    def _get_unfolding_dict(self) -> dict:
        if not self.trex_settings_dict:
            return {}
        return self.trex_settings_dict.get("Unfolding", {})

    def _get_all_trexfitter_regions(self) -> list[str]:
        result = []
        for region_name, region in self._regions_map.items():
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                variable_name = variable_cpp_object.name()
                variable_name = variable_name.replace("_NOSYS","")
                region_variable = region_name + "_" + variable_name
                if not self._match_selected_regions(region_variable):
                    continue
                result.append(variable_name + "_" + region_name)
        return result

    def get_trex_only_systematics_blocks(self) -> list[tuple[str,str,dict]]:
        if not self.trex_settings_dict:
            return []
        result = []
        systematics_dict = self.trex_settings_dict.get("Systematics", [])
        for syst_dict in systematics_dict:
            syst_name = syst_dict.get("name", None)
            if not syst_name:
                Logger.log_message("ERROR", "Systematic without name found in the yaml file")
                exit(1)
            syst_output_dict = deepcopy(syst_dict)
            del syst_output_dict["name"]
            result.append(("Systematic", syst_name, syst_output_dict))
        return result

    def get_normfactors_from_trex_settings(self) -> list[tuple[str,str,dict]]:
        if not self.trex_settings_dict:
            return []
        result = []
        normfactors_dict = self.trex_settings_dict.get("NormFactors", [])
        for normfactor_dict in normfactors_dict:
            normfactor_name = normfactor_dict.get("name", None)
            if not normfactor_name:
                Logger.log_message("ERROR", "NormFactor without name found in the yaml file")
                exit(1)
            normfactor_output_dict = deepcopy(normfactor_dict)
            del normfactor_output_dict["name"]
            result.append(("NormFactor", normfactor_name, normfactor_output_dict))
        return result

    def _get_automatic_systematics_dict_histoname_to_sample_list(self, output_root_files_folder : str, sample_names : list) -> dict[str,list]:
        """
        Returns a dictionary with key = systematic name and value = list of samples for which it is defined
        """
        trex_regions_names = self._get_all_trexfitter_regions()
        result = {} # key = systematic name, value = list of samples for which it is defined
        for sample_name in sample_names:
            sample_path = os.path.join(output_root_files_folder, sample_name + ".root")
            if not os.path.exists(sample_path):
                Logger.log_message("ERROR", "ROOT file for sample {} does not exist".format(sample_name))
                exit(1)
            root_file = TFile(sample_path, "READ")
            # loop over all TDirectories in the root file
            for key in root_file.GetListOfKeys():
                if not key.IsFolder():
                    continue
                systematic_name = key.GetName()
                if systematic_name in ["NOSYS", "truth", "particleLevel"]:
                    continue

                # get list of all histograms in folder:
                directory = root_file.Get(systematic_name)
                histogram_found = False
                for histo_key in directory.GetListOfKeys():
                    histo_key_name = histo_key.GetName()
                    if histo_key_name in trex_regions_names:
                        histogram_found = True
                        break
                if systematic_name not in result:
                    result[systematic_name] = []
                syst_list = result[systematic_name]
                if histogram_found:
                    syst_list.append(sample_name)
            root_file.Close()
        return result

    def _get_automatic_systematics_dict_systname_to_dict(self, output_root_files_folder : str, sample_names : list[str]) -> dict[str,dict]:
        """
        Returns dictionary where key is the name of the systematics (without __1up or __1down suffixes) and value is a dictionary with the following keys:
        "HistoFolderNameUp" : name of the folder with up variation
        "HistoFolderNameDown" : name of the folder with down variation
        "Samples" : list of samples for which the systematic is defined
        """
        sample_names = [sample.name() for sample in self._all_MC_samples if sample.automaticSystematics()]
        automatic_systematics = self._get_automatic_systematics_dict_histoname_to_sample_list(output_root_files_folder, sample_names)
        contains_generator_syst = False
        result = {}
        for histo_name in automatic_systematics:
            if histo_name.startswith("GEN_"):
                contains_generator_syst = True
                continue
            syst_name = histo_name
            if histo_name.endswith("__1up"):
                if histo_name.endswith("_PseudoData__1up"):
                    syst_name = histo_name[:-16]
                    if syst_name in result:
                        result[syst_name]["HistoFolderSubtractNameUp"] = histo_name
                    else:
                        result[syst_name] = {"HistoFolderSubtractNameUp": histo_name, "Samples": automatic_systematics[histo_name]}                    
                else:
                    syst_name = histo_name[:-5]
                    if syst_name in result:
                        result[syst_name]["HistoFolderNameUp"] = histo_name
                    else:
                        result[syst_name] = {"HistoFolderNameUp": histo_name, "Samples": automatic_systematics[histo_name]}
            elif histo_name.endswith("__1down"):
                if histo_name.endswith("_PseudoData__1down"):
                    syst_name = histo_name[:-18]
                    if syst_name in result:
                        result[syst_name]["HistoFolderSubtractNameDown"] = histo_name
                    else:
                        result[syst_name] = {"HistoFolderSubtractNameDown": histo_name, "Samples": automatic_systematics[histo_name]}
                else:
                    syst_name = histo_name[:-7]
                    if syst_name in result:
                        result[syst_name]["HistoFolderNameDown"] = histo_name
                    else:
                        result[syst_name] = {"HistoFolderNameDown": histo_name, "Samples": automatic_systematics[histo_name]}
            else:
                result[syst_name] = {"HistoFolderNameUp": histo_name, "Samples": automatic_systematics[histo_name]}
        
        if contains_generator_syst:
            Logger.log_message("WARNING", "The ROOT files contain generator systematics. These cannot be added automaticaly. Please take a look at it.")

        return result

    def _merge_automatic_and_config_based_systematics_and_resolve_samples_list_and_excludes(self, automatic_systematics_dict : dict[str,dict], config_based_systematics : list[tuple[str,str,dict]]) -> list[tuple[str,str,dict]]:
        """
        Returns a list of systematics blocks. The list contains all systematics from the config file and the automatic systematics.
        The samples list and excludes are resolved.
        """
        result = []
        systematics_present_in_config = {}

        MC_samples_names = [sample.name() for sample in self._all_MC_samples]
        for syst_tuple in config_based_systematics:
            syst_name = syst_tuple[1]
            syst_dict = syst_tuple[2]
            systematics_present_in_config[syst_name] = True
            if syst_name in automatic_systematics_dict:
                automatic_syst_dict = automatic_systematics_dict[syst_name]
                syst_dict["Samples"] += automatic_syst_dict["Samples"]
            if syst_dict["Samples"] == []:
                continue
            TrexSettingsGetter._resolve_samples_list_and_excludes_for_systematics(syst_dict, MC_samples_names)
            result.append(("Systematic", syst_name, syst_dict))

        for syst_name in automatic_systematics_dict:
            if systematics_present_in_config.get(syst_name, False):
                continue
            syst_dict = automatic_systematics_dict[syst_name]
            if syst_dict["Samples"] == []:
                continue
            TrexSettingsGetter._resolve_samples_list_and_excludes_for_systematics(syst_dict, MC_samples_names)
            result.append(("Systematic", syst_name, syst_dict))

        return result

    def get_automatic_systematics_blocks(self, output_root_files_folder : str) -> dict[str,dict]:
        sample_names = [sample.name() for sample in self._all_MC_samples if sample.automaticSystematics()]
        result = self._get_automatic_systematics_dict_systname_to_dict(output_root_files_folder, sample_names)

        # add all other info but do not resolve samples:
        for syst_name in result:
            syst_dict = result[syst_name]
            syst_dict["Title"] = syst_name.replace("_"," ")
            syst_dict["Type"] = "HISTO"
            syst_dict["Symmetrisation"] = "TWOSIDED" if (("HistoFolderNameDown" in syst_dict) and ("HistoFolderNameUp" in syst_dict)) else "ONESIDED"
            syst_dict["Smoothing"] = 40
        return result


    def get_sample_color(self) -> int:
        self._sample_color_counter += 1
        return self._sample_color_counter

    def get_normfactor_dicts(self) -> list:
        normfactor_dicts = self.get_normfactors_from_trex_settings()
        if normfactor_dicts:
            return normfactor_dicts

        if self.run_unfolding:
            return None
        normfactor_dict = {}
        normfactor_dict["Title"] =  '"#mu(signal)"'
        normfactor_dict["Nominal"] =  1
        normfactor_dict["Min"] =  -100
        normfactor_dict["Max"] =  100
        normfactor_dict["Samples"] = self._all_MC_samples[0].name()
        return [("NormFactor", "mu_signal", normfactor_dict)]

    def get_fit_block(self) -> tuple[str,str,dict]:
        result = {}
        fit_dict_settings = self._get_fit_dict()
        result["FitType"]   = fit_dict_settings.get("FitType",  "SPLUSB")
        if self.run_unfolding:
            result["FitType"] = "UNFOLDING"
            result["BinnedLikelihoodOptimization"] = fit_dict_settings.get("BinnedLikelihoodOptimization", "TRUE")
        else:
            result["POIAsimov"] = fit_dict_settings.get("POIAsimov",1)
        result["FitRegion"] = fit_dict_settings.get("FitRegion","CRSR")
        result["FitBlind"]  = fit_dict_settings.get("FitBlind", "True")
        if "UseMinos" in fit_dict_settings:
            result["UseMinos"] = fit_dict_settings["UseMinos"]
        return "Fit", "fit", result

    def get_morphing_block(self) -> tuple[str,str,dict]:
        morphing_dict_settings = {}
        if self.trex_settings_dict:
            morphing_dict_settings = self.trex_settings_dict.get("Morphing", {})
        if morphing_dict_settings:
            return "Morphing","morphing",morphing_dict_settings
        else:
            return {}    

    def get_job_dictionary(self) -> tuple[str,str,dict]:
        dictionary = {}
        job_dict_settings = {}
        if self.trex_settings_dict:
            job_dict_settings = self.trex_settings_dict.get("Job", {})
        job_name = job_dict_settings.get("Name","my_fit")
        dictionary["HistoPath"] = job_dict_settings.get("HistoPath",self._files_path)
        if self.run_unfolding:
            dictionary["AcceptancePath"] = self._files_path
            dictionary["MigrationPath"] = self._files_path
            dictionary["SelectionEffPath"] = self._files_path
        dictionary["Lumi"] = job_dict_settings.get("Lumi",1)
        dictionary["ImageFormat"] = job_dict_settings.get("ImageFormat","pdf")
        dictionary["ReadFrom"] = job_dict_settings.get("ReadFrom","HIST")
        if not self.run_unfolding:
            dictionary["POI"] = job_dict_settings.get("POI","mu_signal")
        dictionary["HistoChecks"] = job_dict_settings.get("HistoChecks","NOCRASH")
        for key in job_dict_settings:
            if key not in dictionary and key != "Name":
                dictionary[key]=job_dict_settings[key]
        return "Job",job_name,dictionary

    def get_unfolding_block(self) -> tuple[str,str,dict]:
        if not self.run_unfolding:
            return None
        unfolding_settings_dict = self._get_unfolding_dict()
        result = {}
        result["NumberOfTruthBins"] = self.unfolding_n_bins
        result["UnfoldNormXSec"] = unfolding_settings_dict.get("UnfoldNormXSec", "TRUE")
        result["DivideByBinWidth"] = unfolding_settings_dict.get("DivideByBinWidth", "TRUE")
        result["DivideByLumi"] = unfolding_settings_dict.get("DivideByLumi", self._total_lumi) # TODO
        result["LogX"] = unfolding_settings_dict.get("LogX", "FALSE")
        result["LogY"] = unfolding_settings_dict.get("LogY", "FALSE")
        result["NominalTruthSample"] = self._nominal_truth_sample
        result["MatrixOrientation"] = "TRUTHONHORIZONTAL"

        block_name = self.unfolding_level + "_" + self.unfolding_variable_truth
        return "Unfolding",block_name, result

    def _get_systematics_blocks_from_config_wo_samples_resolving(self) -> list[tuple[str,str,dict]]:
        samples_cpp_objects_wo_automatic_systematics = [sample_cpp for sample_cpp in self.config_reader.block_general.get_samples_objects() if not sample_cpp.automaticSystematics()]
        systematics_dicts = self.config_reader.systematics_dicts
        all_regions = []
        for region in self._regions_map.values():
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable in variable_cpp_objects:
                region_variable = region.name() + "_" + variable.name().replace("_NOSYS","")
                if not self._match_selected_regions(region_variable):
                    continue
                all_regions.append(region_variable)

        all_MC_samples = []
        for sample in samples_cpp_objects_wo_automatic_systematics:
            if BlockReaderSample.is_data_sample(sample):
                continue
            all_MC_samples.append(sample.name())

        result = []
        for systematic_pair in systematics_dicts:
            syst_cpp_object_up = systematic_pair.get("up", None)
            syst_cpp_object_down = systematic_pair.get("down", None)
            syst_non_empty_cpp_object = syst_cpp_object_up if syst_cpp_object_up else syst_cpp_object_down

            syst_name_up    = syst_cpp_object_up.name()     if syst_cpp_object_up   else ""
            syst_name_down  = syst_cpp_object_down.name()   if syst_cpp_object_down else ""
            non_empty_variation = syst_name_up if syst_name_up else syst_name_down
            two_sided_variation = syst_name_up and syst_name_down

            syst_name = non_empty_variation
            if syst_name_down and syst_name_up:
                common_part = get_strings_common_part(syst_name_up, syst_name_down)
                if common_part:
                    syst_name = common_part
            syst_name = syst_name.strip("_")

            result_dict = {}
            if syst_name_up:
                result_dict["HistoFolderNameUp"] = syst_name_up
            if syst_name_down:
                result_dict["HistoFolderNameDown"] = syst_name_down
            result_dict["Title"] = syst_name.replace("_"," ")
            result_dict["Type"] = "HISTO"
            result_dict["Symmetrisation"] = "TWOSIDED" if two_sided_variation else "ONESIDED"
            result_dict["Smoothing"] = 40
            result.append(("Systematic", syst_name, result_dict))

            # regions
            regions_selected = []
            region_names = vector_to_list(syst_non_empty_cpp_object.regionsNames())
            for region_name in region_names:
                if not region_name in self._regions_map:
                    continue
                region = self._regions_map[region_name]
                variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
                for variable in variable_cpp_objects:
                    variable_name = variable.name()
                    region_variable = region.name() + "_" + variable_name.replace("_NOSYS","")
                    if not self._match_selected_regions(region_variable):
                        continue
                    regions_selected.append(region_variable)
            if len(regions_selected) != len(all_regions):
                if len(regions_selected) > 0.5*len(all_regions):
                    excluded_regions = []
                    for region in all_regions:
                        if not region in regions_selected:
                            excluded_regions.append(region)
                    if excluded_regions:
                        result_dict["Exclude"] = ",".join(excluded_regions)
                else:
                    result_dict["Regions"] = ",".join(regions_selected)

            # samples
            samples_selected = []
            all_MC_samples = [sample for sample in samples_cpp_objects_wo_automatic_systematics if not BlockReaderSample.is_data_sample(sample)]
            for sample in samples_cpp_objects_wo_automatic_systematics:
                if sample.automaticSystematics():
                    continue
                if not sample.hasSystematics(non_empty_variation):
                    continue
                if sample.automaticSystematics():
                    continue
                samples_selected.append(sample.name())
            result_dict["Samples"] = samples_selected

        return result

    def _resolve_samples_list_and_excludes_for_systematics(systematics_dict : dict, MC_samples_names : list[str]) -> dict[str,str]:
        if len(systematics_dict["Samples"]) != len(MC_samples_names):
            if len(systematics_dict["Samples"]) > 0.5*len(MC_samples_names):
                excluded_samples = []
                for sample_name in MC_samples_names:
                    if not sample_name in systematics_dict["Samples"]:
                        excluded_samples.append(sample_name)
                if excluded_samples:
                    if "Exclude" in systematics_dict:
                        systematics_dict["Exclude"] += "," + ",".join(excluded_samples)
                    else:
                        systematics_dict["Exclude"] = ",".join(excluded_samples)
                del systematics_dict["Samples"]
            else:
                systematics_dict["Samples"] = ",".join(systematics_dict["Samples"])
        else:
            del systematics_dict["Samples"]

    def split_systematics_into_inclusive_and_unfolding(self, systematics_tuple : tuple) -> tuple[tuple,tuple]:
        if self._unfolding_MC_samples_names == None:
            return (systematics_tuple, None)
        systematics_name = systematics_tuple[1]
        systematics_dict = systematics_tuple[2]
        samples = systematics_dict.get("Samples", "")
        if samples:
            samples = samples.split(",")
        else:
            samples = [sample.name() for sample in self._all_MC_samples]

        excludes = systematics_dict.get("Exclude", "")
        if excludes:
            excludes = excludes.split(",")
            for exclude in excludes:
                if exclude in samples:
                    samples.remove(exclude)

        samples_inclusive = []
        samples_unfolding = []
        for sample in samples:
            if sample in self._unfolding_MC_samples_names:
                samples_unfolding.append(sample)
            else:
                samples_inclusive.append(sample)

        inclusive_dict = None
        if samples_inclusive:
            inclusive_dict = deepcopy(systematics_dict)
            remove_items(inclusive_dict, "Exclude", self._unfolding_MC_samples_names)
            remove_items(inclusive_dict, "Samples", self._unfolding_MC_samples_names)

        unfolding_dict = None
        if samples_unfolding:
            unfolding_dict = deepcopy(systematics_dict)
            remove_items(unfolding_dict, "Exclude", self._inclusive_MC_samples_names)
            remove_items(unfolding_dict, "Samples", self._inclusive_MC_samples_names)
            histo_folder_up   = unfolding_dict.get("HistoFolderNameUp", None)
            histo_folder_down = unfolding_dict.get("HistoFolderNameDown", None)
            if histo_folder_up:
                del unfolding_dict["HistoFolderNameUp"]
            if histo_folder_down:
                del unfolding_dict["HistoFolderNameDown"]
            if "Type" in unfolding_dict:
                del unfolding_dict["Type"]

            if histo_folder_up:
                unfolding_dict["MigrationFolderNameUp"] = histo_folder_up
                unfolding_dict["AcceptanceFolderNameUp"] = histo_folder_up
                unfolding_dict["SelectionEffFolderNameUp"] = histo_folder_up
            if histo_folder_down:
                unfolding_dict["MigrationFolderNameDown"] = histo_folder_down
                unfolding_dict["AcceptanceFolderNameDown"] = histo_folder_down
                unfolding_dict["SelectionEffFolderNameDown"] = histo_folder_down

        result_inclusive = None
        if inclusive_dict:
            result_inclusive = ("Systematic", systematics_name, inclusive_dict)

        result_unfolding = None
        if unfolding_dict:
            result_unfolding = ("UnfoldingSystematic", systematics_name, unfolding_dict)

        return (result_inclusive, result_unfolding)