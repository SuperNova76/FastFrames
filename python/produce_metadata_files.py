#!/bin/env python3

"""!Script to produce filelist.txt and sum_of_weights.txt files from a folder containing root files.
It will loop over all root files in root_files_folder, and produce filelist and sum_of_weights files for all samples, where sample is characterized by (dsid, campaign, data_type).

Usage:

    python python/produce_metadata_files.py --root_files_folder <path_to_root_files_folder> --output_path <path_to_output_folder>

if output_path is not specified, it will be the same as root_files_folder
"""

import argparse
from produce_filelist import produce_filelist
from produce_sum_weights_file import produce_sum_of_weights_file

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files")
    parser.add_argument("--output_path",        help="Path to the folder with output text files", nargs = '?', default="")
    args = parser.parse_args()
    root_files_folder = args.root_files_folder
    output_path = args.output_path if args.output_path != "" else root_files_folder
    filelist_path = output_path + "/filelist.txt"
    sum_of_weights_path = output_path + "/sum_of_weights.txt"

    produce_filelist(root_files_folder, filelist_path)
    produce_sum_of_weights_file(filelist_path, sum_of_weights_path)
