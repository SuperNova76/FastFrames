
import os
import sys
from copy import deepcopy

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
    def __init__(self, config_reader : ConfigReader, trex_settings_yaml : str = ""):
        self.trex_settings_dict = None
        if trex_settings_yaml:
            with open(trex_settings_yaml, "r") as f:
                try:
                    self.trex_settings_dict = yaml.load(f, Loader=yaml.FullLoader)
                except yaml.scanner.ScannerError:
                    Logger.log_message("ERROR", "Cannot parse yaml file {}".format(trex_settings_yaml))
                    exit(1)
        self._sample_color_counter = 2

        self.config_reader = config_reader
        self.run_unfolding = False
        self.unfolding_sample = ""
        self.unfolding_level = ""
        self.unfolding_variable_truth = ""
        self.unfolding_variable_reco = ""
        self.unfolding_n_bins = None

        self._all_MC_samples = None
        self._unfolding_MC_samples_names = None
        self._inclusive_MC_samples_names = None

    def set_unfolding_settings(self, unfolding_settings_tuple : tuple[str,str,str]) -> None:
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

    def _get_all_trexfitter_regions(self, regions_cpp_objects : list) -> list[str]:
        result = []
        for region in regions_cpp_objects:
            region_name = region.name()
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                variable_name = variable_cpp_object.name()
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

    def get_automatic_systematics_list(self, output_root_files_folder : str, sample_names : list, regions_objects : list) -> dict:
        trex_regions_names = self._get_all_trexfitter_regions(regions_objects)
        result = {} # key = systematic name, value = list of samples for which it is defined
        for sample_name in sample_names:
            sample_path = os.path.join(output_root_files_folder, sample_name + ".root")
            if not os.path.exists(sample_path):
                Logger.log_message("ERROR", "Sample {} does not exist".format(sample_name))
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
            root_file.Close
        return result

    def get_automatic_systematics_pairs(self, output_root_files_folder : str, sample_names : list, regions_objects : list) -> dict[str,dict]:
        automatic_systematics = self.get_automatic_systematics_list(output_root_files_folder, sample_names, regions_objects)
        contains_generator_syst = False
        result = {}
        for histo_name in automatic_systematics:
            if histo_name.startswith("GEN_"):
                contains_generator_syst = True
                continue
            syst_name = histo_name
            if histo_name.endswith("__1up"):
                syst_name = histo_name[:-5]
                if syst_name in result:
                    result[syst_name]["HistoFolderNameUp"] = histo_name
                else:
                    result[syst_name] = {"HistoFolderNameUp": histo_name, "Samples": automatic_systematics[histo_name]}
            elif histo_name.endswith("__1down"):
                syst_name = histo_name[:-7]
                if syst_name in result:
                    result[syst_name]["HistoFolderNameDown"] = histo_name
                else:
                    result[syst_name] = {"HistoFolderNameDown": histo_name, "Samples": automatic_systematics[histo_name]}
            else:
                result[syst_name] = {"HistoFolderNameUp": histo_name, "Samples": automatic_systematics[histo_name]}

        if contains_generator_syst:
            Logger.log_message("WARNING", "The ROOT files contain generator systematics. These cannot be added automaticaly. Please take a look at it.")

        # add all other info and resolve samples:
        for syst_name in result:
            syst_dict = result[syst_name]
            syst_dict["Title"] = syst_name.replace("_"," ")
            syst_dict["Type"] = "HISTO"
            syst_dict["Symmetrisation"] = "TWOSIDED" if (("HistoFolderNameDown" in syst_dict) and ("HistoFolderNameUp" in syst_dict)) else "ONESIDED"
            syst_dict["Smoothing"] = 40
            samples = syst_dict["Samples"]
            if len(samples) == len(sample_names):
                del syst_dict["Samples"]
            elif len(samples) < 0.5*len(sample_names):
                syst_dict["Samples"] = ",".join(samples)
            else:
                excluded_samples = []
                del syst_dict["Samples"]
                for sample in sample_names:
                    if not sample in samples:
                        excluded_samples.append(sample)
                syst_dict["Exclude"] = ",".join(excluded_samples)

        return result


    def get_sample_color(self) -> int:
        self._sample_color_counter += 1
        return self._sample_color_counter

    def get_normfactor_dicts(self, samples_cpp_objects : list) -> list:
        normfactor_dicts = self.get_normfactors_from_trex_settings()
        if normfactor_dicts:
            return normfactor_dicts

        normfactor_dict = {}
        normfactor_dict["Title"] =  '"#mu(signal)"'
        normfactor_dict["Nominal"] =  1
        normfactor_dict["Min"] =  -100
        normfactor_dict["Max"] =  100
        normfactor_dict["Samples"] = samples_cpp_objects[0].name()
        return [("NormFactor", "mu_signal", normfactor_dict)]

    def get_fit_block(self) -> tuple:
        result = {}
        fit_dict_settings = self._get_fit_dict()
        result["FitType"]   = fit_dict_settings.get("FitType",  "SPLUSB")
        if self.run_unfolding:
            result["FitType"] = "UNFOLDING"
        result["FitRegion"] = fit_dict_settings.get("FitRegion","CRSR")
        result["POIAsimov"] = fit_dict_settings.get("POIAsimov",1)
        result["FitBlind"]  = fit_dict_settings.get("FitBlind", "True")
        return "Fit", "fit", result

    def get_job_dictionary(self) -> tuple[str,str,dict]:
        block_general = self.config_reader.block_general
        dictionary = {}
        histo_path = block_general.cpp_class.outputPathHistograms()
        if histo_path == "":
            histo_path = "."
        dictionary["HistoPath"] = histo_path
        dictionary["Lumi"] = 1
        dictionary["ImageFormat"] = "pdf"
        dictionary["ReadFrom"] = "HIST"
        dictionary["POI"] = "mu_signal"
        dictionary["HistoChecks"] = "NOCRASH"
        return "Job","my_fit",dictionary

    def get_region_dictionary(self, region, variable) -> tuple[str,str,dict]:
        dictionary = {}
        variable_name = variable.name().replace("_NOSYS","")
        region_name = region.name() + "_" + variable_name
        dictionary["Type"] = "SIGNAL"
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

    def get_unfolding_samples_blocks(self, samples_cpp_objects : list) -> list[tuple]:
        if not self.run_unfolding:
            return []
        result = []
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
                    sample_dict = {}
                    sample_name = sample.name()
                    if sample_name != self.unfolding_sample:
                        sample_dict["Type"] = "GHOST"

                    sample_color = self.get_sample_color()
                    sample_dict["FillColor"] = sample_setting_dict.get("FillColor", sample_color)
                    sample_dict["LineColor"] = sample_setting_dict.get("LineColor", sample_color)
                    sample_dict["Title"] = sample_setting_dict.get("Title", sample.name())

                    sample_dict["AcceptanceFile"] =  sample_name
                    sample_dict["MigrationFile"] =  sample_name
                    sample_dict["SelectionEffFile"] =  sample_name

                    sample_dict["AcceptanceName"]   = "NOSYS/acceptance_eff_" + level + "_" + self.unfolding_variable_reco
                    sample_dict["SelectionEffName"] = "NOSYS/selection_eff_" + level + "_" + truth_variable.name()
                    sample_dict["MigrationName"]    = "NOSYS/" + self.unfolding_variable_reco + "_vs_" + level + "_" + truth_variable.name()

                    result.append(("UnfoldingSample", sample.name(), sample_dict))
                    unfolding_samples_cpp_objects.append(sample.name())

        self._unfolding_MC_samples_names = unfolding_samples_cpp_objects
        return result


    def get_samples_blocks(self, regions_map) -> list[tuple[str,str,dict]]:
        all_samples = self.config_reader.block_general.get_samples_objects()
        result = []
        self._inclusive_MC_samples_names = []
        for sample in all_samples:
            if self._unfolding_MC_samples_names:
                if sample.name() in self._unfolding_MC_samples_names:
                    continue
            self._inclusive_MC_samples_names.append(sample.name())
            sample_tuple = self.get_sample_tuple(sample, regions_map)
            result.append(sample_tuple)
        self._all_MC_samples = all_samples
        return result

    def get_sample_tuple(self, sample, regions_map) -> tuple[str,str,dict]:
        dictionary = {}
        is_data = BlockReaderSample.is_data_sample(sample)
        sample_setting_dict = self._get_sample_dict(sample.name())
        dictionary["Type"] =  sample_setting_dict.get("Type", "BACKGROUND" if not is_data else "DATA")
        dictionary["Title"] = sample_setting_dict.get("Title", sample.name())
        dictionary["HistoFile"] = sample.name()
        sample_color = sample_setting_dict.get("Color", self.get_sample_color())
        dictionary["FillColor"] = sample_setting_dict.get("FillColor", sample_color) # TODO: FillColor
        dictionary["LineColor"] = sample_setting_dict.get("LineColor", sample_color)  # TODO: LineColor

        region_names = vector_to_list(sample.regionsNames())
        selected_regions = []
        all_regions_list = []
        variable_names_defined_for_sample = vector_to_list(sample.variables())
        for region_name in region_names:
            region = regions_map[region_name]
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable in variable_cpp_objects:
                variable_name = variable.name()
                all_regions_list.append(region.name() + "_" + variable_name.replace("_NOSYS",""))
                if not variable_name in variable_names_defined_for_sample:
                    continue
                variable_name = variable_name.replace("_NOSYS","")
                selected_regions.append(region.name() + "_" + variable_name.replace("_NOSYS",""))

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

        return "Sample", sample.name(), dictionary

    def get_systematics_blocks(self, systematics_dicts : list[dict], samples_cpp_objects : list, regions_map : dict) -> list:
        all_regions = []
        for region in regions_map.values():
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable in variable_cpp_objects:
                all_regions.append(region.name() + "_" + variable.name().replace("_NOSYS",""))

        all_MC_samples = []
        for sample in samples_cpp_objects:
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
                region = regions_map[region_name]
                variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
                for variable in variable_cpp_objects:
                    variable_name = variable.name()
                    regions_selected.append(region.name() + "_" + variable_name.replace("_NOSYS",""))
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
            for sample in samples_cpp_objects:
                if not sample.hasSystematics(non_empty_variation):
                    continue
                samples_selected.append(sample.name())
            if len(samples_selected) != len(all_MC_samples):
                if len(samples_selected) > 0.5*len(all_MC_samples):
                    excluded_samples = []
                    for sample in all_MC_samples:
                        if not sample in samples_selected:
                            excluded_samples.append(sample)
                    if excluded_samples:
                        if "Exclude" in result_dict:
                            result_dict["Exclude"] += "," + ",".join(excluded_samples)
                        else:
                            result_dict["Exclude"] = ",".join(excluded_samples)
                else:
                    result_dict["Samples"] = ",".join(samples_selected)

        return result

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