from sys import path,argv
import os

def set_paths():
    """
    Set all import paths for the python scripts and C++ shared libraries
    """
    set_path_to_shared_lib()
    set_import_path()
    set_main_directory_path()

def set_path_to_shared_lib():
    """
    Set path to the shared libraries
    """
    path_to_shared_lib = find_path_to_shared_libs()
    path.append(path_to_shared_lib)

def set_import_path():
    """
    Set path to the python scripts
    """
    path_to_main_dir = find_path_to_main_dir()
    path.append(path_to_main_dir + "/python/ConfigReaderModules/")

def set_main_directory_path():
    """
    Set path to the main directory
    """
    path_to_main_dir = find_path_to_main_dir()
    path.append(path_to_main_dir)

def all_paths_exist(paths_list : list, path_to_main_dir : str):
    """
    Check if all paths exist given a path to the main directory
    @param paths_list: list of paths to check
    @param path_to_main_dir: path to the main directory
    """
    for path in paths_list:
        if not os.path.exists(path_to_main_dir + "/" + path):
            return False
    return True

def find_path_to_main_dir():
    """
    Find path to the main directory
    """
    files_to_check = ["python/ConfigReaderModules/BlockReaderCommon.py", "CMakeLists.txt", "FastFrames"]
    paths_to_check = [".", "../", "../../", "../../../"]

    for path_to_check in paths_to_check:
        if all_paths_exist(files_to_check, path_to_check):
            return path_to_check

    raise ValueError("Could not find path to main directory, please run this script from the main directory")

def find_path_to_shared_libs():
    """
    Find path to the shared libraries
    """
    path_to_main_dir = find_path_to_main_dir()
    paths_to_check = ["build/lib", "bin/lib"]
    for path_to_check in paths_to_check:
        if os.path.exists(path_to_main_dir + "/" + path_to_check):
            return path_to_main_dir + "/" + path_to_check
    raise ValueError("Could not find path to shared libraries, please run this script from the main directory")