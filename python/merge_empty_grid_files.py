#!/bin/env python3

"""!Script for mering files from GRID with empty reco tree - this is needed
"""

from ROOT import TFile
import os, sys
import argparse

# to be able to import from the same directory
this_file_path = os.path.abspath(__file__)
this_folder_path = os.path.dirname(this_file_path)
sys.path.append(this_folder_path)

from produce_filelist import Metadata, get_file_metadata, get_list_of_root_files_in_folder

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger


def has_empty_trees(file_address : str, tree_names : list[str]) -> bool:
    file = TFile(file_address,"READ")
    for tree_name in tree_names:
        tree = file.Get(tree_name)
        if tree == None:
            continue
        if tree.GetEntries() == 0:
            return True
        if len(tree.GetListOfBranches()) == 0:
            return True
    return False

def get_file_dictionary(folder_address : str) -> dict[Metadata, list[str]]:
    result = {}
    file_list = get_list_of_root_files_in_folder(folder_address)
    for file in file_list:
        metadata = get_file_metadata(file)
        metadata_tuple = metadata.get_metadata_tuple()
        if metadata_tuple not in result:
            result[metadata_tuple] = []
        result[metadata_tuple].append(file)
    return result

def merge_files(files_from_unique_sample : list[str], remove_original_files : bool):
    if len(files_from_unique_sample) <= 1: # nothing to merge
        return

    empty_files = [file for file in files_from_unique_sample if has_empty_trees(file, ["reco", "truth", "particleLevel"])]
    first_non_empty_file = None
    for file in files_from_unique_sample:
        if file not in empty_files:
            first_non_empty_file = file
            break

    if first_non_empty_file == None:
        raise Exception("All files are empty in the sample")

    merged_file_name = first_non_empty_file[:-5] + "_merged.root"
    command = "hadd " + " " + merged_file_name + " " + first_non_empty_file + " " + " ".join(empty_files)
    os.system(command)

    if remove_original_files:
        for file in empty_files:
            os.system("rm {}".format(file))
        os.system("rm {}".format(first_non_empty_file))

if __name__ == "__main__":


    parser = argparse.ArgumentParser()
    parser.add_argument("--delete_empty_files", help="Should empty files be deleted after merging", default=None)
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files", default=None)
    args = parser.parse_args()
    root_files_folder   = args.root_files_folder
    delete_empty_files  = args.delete_empty_files

    # check if the input is correct
    if root_files_folder is None:
        Logger.log_message("ERROR", "root_files_folder option not specified")

    if delete_empty_files is not None:
        if type(delete_empty_files) != str:
            Logger.log_message("ERROR", "delete_empty_files must be string ('true' or 'false')")
        if delete_empty_files.lower() == "true":
            delete_empty_files = True
        elif delete_empty_files.lower() == "false":
            delete_empty_files = False
        else:
            Logger.log_message("ERROR", "delete_empty_files option not specified. Please specify True or False")


    if delete_empty_files is None:
        Logger.log_message("ERROR", "delete_empty_files option not specified. Please specify True or False")

    file_dictionary = get_file_dictionary(root_files_folder)
    for metadata_tuple, files_from_unique_sample in file_dictionary.items():
        merge_files(files_from_unique_sample, delete_empty_files)