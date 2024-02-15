#!/usr/bin/env python3

"""!Script for merging the output ROOT files
"""
import os
import sys

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader
from CommandLineOptions import CommandLineOptions

def get_njobs_and_job_index_from_file_name(file_name : str, sample_name : str) -> tuple[int, int]:
    """! Decode the number of jobs and job index from the file name
    """
    prefix = sample_name + "_Njobs_"
    name_skimmed = file_name[len(prefix):]
    try:
        njobs = int(name_skimmed.split("_")[0])
        job_index = int(name_skimmed.split("_")[2].replace(".root", ""))
        return (njobs, job_index)
    except:
        Logger.log_message("ERROR", "Could not parse the file name: " + file_name)
        exit(1)

def get_sample_name_to_list_of_subjob_outputs(original_root_files : list[str], sample_names : list[str]) -> dict[str, list[str]]:
    """!Get dictionary with sample names as keys and list of subjob outputs as values
    @param original_root_files: list of all original root files
    @param sample_names: list of sample names
    @return dict[str, list[str]]
    """
    result = {}
    for sample_name in sample_names:
        # example name: ttbar_FS_Njobs_4_jobIndex_0.root
        prefix = sample_name + "_Njobs_"
        files_unmerged = [file for file in original_root_files if file.startswith(prefix)]

        # check consistency, i.e. if all files have the same number of jobs and if all job indices are present
        njobs = None
        job_indices = []
        for file in files_unmerged:
            njobs_file, job_index_file = get_njobs_and_job_index_from_file_name(file, sample_name)
            if njobs == None:
                njobs = njobs_file
            else:
                if njobs != njobs_file:
                    Logger.log_message("ERROR", "Inconsistent number of jobs for sample " + sample_name + ". Please clean up the folder and keep only files with the same Njobs parameter")
                    exit(1)
            job_indices.append(job_index_file)

        # check if all job indices are present
        for i_job in range(njobs):
            if i_job not in job_indices:
                Logger.log_message("ERROR", "Missing job index " + str(i_job) + " for sample " + sample_name)
                exit(1)

        result[sample_name] = files_unmerged
    return result

def merge_files(input_files : list[str], output_file : str, unfolding : list[tuple[str,str]] = None, regions : list[str] = None) -> None:
    """!Merge input files into the output file
    @param input_files: list of input files
    @param output_file: output file
    @param unfolding: list of tuples, each containing two strings: reco-level name and truth-level name
    @param regions: list of regions
    """
    command = "hadd -f " + output_file + " " + " ".join(input_files)
    Logger.log_message("DEBUG", "Going to execute: " + command)
    os.system(command)

    if unfolding == None:
        return



if __name__ == "__main__":
    config_path = CommandLineOptions().get_config_path()

    Logger.log_message("INFO", "Going to read the config: " + config_path)
    config_reader = ConfigReader(config_path)

    output_path = config_reader.block_general.cpp_class.outputPathHistograms()
    sample_objects = config_reader.block_general.get_samples_objects()

    original_root_files = [file for file in os.listdir(output_path) if file.endswith(".root")]
    sample_names = [sample_object.name() for sample_object in sample_objects]
    sample_to_unmerged_files_list_dict = get_sample_name_to_list_of_subjob_outputs(original_root_files, sample_names)
    regions = [region.name() for region in config_reader.block_general.get_regions_cpp_objects()]

    for sample_object in sample_objects:
        sample_name = sample_object.name()
        merged_file_address = output_path + "/" + sample_name + ".root"

        unmerged_files = [output_path + "/" + filename for filename in sample_to_unmerged_files_list_dict[sample_name]]
        merge_files(unmerged_files, merged_file_address, [], regions)
