from ROOT import TFile
import os
import argparse

class Metadata:
    def __init__(self):
        self.dsid        = 0
        self.campaign    = ""
        self.data_type = "mc"

    def get_metadata_tuple(self):
        return (self.dsid, self.campaign, self.data_type)

def get_list_of_root_files_in_folder(folder_path : str) -> list:
    return [os.path.join(folder_path, file) for file in os.listdir(folder_path) if file.endswith(".root")]

def get_metadata_string(root_file : TFile, key : str) -> str:
    tnamed_object = root_file.Get(key)
    if tnamed_object == None:
        raise Exception("Could not find key {} in file {}".format(key, root_file.GetName()))
    return tnamed_object.GetTitle()

def get_file_metadata(file_path : str) -> Metadata:
    metadata = Metadata()
    root_file = TFile(file_path)
    metadata.dsid        = int(get_metadata_string(root_file, "dsid"))
    metadata.campaign    = get_metadata_string(root_file, "campaign")
    metadata.data_type = get_metadata_string(root_file, "dataType")
    root_file.Close()
    return metadata

def produce_filelist(root_files_folder : str, filelist_address : str) -> None:
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


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--root_files_folder",  help="Path to folder containing root files")
    parser.add_argument("--output_path",        help="Address of the output filelist", nargs = '?', default="")
    args = parser.parse_args()
    root_files_folder = args.root_files_folder
    output_path = args.output_path if args.output_path != "" else root_files_folder + "/filelist.txt"

    produce_filelist(root_files_folder, output_path)
