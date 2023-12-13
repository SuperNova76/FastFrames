"""
@file Script for producing TRExFitter config file
"""

import os
import sys
import argparse

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigSettingWrapper, FastFramesExecutor
from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from CommandLineOptions import CommandLineOptions

def dump_dictionary_to_file(block_type : str, block_name : str, dictionary : dict, file) -> None:
    file.write(block_type + ': "' + block_name + '"\n')
    for key in dictionary:
        value = dictionary[key]
        if type(value) == str:
            file.write("\t" + key + ': "' + value + '"\n')
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
    region_name = region.name() + "_" + variable.name()
    dictionary["Type"] = "SIGNAL"
    dictionary["NumberOfRecoBins"] = variable.axisNbins()
    dictionary["VariableTitle"] = variable.name() # TODO: proper title
    dictionary["HistoName"] = "NOSYS/" + variable.name() + "_" + region.name()
    dictionary["Label"] = region_name       # TODO: proper label
    dictionary["ShortLabel"] = region_name  # TODO: proper label
    return "Region", region_name, dictionary

def get_sample_dictionary(sample) -> tuple[str,str,dict]:
    dictionary = {}
    is_data = BlockReaderSample.is_data_sample(sample)
    dictionary["Type"] = "BACKGROUND" if not is_data else "DATA"
    dictionary["Title"] = sample.name()
    dictionary["HistoFile"] = sample.name()
    # TODO: FillColor
    # TODO: LineColor

    return "Sample", sample.name(), dictionary

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
        for region in regions:
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                region_dict = get_region_dictionary(region,variable_cpp_object)
                dump_dictionary_to_file(*region_dict, file)

        add_block_comment("SAMPLES", file)
        samples = config_reader.block_general.get_samples_objects()
        for sample in samples:
            sample_dict = get_sample_dictionary(sample)
            dump_dictionary_to_file(*sample_dict, file)

