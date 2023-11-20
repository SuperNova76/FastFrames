"""!Script/module to produce sum_of_weights.txt file from a filelist.txt file, it is imported by python/produce_metadata_files.py
"""
from ROOT import TFile, TTree
from sys import argv
import sys
import os
import argparse

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import SumWeightsGetter, DoubleVector, StringVector

def read_filelist(filelist_path : str) -> dict:
    """!Reads the filelist.txt file and returns it as a dictionary metadata_tuple -> list of root files
    @param filelist_path: path to the filelist.txt file
    @return dictionary metadata_tuple -> list of root files
    """
    filelist = {}
    with open(filelist_path) as f:
        for line in f.readlines():
            elements = line.split()
            key = tuple(elements[:-1])
            file_name = elements[-1]
            if key not in filelist:
                filelist[key] = []
            filelist[key].append(file_name)
    return filelist

def get_variation_name(histo_name : str) -> str:
    """!Get variation name from the histogram name
    @param histo_name: histogram name
    @return variation name - str
    """
    elements = histo_name.split("_")
    if len(elements) < 4:
        return ""
    if elements[0] != "CutBookkeeper":
        return ""
    if not (elements[1].isdigit() and elements[2].isdigit()):
        return ""
    return "_".join(elements[3:])

def get_sum_of_weights_for_single_file(root_file : str) -> dict:
    """!Get sum of weights for a single root file
    @param root_file: path to the root file
    @return dictionary variation_name -> sum_of_weights
    """
    result = {}
    root_file = TFile.Open(root_file)
    # loop over all objects in file
    for key in root_file.GetListOfKeys():
        if key.GetClassName() == "TH1F":
            histogram = key.ReadObj()
            histogram_name = histogram.GetName()
            variation_name = get_variation_name(histogram_name)
            if variation_name == "":
                continue
            sum_of_weights = histogram.GetBinContent(2)
            result[variation_name] = sum_of_weights
    root_file.Close()
    return result

def get_sum_of_weights_for_sample(root_files : list) -> dict:
    """!Get sum of weights for a sample
    @param root_files: list of paths to root files
    @return dictionary variation_name -> sum_of_weights
    """
    result = {}
    result_initialized = False
    for root_file in root_files:
        sum_of_weights_for_single_file = get_sum_of_weights_for_single_file(root_file)
        for variation_name, sum_of_weights in sum_of_weights_for_single_file.items():
            if variation_name not in result:
                if result_initialized:
                    raise Exception("Variation {} found in file {} but not in other files".format(variation_name, root_file))
                result[variation_name] = 0
            result[variation_name] += sum_of_weights
        result_initialized = True
    return result

def produce_sum_of_weights_file(filelist_path : str, output_path : str) -> None:
    """!Produce sum_of_weights.txt file from the input filelist.txt file
    @param filelist_path: path to the filelist.txt file
    @param output_path: path to the output sum_of_weights.txt file
    """
    sample_map = {}
    filelist = read_filelist(filelist_path)
    with open(output_path, "w") as sum_of_weights_file:
        for sample, root_files in filelist.items():
            MAX_METADATA_ITEM_LENGTHS = [8 for i in range(len(sample))]
            root_files_vector = StringVector()
            for root_file in root_files:
                root_files_vector.append(root_file)
            sum_weights_getter = SumWeightsGetter(root_files_vector)
            sum_weights_values = sum_weights_getter.getSumWeightsValues()
            sum_weights_names  = sum_weights_getter.getSumWeightsNames()

            for i in range(len(sum_weights_values)):
                variation_name = sum_weights_names[i]
                sum_of_weights = sum_weights_values[i]
                for metadata_element in sample:
                    n_spaces = MAX_METADATA_ITEM_LENGTHS[sample.index(metadata_element)] - len(str(metadata_element))
                    sum_of_weights_file.write(str(metadata_element) + n_spaces*" ")
                sum_of_weights_file.write("{} {}\n".format(variation_name, sum_of_weights))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--filelist_path", help="Path to the filelist")
    parser.add_argument("--output_path", help="Path the output metadata file", nargs = '?', default="")
    args = parser.parse_args()
    filelist_path = args.filelist_path
    output_path = args.output_path if args.output_path != "" else "/".join(filelist_path.split("/")[:-1]) + "/sum_of_weights.txt"

    produce_sum_of_weights_file(filelist_path, output_path)
