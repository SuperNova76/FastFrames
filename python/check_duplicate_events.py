#!/bin/env python3

"""!Script for checking duplicate events in the input root files, based on runNumber and eventNumber.
"""

from ROOT import TFile
import os, sys
import argparse


this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import DuplicateEventChecker, DoubleVector, StringVector
from python_wrapper.python.logger import Logger
from produce_filelist import get_list_of_root_files_in_folder, get_file_metadata, Metadata

def get_metadata_to_list_of_root_files_dict(root_files_folder : str) -> dict[tuple, list[str]]:
    """!Get dictionary with metadata tuple as key and list of root files as value
    @param root_files_folder: folder with root files
    @return dict
    """
    result = {}
    root_files = get_list_of_root_files_in_folder(root_files_folder)
    for root_file in root_files:
        metadata = get_file_metadata(root_file)
        if metadata.get_metadata_tuple() in result:
            result[metadata.get_metadata_tuple()].append(root_file)
        else:
            result[metadata.get_metadata_tuple()] = [root_file]
    return result

def check_duplicate_events_in_folder(root_files_folder : str):
    """!Check duplicate events in the input root files
    @param root_files_folder: folder with root files
    """
    metadata_to_list_of_root_files_dict = get_metadata_to_list_of_root_files_dict(root_files_folder)
    for metadata_tuple, root_files in metadata_to_list_of_root_files_dict.items():
        root_files_vector = StringVector()
        for root_file in root_files:
            root_files_vector.append(root_file)
        duplicate_event_checker = DuplicateEventChecker(root_files_vector)
        duplicate_event_checker.checkDuplicateEntries()

        duplicate_run_numbers = duplicate_event_checker.duplicateRunNumbers()
        duplicate_event_numbers = duplicate_event_checker.duplicateEventNumbers()

        if len(duplicate_event_numbers) == 0:
            continue
        elif len(duplicate_event_numbers) < 10:
            Logger.log_message("WARNING", "Duplicate events found in unique sample: " + str(metadata_tuple) + " list of their event numbers and run numbers:")
            for i in range(len(duplicate_event_numbers)):
                Logger.log_message("WARNING", "\trunNumber: " + str(duplicate_run_numbers[i]) + "\t\teventNumber: " + str(duplicate_event_numbers[i]))
        else:
            Logger.log_message("WARNING", "Duplicate events found in unique sample: " + str(metadata_tuple) + " number of duplicate events: " + str(len(duplicate_event_numbers)))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files")
    args = parser.parse_args()
    check_duplicate_events_in_folder(args.root_files_folder)
