"""!Script for comparing two ROOT files.
It will compare all histograms and trees in the files and if any difference is found, it will print out the first difference and return a non-zero exit code.
Two identical files will return a zero exit code and print out success message.

Usage:
    python3 compare_two_root_files.py <file1> <file2>
"""
from ROOT import TFile, TTree, TH1D, TH2D, TDirectory
import sys

def floats_are_equal(val1 : float, val2 : float, tolerance : float = 1e-4) -> bool:
    """
    Compare two floats and return True if they are equal within the given tolerance
    """
    if val1 != val1 and val2 != val2:
        return True
    return abs(val1 - val2) < tolerance

def test_compare_trees(tree_1 : TTree, tree_2 : TTree) -> str:
    # Get the list of branch names for each tree
    branch_names1 = [b.GetName() for b in tree_1.GetListOfBranches()]
    branch_names2 = [b.GetName() for b in tree_2.GetListOfBranches()]

    # check if the trees have the same branches
    branch_names1.sort()
    branch_names2.sort()
    if branch_names1 != branch_names2:
        return f"Branch names in tree '{tree_1.GetName()}' are different: {branch_names1} != {branch_names2}"

    # Find the common branch names for both trees
    common_branch_names = list(set(branch_names1) & set(branch_names2))

    if tree_1.GetEntries() != tree_2.GetEntries():
        return f"Tree '{tree_1.GetName()}' has different number of entries: {tree_1.GetEntries()} != {tree_2.GetEntries()}"


    dict_eventNumber_to_entry_tree2 = {}
    for i in range(tree_2.GetEntries()):
        tree_2.GetEntry(i)
        event_number = getattr(tree_2, "eventNumber")
        dict_eventNumber_to_entry_tree2[event_number] = i

    # Loop over all entries in the trees and compare the common branch values
    for i in range(tree_1.GetEntries()):
        tree_1.GetEntry(i)
        event_number = tree_1.eventNumber
        if event_number not in dict_eventNumber_to_entry_tree2:
            return f"Event number {event_number} in tree '{tree_1.GetName()}' is not found in tree '{tree_2.GetName()}'"
        tree_2.GetEntry(dict_eventNumber_to_entry_tree2[event_number])
        for branch_name in common_branch_names:
            val1 = getattr(tree_1, branch_name)
            val2 = getattr(tree_2, branch_name)
            if not floats_are_equal(val1, val2):
                return f"Branch '{branch_name}' in tree '{tree_1.GetName()}' has different values for entry {i}: {val1} != {val2}"
    return None

def compare_1d_histograms(hist1 : TH1D, hist2 : TH1D) -> str:
    """
    Compare two TH1D histograms. If they are different, return a string explaining the first difference found. If identical, return None
    """
    if hist1.GetNbinsX() != hist2.GetNbinsX():
        return "Histograms have different number of bins"
    for i in range(hist1.GetNbinsX()+2):
        if not floats_are_equal(hist1.GetBinContent(i), hist2.GetBinContent(i)):
            return f"Histograms have different bin content at bin {i}: {hist1.GetBinContent(i)} != {hist2.GetBinContent(i)}"
    return None

def compare_2d_histograms(hist1 : TH2D, hist2 : TH2D) -> str:
    """
    Compare two TH2D histograms. If they are different, return a string explaining the first difference found. If identical, return None
    """
    if hist1.GetNbinsX() != hist2.GetNbinsX():
        return "Histograms have different number of bins in x"
    if hist1.GetNbinsY() != hist2.GetNbinsY():
        return "Histograms have different number of bins in y"
    for i in range(hist1.GetNbinsX()+2):
        for j in range(hist1.GetNbinsY()+2):
            if not floats_are_equal(hist1.GetBinContent(i, j), hist2.GetBinContent(i, j)):
                return f"Histograms have different bin content at bin ({i}, {j}): {hist1.GetBinContent(i, j)} != {hist2.GetBinContent(i, j)}"
    return None

def get_list_of_folders(file : TFile) -> list[str]:
    """
    Get the list TDirectories names in a TFile
    """
    folders = []
    for key in file.GetListOfKeys():
        if key.GetClassName() == "TDirectoryFile":
            folders.append(key.GetName())
    return folders

def get_list_of_objects_in_tdirectory(tdirectory : TDirectory, object_type : str) -> list[str]:
    """
    Get names of all objects of a given type in a TDirectory
    """
    objects = []
    for key in tdirectory.GetListOfKeys():
        if key.GetClassName() == object_type:
            objects.append(key.GetName())
    return objects

