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

from TRExFitterConfigPreparation.TrexSettingsGetter import TrexSettingsGetter, remove_items

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
            use_quotes = " " in value or "#" in value
            if use_quotes:
                file.write("\t" + key + ': "' + value + '"\n')
            else:
                file.write("\t" + key + ': ' + value + '\n')
        else:
            file.write("\t" + key + ': ' + str(value) + '\n')
    file.write("\n")


if __name__ == "__main__":
    cli = CommandLineOptions()
    config_path = cli.get_config_path()

    config_reader = ConfigReader(config_path)
    block_general = config_reader.block_general
    unfolding_settings_tuple = cli.get_unfolding_settings()


    with open(cli.get_trex_fitter_output_config(),"w") as file:
        trex_settings_getter = TrexSettingsGetter(config_reader, cli.get_trex_settings_yaml(), unfolding_settings_tuple)
        add_block_comment("JOB", file)
        job_tuple = trex_settings_getter.get_job_dictionary()
        dump_dictionary_to_file(*job_tuple, file)

        add_block_comment("FIT", file)
        fit_block = trex_settings_getter.get_fit_block()
        dump_dictionary_to_file(*fit_block, file)


        if trex_settings_getter.run_unfolding:
            add_block_comment("UNFOLDING", file)
            unfolding_block = trex_settings_getter.get_unfolding_block()
            dump_dictionary_to_file(*unfolding_block, file)

        add_block_comment("REGIONS", file)
        regions_blocks = trex_settings_getter.get_region_blocks()
        for region_block in regions_blocks:
            dump_dictionary_to_file(*region_block, file)

        if trex_settings_getter.run_unfolding:
            add_block_comment("UNFOLDINGSAMPLES", file)
            unfolding_sample_blocks = trex_settings_getter.get_unfolding_samples_blocks()
            for unfolding_sample_block in unfolding_sample_blocks:
                dump_dictionary_to_file(*unfolding_sample_block, file)

            truth_samples = trex_settings_getter.get_truth_samples_blocks()
            add_block_comment("TRUTHSAMPLES", file)
            for truth_sample in truth_samples:
                dump_dictionary_to_file(*truth_sample, file)

        add_block_comment("SAMPLES", file)
        sample_blocks = trex_settings_getter.get_samples_blocks()
        for sample in sample_blocks:
            dump_dictionary_to_file(*sample, file)


        add_block_comment("NORM. FACTORS", file)
        norm_factor_blocks = trex_settings_getter.get_normfactor_dicts()
        if norm_factor_blocks:
            for norm_factor_block in norm_factor_blocks:
                trex_settings_getter.remove_regions_wo_unfolding(norm_factor_block)
                dump_dictionary_to_file(*norm_factor_block, file)

        add_block_comment("SYSTEMATICS", file)
        systematics_blocks = trex_settings_getter.get_systematics_blocks()
        for syst_block in systematics_blocks:
            dump_dictionary_to_file(*syst_block, file)
