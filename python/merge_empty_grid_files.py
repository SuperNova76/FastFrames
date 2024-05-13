#!/bin/env python3

"""!Script for mering files from GRID with empty reco tree - this is needed to avoid the problem with empty reco trees in the GRID files - RDF cannot properly handle them
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

def has_at_least_one_tree(file_address : str, tree_names : list[str]) -> bool:
    """
    Check if the file contains at least one of the trees from the list - to catch upstream bugs
    """
    file = TFile(file_address,"READ")
    for tree_name in tree_names:
        tree = file.Get(tree_name)
        if tree != None:
            return True
    return False

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

def merge_files(files_from_unique_sample : list[str], remove_original_files : bool) -> bool:
    """
    Merge empty files from the same sample
    @param files_from_unique_sample: list of files from the same sample
    @param remove_original_files: if True, the original files will be removed, unless one file is buggy

    @return: bool - True if everything went fine, False if at least one file was buggy
    """
    if len(files_from_unique_sample) <= 1: # nothing to merge
        return

    trees_to_check = ["reco", "truth", "particleLevel"]
    empty_files = [file for file in files_from_unique_sample if has_empty_trees(file, trees_to_check)]
    first_non_empty_file = None
    at_least_one_buggy_file = False
    for file in files_from_unique_sample:
        if not has_at_least_one_tree(file, trees_to_check):
            Logger.log_message("ERROR", "File {} does not contain any of the trees: {}. This seems like a bug upstream, please check it carefully!".format(file, trees_to_check))
            at_least_one_buggy_file = True

        if file not in empty_files and first_non_empty_file == None:
            first_non_empty_file = file

    # if all files are empty, we still need to merge them
    if first_non_empty_file == None:
        first_non_empty_file = empty_files[0]
        empty_files = empty_files[1:]

    if len(empty_files) != 0:
        merged_file_name = first_non_empty_file[:-5] + "_merged.root"
        command = "hadd " + " " + merged_file_name + " " + first_non_empty_file + " " + " ".join(empty_files)
        os.system(command)

        if remove_original_files and not at_least_one_buggy_file:
            for file in empty_files:
                os.system("rm {}".format(file))
            os.system("rm {}".format(first_non_empty_file))

    return not at_least_one_buggy_file

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files", default=None)
    args = parser.parse_args()
    root_files_folder   = args.root_files_folder

    # check if the input is correct
    if root_files_folder is None:
        Logger.log_message("ERROR", "root_files_folder option not specified")

    file_dictionary = get_file_dictionary(root_files_folder)
    buggy_samples = []
    for metadata_tuple, files_from_unique_sample in file_dictionary.items():
        print("Processing sample: ", metadata_tuple)
        print("Files: ", files_from_unique_sample)
        sample_contains_buggy_files = not merge_files(files_from_unique_sample, True)
        if sample_contains_buggy_files:
            buggy_samples.append(metadata_tuple)

    if len(buggy_samples) > 0:
        Logger.log_message("ERROR", "This is the list of samples with at least one file without any of reco,truth and particleLevel trees. The original files for them have not been removed. You should check the inputs for samples:\n {}".format(buggy_samples))
        exit(1)