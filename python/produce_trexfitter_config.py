"""
@file Script for producing TRExFitter config file
"""

import os
import sys

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from CommandLineOptions import CommandLineOptions
from BlockReaderGeneral import vector_to_list

sample_color_counter = 2
def get_sample_color(sample_name : str) -> int:
    global sample_color_counter
    sample_color_counter += 1
    return sample_color_counter

def dump_dictionary_to_file(block_type : str, block_name : str, dictionary : dict, file) -> None:
    file.write(block_type + ': "' + block_name + '"\n')
    for key in dictionary:
        value = dictionary[key]
        if type(value) == str:
            contains_spaces = " " in value
            if contains_spaces:
                file.write("\t" + key + ': "' + value + '"\n')
            else:
                file.write("\t" + key + ': ' + value + '\n')
        else:
            file.write("\t" + key + ': ' + str(value) + '\n')
    file.write("\n")

def add_block_comment(block_type : str, file) -> None:
    length = len(block_type) + 8
    file.write("% " + "-"*(length-4) + " %\n")
    file.write("% - " + block_type + " - %\n")
    file.write("% " + "-"*(length-4) + " %\n")
    file.write("\n")

def get_job_dictionary(block_general) -> tuple[str,str,dict]:
    dictionary = {}
    histo_path = block_general.cpp_class.outputPathHistograms()
    if histo_path == "":
        histo_path = "."
    dictionary["HistoPath"] = histo_path
    dictionary["Lumi"] = 1
    dictionary["ImageFormat"] = "pdf"
    dictionary["ReadFrom"] = "HIST"
    return "Job","my_fit",dictionary

def get_region_dictionary(region, variable) -> tuple[str,str,dict]:
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

def get_sample_dictionary(sample, regions_map) -> tuple[str,str,dict]:
    dictionary = {}
    is_data = BlockReaderSample.is_data_sample(sample)
    dictionary["Type"] = "BACKGROUND" if not is_data else "DATA"
    dictionary["Title"] = sample.name()
    dictionary["HistoFile"] = sample.name()
    sample_color = get_sample_color(sample.name())
    dictionary["FillColor"] = sample_color # TODO: FillColor
    dictionary["LineColor"] = sample_color  # TODO: LineColor

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

def get_strings_common_part(str1 : str, str2 : str) -> str:
    result = ""
    for i in range(min(len(str1), len(str2))):
        if str1[i] == str2[i]:
            result += str1[i]
        else:
            return result

def get_systematics_blocks(systematics_dicts : list[dict], samples_cpp_objects : list, regions_map : dict) -> list:
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


if __name__ == "__main__":
    config_path = CommandLineOptions().get_config_path()

    config_reader = ConfigReader(config_path)
    block_general = config_reader.block_general

    with open("trex_test.config","w") as file:
        job_tuple = get_job_dictionary(block_general)
        add_block_comment("JOB", file)
        dump_dictionary_to_file(*job_tuple, file)

        add_block_comment("REGIONS", file)
        regions = config_reader.block_general.get_regions_cpp_objects()
        region_map = {}
        for region in regions:
            region_name = region.name()
            region_map[region_name] = region
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                region_dict = get_region_dictionary(region,variable_cpp_object)
                dump_dictionary_to_file(*region_dict, file)

        add_block_comment("SAMPLES", file)
        samples = config_reader.block_general.get_samples_objects()
        for sample in samples:
            sample_dict = get_sample_dictionary(sample, region_map)
            dump_dictionary_to_file(*sample_dict, file)


        add_block_comment("SYSTEMATICS", file)
        systematics_dicts = config_reader.systematics_dicts
        samples_cpp_objects = config_reader.block_general.get_samples_objects()
        regions_cpp_objects = config_reader.block_general.get_regions_cpp_objects()
        systematics_blocks = get_systematics_blocks(systematics_dicts, samples_cpp_objects, region_map)
        for syst_block in systematics_blocks:
            dump_dictionary_to_file(*syst_block, file)
