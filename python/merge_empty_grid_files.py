#!/bin/env python3

"""!Script for merging files from GRID with empty trees - this is needed to avoid the problem with empty trees in the GRID files - RDF cannot properly handle them
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

def build_model_file_from_empty_files(empty_files : list, trees_to_check : list[str]):
    """
    Build a file with the correct tree structure from the empty files.
    This file will be used as the model when calling hadd.
    This function goes tree by tree:
    If none of the files contains the tree, skip the tree.
    If none of the files have a tree with structure, write an empty tree.
    """
    model_file_name = "model_file.root"
    model_file = TFile.Open(model_file_name, "RECREATE")
    for tree_name in trees_to_check:
        model_tree = None
        for ifileName in empty_files:
            ifile = TFile.Open(ifileName, "READ")
            tree = ifile.Get(tree_name)
            if tree == None: # Skip the file if it does not contain the tree
                continue
            if tree.GetEntries() != 0: # Copy the tree structure if it has entries
                model_tree = tree.CloneTree(0)
                break # We found the tree with entries - no need to check the rest of the files
            if tree.GetEntries() == 0: # If the tree has no entries, copy the structure and keep looking the other files.
                model_tree = tree.CloneTree(0)

        if model_tree != None: # Copy the tree to the model file if the tree is found in one of the files
            model_file.cd()
            model_tree.Write()

    model_file.Close()
    model_file.Save()

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
    """
    Check if the file contains empty trees from the list of trees passed as argument
    """
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

def has_trees_wihtout_structure(file_address : str, tree_names : list[str]) -> bool:
    """
    Check if the file contains trees (from the list of trees passed as argument) that do not have any branches.
    """
    file = TFile(file_address,"READ")
    for tree_name in tree_names:
        tree = file.Get(tree_name)
        if tree == None:
            continue
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

def merge_files(files_from_unique_sample : list[str], remove_original_files : bool, legacy_mode = False) -> bool:
    """
    Merge empty files from the same sample
    @param files_from_unique_sample: list of files from the same sample
    @param remove_original_files: if True, the original files will be removed, unless one file is buggy
    @param legacy_mode: if True, the merging will be done in the legacy mode. In legacy mode the code expects that there is at least one tree in each file, even if it is an empty tree.

    @return: bool - True if everything went fine, False if at least one file was buggy
    """
    if len(files_from_unique_sample) <= 1: # nothing to merge
        return True

    if not legacy_mode:
        # This work under the assumption that all files containing a tree, that tree will have structure.
        # So we only really care about separating the files that are completely empty from the ones that are not.
        trees_to_check = ["reco", "truth", "particleLevel"]
        empty_files = [file for file in files_from_unique_sample if not has_at_least_one_tree(file, trees_to_check)]
        first_non_empty_file = None
        at_least_one_buggy_file = False # Buggy files here are defined as files with trees that do not have any structure
        for file in files_from_unique_sample:
            if has_trees_wihtout_structure(file, trees_to_check):
                Logger.log_message("ERROR", "File {} contains a tree without structure. This seems like a bug upstream, please check it carefully!".format(file))
                at_least_one_buggy_file = True

            if file not in empty_files and first_non_empty_file == None:
                first_non_empty_file = file

        # if all files are empty, we still merge them
        if first_non_empty_file == None:
            first_non_empty_file = empty_files[0]
            empty_files = empty_files[1:]

        if len(empty_files) != 0:
            merged_file_name = first_non_empty_file[:-5] + "_merged.root" # Put the model file first such that hadd takes the structure from it
            command = "hadd " + " " + merged_file_name + " " + first_non_empty_file + " " + " ".join(empty_files)
            os.system(command)

            if remove_original_files and not at_least_one_buggy_file:
                for file in empty_files:
                    os.system("rm {}".format(file))
                    Logger.log_message("DEBUG", "Removing empty file: {}".format(file))
                os.system("rm {}".format(first_non_empty_file))
                Logger.log_message("DEBUG", "Removing  first non-empty file: {}".format(file))

        return not at_least_one_buggy_file
    else:
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
            build_model_file_from_empty_files(files_from_unique_sample, trees_to_check)
            merged_file_name = first_non_empty_file[:-5] + "_merged.root" # Put the model file first such that hadd takes the structure from it
            command = "hadd " + " " + merged_file_name + " " + "model_file.root" + " " + first_non_empty_file + " " + " ".join(empty_files)
            os.system(command)

            if remove_original_files and not at_least_one_buggy_file:
                os.system("rm model_file.root")
                for file in empty_files:
                    os.system("rm {}".format(file))
                    Logger.log_message("DEBUG", "Removing empty file: {}".format(file))
                os.system("rm {}".format(first_non_empty_file))
                Logger.log_message("DEBUG", "Removing  first non-empty file: {}".format(file))

        return not at_least_one_buggy_file

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files", default=None)
    parser.add_argument("--legacy-tct-output",  help="From TopCPToolkit (TCT) v2.12.0 empty trees are not written to the outputs. Use this flag if your files where produced with older TCT.",action="store_true")
    parser.add_argument("--log_level", help="Logging level", default="INFO")
    args = parser.parse_args()
    root_files_folder   = args.root_files_folder
    log_level           = args.log_level

    Logger.set_log_level(log_level)

    # Give information about the legacy mode and define the merging function
    legacy_mode = False
    if args.legacy_tct_output:
        Logger.log_message("WARNING", "You are using the legacy mode of this script. This is only needed if your files were produced with an older version than TopCPToolkit v2.12.0.")
        Logger.log_message("WARNING", "See https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/169")
        legacy_mode = True
    else :
        Logger.log_message("INFO", "Youre using the new mode of this script. This is the default mode since TopCPToolkit v2.12.0.")

    # check if the input is correct
    if root_files_folder is None:
        Logger.log_message("ERROR", "root_files_folder option not specified")

    file_dictionary = get_file_dictionary(root_files_folder)
    buggy_samples = []
    for metadata_tuple, files_from_unique_sample in file_dictionary.items():
        Logger.log_message("INFO", "Processing sample: {}".format(metadata_tuple))
        Logger.log_message("DEBUG", "List of all files for sample: {}".format("\n\t\t".join(files_from_unique_sample)))
        sample_contains_buggy_files = (not merge_files(files_from_unique_sample, True, legacy_mode)) # This is where the merging happens
        if sample_contains_buggy_files:
            buggy_samples.append(metadata_tuple)

    if len(buggy_samples) > 0:
        if not legacy_mode:
            Logger.log_message("ERROR", "This is the list of samples with at least one file with an empty reco, truth or particleLevel tree. The original files for them have not been removed. You should check the inputs for samples:\n {}".format(buggy_samples))
        else:
            Logger.log_message("ERROR", "This is the list of samples with at least one file without any of reco, truth and particleLevel trees. The original files for them have not been removed. You should check the inputs for samples:\n {}".format(buggy_samples))
        exit(1)