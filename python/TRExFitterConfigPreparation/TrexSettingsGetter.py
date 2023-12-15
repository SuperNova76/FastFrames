
import os
import sys

from ROOT import TFile

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir + "../")

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from BlockReaderGeneral import vector_to_list

from python_wrapper.python.logger import Logger

import yaml

def get_strings_common_part(str1 : str, str2 : str) -> str:
    result = ""
    for i in range(min(len(str1), len(str2))):
        if str1[i] == str2[i]:
            result += str1[i]
        else:
            return result

class TrexSettingsGetter:
    def __init__(self, trex_settings_yaml : str = ""):
        self.trex_settings_dict = None
        if trex_settings_yaml:
            with open(trex_settings_yaml, "r") as f:
                try:
                    self.trex_settings_dict = yaml.load(f, Loader=yaml.FullLoader)
                except yaml.scanner.ScannerError:
                    Logger.log_message("ERROR", "Cannot parse yaml file {}".format(trex_settings_yaml))
                    exit(1)
        self._sample_color_counter = 2

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

    def get_sample_color(self) -> int:
        self._sample_color_counter += 1
        return self._sample_color_counter

    def get_normfactor_dicts(self, samples_cpp_objects : list) -> list:
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
        result["FitRegion"] = fit_dict_settings.get("FitRegion","CRSR")
        result["POIAsimov"] = fit_dict_settings.get("POIAsimov",1)
        result["FitBlind"]  = fit_dict_settings.get("FitBlind", "True")
        return "Fit", "fit", result

    def get_job_dictionary(self, block_general) -> tuple[str,str,dict]:
        dictionary = {}
        histo_path = block_general.cpp_class.outputPathHistograms()
        if histo_path == "":
            histo_path = "."
        dictionary["HistoPath"] = histo_path
        dictionary["Lumi"] = 1
        dictionary["ImageFormat"] = "pdf"
        dictionary["ReadFrom"] = "HIST"
        dictionary["POI"] = "mu_signal"
        return "Job","my_fit",dictionary

    def get_region_dictionary(self, region, variable) -> tuple[str,str,dict]:
        dictionary = {}
        variable_name = variable.name().replace("_NOSYS","")
        region_name = region.name() + "_" + variable_name
        dictionary["Type"] = "SIGNAL"
        dictionary["NumberOfRecoBins"] = variable.axisNbins()
        dictionary["VariableTitle"] = variable_name # TODO: proper title
        dictionary["HistoName"] = "NOSYS/" + variable_name + "_" + region.name()
        dictionary["Label"] = region_name       # TODO: proper label
        dictionary["ShortLabel"] = region_name  # TODO: proper label
        return "Region", region_name, dictionary

    def get_sample_dictionary(self, sample, regions_map) -> tuple[str,str,dict]:
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