def compare_histograms_in_folder(tfile1 : TFile, tfile2 : TFile, folder : str, histo_type : str = "TH1D"):
    """
    Compare all histograms in a folder of two TFiles and return a string explaining the first difference found. If identical, return None
    """
    # Get the list of histograms in the folder
    histograms1 = get_list_of_objects_in_tdirectory(tfile1.Get(folder), histo_type)
    histograms2 = get_list_of_objects_in_tdirectory(tfile2.Get(folder), histo_type)

    # check if the files have the same histograms
    histograms1.sort()
    histograms2.sort()
    if histograms1 != histograms2:
        return f"Histograms in folder {folder} are different: {histograms1} != {histograms2}"

    # choose the comparison function based on the histogram type
    compare_function = None
    if histo_type == "TH1D":
        compare_function = compare_1d_histograms
    elif histo_type == "TH2D":
        compare_function = compare_2d_histograms
    else:
        raise ValueError(f"Unknown histogram type {histo_type}")

    # Loop over all histograms and compare them
    for hist_name in histograms1:
        hist1 = tfile1.Get(f"{folder}/{hist_name}")
        hist2 = tfile2.Get(f"{folder}/{hist_name}")
        result = compare_function(hist1, hist2)
        if result:
            return f"Histograms {hist_name} in folder {folder} have different values: {result}"
    return None

def compare_all_histograms_in_files(file1 : TFile, file2 : TFile, folders_to_ignore : list[str] = []) -> str:
    """
    Loop over all folders in the files and compare the histograms. If they are different, return a string explaining the first difference found. If identical, return None
    """
    # Get the list of folders in each file
    folders1 = get_list_of_folders(file1)
    folders2 = get_list_of_folders(file2)

    # remove ignored folders
    for folder in folders_to_ignore:
        if folder in folders1:
            folders1.remove(folder)
        if folder in folders2:
            folders2.remove(folder)

    # check if the files have the same folders
    folders1.sort()
    folders2.sort()
    if folders1 != folders2:
        f"Folders in file {file1} are different: {folders1} != {folders2}"

    # Loop over all folders and compare the histograms
    for folder in folders1:
        comparison_1D = compare_histograms_in_folder(file1, file2, folder, "TH1D")
        if comparison_1D:
            return comparison_1D
        comparison_2D = compare_histograms_in_folder(file1, file2, folder, "TH1D")
        if comparison_2D:
            return comparison_2D

    return None

def compare_all_trees_in_files(file1 : TFile, file2 : TFile, trees_to_ignore : list[str]) -> str:
    """
    Compare all trees, except for trees_to_ignore, in two ROOT files and return a string explaining the first difference found. If identical, return None
    """
    # Get the list of trees in each file
    trees1 = get_list_of_objects_in_tdirectory(file1, "TTree")
    trees2 = get_list_of_objects_in_tdirectory(file2, "TTree")

    # remove ignored trees
    for tree in trees_to_ignore:
        if tree in trees1:
            trees1.remove(tree)
        if tree in trees2:
            trees2.remove(tree)

    # check if the files have the same trees
    trees1.sort()
    trees2.sort()
    if trees1 != trees2:
        return f"Trees in file {file1} are different: {trees1} != {trees2}"

    # Loop over all trees and compare them
    for tree_name in trees1:
        tree1 = file1.Get(tree_name)
        tree2 = file2.Get(tree_name)
        result = test_compare_trees(tree1, tree2)
        if result:
            return result
    return None

def test_compare_files(file1 : str, file2 : str) -> str:
    """
    Compare two ROOT files, if they are not identical, return a string explaining the first difference found. If identical, return None
    """
    tfile1 = TFile.Open(file1)
    tfile2 = TFile.Open(file2)

    if not tfile1 or tfile1.IsZombie():
        return f"File {file1} could not be opened"
    if not tfile2 or tfile2.IsZombie():
        return f"File {file2} could not be opened"

    # Compare all histograms in the files
    comparison_histograms = compare_all_histograms_in_files(tfile1, tfile2, [])
    if comparison_histograms:
        return comparison_histograms

    # Compare all trees in the files
    comparison_trees = compare_all_trees_in_files(tfile1, tfile2, [])
    if comparison_trees:
        return comparison_trees

    # Close the ROOT files
    tfile1.Close()
    tfile2.Close()

    return None


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python test_compare_trees.py <file1> <file2>")
        sys.exit(1)

    comparison_result = test_compare_files(sys.argv[1], sys.argv[2])
    if comparison_result:
        print(comparison_result)
        sys.exit(1)
    else:
        print("Files are identical")