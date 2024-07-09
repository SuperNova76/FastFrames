#!/bin/env python3

"""!Script to produce filelist.txt and sum_of_weights.txt files from a folder containing root files.
It will loop over all root files in root_files_folder, and produce filelist and sum_of_weights files for all samples, where sample is characterized by (dsid, campaign, data_type).

Usage:

    python python/produce_metadata_files.py --root_files_folder <path_to_root_files_folder> --output_path <path_to_output_folder>

if output_path is not specified, it will be the same as root_files_folder
"""
import sys,os
import argparse
from produce_filelist import produce_filelist,produce_filelist_grid
from produce_sum_weights_file import produce_sum_of_weights_file
from check_duplicate_events import check_duplicate_events_in_folder

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

def createGridFileList():
    import subprocess
    # Check that rucio has been setup by user
    proc = subprocess.Popen(["rucio --version"], stdout=subprocess.PIPE,  stderr=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    if proc.returncode!=0:
        Logger.log_message("ERROR", "Check that you have set up rucio and have a valid grid certificate.")
        sys.exit(1)

    # Ask user in which RSE to use when looking for the files
    Logger.log_message("INFO", "Please specify the RSE to look for the files")
    print("Rucio Storage Element: ")
    rucioRSE = input() # One example -> UKI-NORTHGRID-MAN-HEP_LOCALGROUPDISK
    gridDatasets = {}
    # Open the text file
    with open(args.grid_datasets, "r") as f:
        for line in f:
            # Append the grid dataset to the list if it is not commented out
            if not line.startswith("#"):
                gridDatasets[line.strip()] = []

    # Assign each grid dataset to a list of grid paths
    for gridSample in gridDatasets:
        Logger.log_message("INFO", "Getting grid paths for "+gridSample+"...")
        proc = subprocess.Popen(["rucio list-file-replicas --protocol root --pfns --rse "+rucioRSE+" "+gridSample], stdout=subprocess.PIPE,  stderr=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()

        # Get the grid paths
        gridPathsPerSample = [str(i)[2:-1] for i in out.splitlines()] # Remove b' and ' at the beginning and end.
        if len(gridPathsPerSample)==0:
            Logger.log_message("ERROR", "No grid paths found for "+gridSample)
            sys.exit(1)
        gridDatasets[gridSample] = gridPathsPerSample

    return gridDatasets



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    fileLocation = parser.add_mutually_exclusive_group()
    fileLocation.add_argument("--root_files_folder",  help="Path to folder containing root files")
    fileLocation.add_argument("--grid_datasets",help="Path to a text file containing grid paths.")
    parser.add_argument("--output_path",        help="Path to the folder with output text files", nargs = '?', default="")
    parser.add_argument("--sum_weights_histo", help="Name of sum of weights files. Default is empty string, which means it will use Cutbookkeeper histograms with corresponding suffixes", nargs = '?', default="")
    parser.add_argument("--check_duplicates",  help="Check for duplicate events in the root files", default="False")
    args = parser.parse_args()

    check_duplicates = args.check_duplicates.upper() == "TRUE"

    histo_name = args.sum_weights_histo
    # If user has local root files
    if args.grid_datasets is None:
        if args.root_files_folder is None:
            Logger.log_message("ERROR", "Please specify root_files_folder argument")
            sys.exit(1)
        root_files_folder = args.root_files_folder
        output_path = args.output_path if args.output_path != "" else root_files_folder
        filelist_path = output_path + "/filelist.txt"
        sum_of_weights_path = output_path + "/sum_of_weights.txt"

        produce_filelist(root_files_folder, filelist_path)
        produce_sum_of_weights_file(filelist_path, sum_of_weights_path, histo_name)

        if check_duplicates:
            check_duplicate_events_in_folder(root_files_folder)

    # Otherwise, user has grid datasets
    else :
        gPaths = createGridFileList()
        output_path = args.output_path if args.output_path != "" else ""
        filelist_path = output_path + "/filelist.txt"
        sum_of_weights_path = output_path + "/sum_of_weights.txt"
        produce_filelist_grid(gPaths,filelist_path)
        produce_sum_of_weights_file(filelist_path, sum_of_weights_path, histo_name)
