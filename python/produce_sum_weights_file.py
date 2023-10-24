from ROOT import TFile, TTree
from sys import argv
import os

def read_filelist(main_folder : str) -> str:
    filelist = {}
    with open(main_folder + "/filelist.txt") as f:
        for line in f.readlines():
            elements = line.split()
            key = tuple(elements[:-1])
            file_name = elements[-1]
            if key not in filelist:
                filelist[key] = []
            filelist[key].append(file_name)
    return filelist

def get_variation_name(histo_name : str) -> str:
    elements = histo_name.split("_")
    if len(elements) < 4:
        return ""
    if elements[0] != "CutBookkeeper":
        return ""
    if not (elements[1].isdigit() and elements[2].isdigit()):
        return ""
    return "_".join(elements[3:])

def get_sum_of_weights_for_single_file(root_file : str) -> dict:
    result = {}
    root_file = TFile(root_file)
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

def produce_sum_of_weights_file(root_files_folder : str) -> None:
    sample_map = {}
    if len(argv) != 2:
        raise Exception("Usage: python produce_filelist.py <folder_path>")
    filelist = read_filelist(root_files_folder)
    with open(root_files_folder + "/sum_of_weights.txt", "w") as sum_of_weights_file:
        for sample, root_files in filelist.items():
            MAX_METADATA_ITEM_LENGTHS = [8 for i in range(len(sample))]
            sample_map[sample] = get_sum_of_weights_for_sample(root_files)
            for variation_name, sum_of_weights in sample_map[sample].items():
                for metadata_element in sample:
                    n_spaces = MAX_METADATA_ITEM_LENGTHS[sample.index(metadata_element)] - len(str(metadata_element))
                    sum_of_weights_file.write(str(metadata_element) + n_spaces*" ")
                sum_of_weights_file.write("{} {}\n".format(variation_name, sum_of_weights))

if __name__ == "__main__":
    if len(argv) != 2:
        raise Exception("Usage: python produce_filelist.py <folder_path>")

    root_files_folder = argv[1]
    produce_sum_of_weights_file(root_files_folder)
