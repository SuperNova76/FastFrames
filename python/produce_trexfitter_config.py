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
from CommandLineOptions import CommandLineOptions

from TRExFitterConfigPreparation.TrexSettingsGetter import TrexSettingsGetter

def add_block_comment(block_type : str, file) -> None:
    length = len(block_type) + 8
    file.write("% " + "-"*(length-4) + " %\n")
    file.write("% - " + block_type + " - %\n")
    file.write("% " + "-"*(length-4) + " %\n")
    file.write("\n")

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

if __name__ == "__main__":
    config_path = CommandLineOptions().get_config_path()

    config_reader = ConfigReader(config_path)
    block_general = config_reader.block_general

    with open("trex_test.config","w") as file:
        trex_settings_getter = TrexSettingsGetter(CommandLineOptions().get_trex_settings_yaml())
        add_block_comment("JOB", file)
        job_tuple = trex_settings_getter.get_job_dictionary(block_general)
        dump_dictionary_to_file(*job_tuple, file)

        add_block_comment("FIT", file)
        fit_block = trex_settings_getter.get_fit_block()
        dump_dictionary_to_file(*fit_block, file)


        add_block_comment("REGIONS", file)
        regions = config_reader.block_general.get_regions_cpp_objects()
        region_map = {}
        for region in regions:
            region_name = region.name()
            region_map[region_name] = region
            variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
            for variable_cpp_object in variable_cpp_objects:
                region_dict = trex_settings_getter.get_region_dictionary(region,variable_cpp_object)
                dump_dictionary_to_file(*region_dict, file)

        add_block_comment("SAMPLES", file)
        samples = config_reader.block_general.get_samples_objects()
        for sample in samples:
            sample_dict = trex_settings_getter.get_sample_dictionary(sample, region_map)
            dump_dictionary_to_file(*sample_dict, file)

        add_block_comment("NORM. FACTORS", file)
        norm_factor_blocks = trex_settings_getter.get_normfactor_dicts(samples)
        for norm_factor_block in norm_factor_blocks:
            dump_dictionary_to_file(*norm_factor_block, file)

        #automatic_systematics = trex_settings_getter.get_automatic_systematics_list(".", [x.name() for x in samples], regions)
        #print("Automatic systematics:")
        #for syst in automatic_systematics:
        #    print(syst, ":", automatic_systematics[syst])

        add_block_comment("SYSTEMATICS", file)
        systematics_dicts = config_reader.systematics_dicts
        samples_cpp_objects = config_reader.block_general.get_samples_objects()
        regions_cpp_objects = config_reader.block_general.get_regions_cpp_objects()
        systematics_blocks = trex_settings_getter.get_systematics_blocks(systematics_dicts, samples_cpp_objects, region_map)
        for syst_block in systematics_blocks:
            dump_dictionary_to_file(*syst_block, file)
