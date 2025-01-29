#!/bin/env python3

"""!Script for producing filelist.txt from a folder containing root files, it can be also imported as a module.
"""

from ROOT import TFile
import os, sys
import argparse

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

class Metadata:
    """!Python class for storing metadata (DSID, campaign and data_type) of a root file. It is used in produce_filelist.py
    """
    def __init__(self):
        """!Constructor of the Metadata class
        """
        ## DSID of the sample (int)
        self.dsid        = 0

        ## Campaign of the sample (string)
        self.campaign    = ""

        ## Data type of the sample (string)
        self.data_type = "mc"

    def get_metadata_tuple(self):
        """!Returns a tuple of (dsid, campaign, data_type)
        """
        return (self.dsid, self.campaign, self.data_type)

def get_list_of_root_files_in_folder(folder_path : str) -> list:
    """!Get list of all root files in the input folder, if it contains a folder, go recursively through it
    @param folder_path: path to the folder
    @return list of paths to root files
    """
    result = [os.path.abspath(os.path.join(folder_path, file)) for file in os.listdir(folder_path) if file.endswith(".root")]
    for directory in os.listdir(folder_path):
        if os.path.isdir(os.path.join(folder_path, directory)):
            result += get_list_of_root_files_in_folder(os.path.join(folder_path, directory))
    return result

def get_metadata_string(root_file : TFile, key : str) -> str:
    """!Get given metadata as string from the input ROOT file
    @param root_file: input ROOT file
    @param key: key of the metadata
    @return str
    """
    tnamed_object = root_file.Get(key)
    if tnamed_object == None:
        raise Exception("Could not find key {} in file {}".format(key, root_file.GetName()))
    return tnamed_object.GetTitle()

def get_file_metadata_old_format(file_path : str) -> Metadata:
    """!Get metadata object of the input ROOT file
    @param file_path: path to the ROOT file
    @return metadata object
    """
    metadata = Metadata()
    root_file = TFile.Open(file_path)
    metadata.dsid        = int(get_metadata_string(root_file, "dsid"))
    metadata.campaign    = get_metadata_string(root_file, "campaign")
    metadata.data_type = get_metadata_string(root_file, "dataType")
    root_file.Close()
    return metadata


def get_file_metadata(file_path : str) -> Metadata:
    """!Get metadata object of the input ROOT file
    @param file_path: path to the ROOT file
    @return metadata object
    """
    metadata = Metadata()
    root_file = TFile.Open(file_path)
    metadata_histo = root_file.Get("metadata")
    if metadata_histo == None:
        Logger.log_message("WARNING", "No metadata histogram in file {}".format(file_path))
        return get_file_metadata_old_format(file_path)
        exit()
    metadata.data_type   = metadata_histo.GetXaxis().GetBinLabel(1)
    metadata.campaign    = metadata_histo.GetXaxis().GetBinLabel(2)
    metadata.dsid        = int(metadata_histo.GetXaxis().GetBinLabel(3))
    root_file.Close()
    return metadata

def produce_filelist(root_files_folder : str, filelist_address : str) -> None:
    """!Produce filelist from all the root files in the input root_files_folder. The filelist will be saved to filelist_address
    @param root_files_folder: path to the folder containing root files
    @param filelist_address: path to the output filelist
    """
    sample_map = {}

    root_files = get_list_of_root_files_in_folder(root_files_folder)
    for root_file in root_files:
        metadata = get_file_metadata(root_file)
        metadata_tuple = metadata.get_metadata_tuple()
        if metadata_tuple not in sample_map:
            sample_map[metadata_tuple] = []
        sample_map[metadata_tuple].append(root_file)

    MAX_METADATA_ITEM_LENGTHS = [8 for i in range(len(metadata_tuple))]
    with open(filelist_address, "w") as filelist:
        for metadata_tuple, root_files in sample_map.items():
            for root_file in root_files:
                for metadata_element in metadata_tuple:
                    n_spaces = MAX_METADATA_ITEM_LENGTHS[metadata_tuple.index(metadata_element)] - len(str(metadata_element))
                    filelist.write(str(metadata_element) + n_spaces*" ")
                filelist.write("{}\n".format(root_file))

def produce_filelist_grid(grid_paths_dict : dict, filelist_address : str) -> None:
    """!Produce filelist from all the root files in the input root_files_folder. The filelist will be saved to filelist_address
    @param root_files_folder: path to the folder containing root files
    @param filelist_address: path to the output filelist
    """
    # First clean the file if it already exists
    with open(filelist_address, "w") as filelist:
        filelist.write("")
    counter = 1 # To keep track of the progress
    for grid_sample, grid_paths in grid_paths_dict.items():
        Logger.log_message("INFO", "Creating file list for sample"+grid_sample+"... "+str(counter)+"/"+str(len(grid_paths_dict)))

        sample_map = {}

        root_files = grid_paths
        for root_file in root_files:
            metadata = get_file_metadata(root_file)
            metadata_tuple = metadata.get_metadata_tuple()
            if metadata_tuple not in sample_map:
                sample_map[metadata_tuple] = []
            sample_map[metadata_tuple].append(root_file)

        MAX_METADATA_ITEM_LENGTHS = [8 for i in range(len(metadata_tuple))]
        with open(filelist_address, "a+") as filelist:
            for metadata_tuple, root_files in sample_map.items():
                for root_file in root_files:
                    for metadata_element in metadata_tuple:
                        n_spaces = MAX_METADATA_ITEM_LENGTHS[metadata_tuple.index(metadata_element)] - len(str(metadata_element))
                        filelist.write(str(metadata_element) + n_spaces*" ")
                    filelist.write("{}\n".format(root_file))
        counter += 1

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files")
    parser.add_argument("--output_path",        help="Address of the output filelist", nargs = '?', default="")
    args = parser.parse_args()
    root_files_folder = args.root_files_folder
    output_path = args.output_path if args.output_path != "" else root_files_folder + "/filelist.txt"

    produce_filelist(root_files_folder, output_path)
