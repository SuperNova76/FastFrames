from sys import path,argv
import os

def set_paths():
    set_path_to_shared_lib()
    set_import_path()
    set_main_directory_path()

def set_path_to_shared_lib():
    path_to_shared_lib = find_path_to_shared_libs()
    path.append(path_to_shared_lib)

def set_import_path():
    path_to_main_dir = find_path_to_main_dir()
    path.append(path_to_main_dir + "/python/ConfigReaderModules/")

def set_main_directory_path():
    path_to_main_dir = find_path_to_main_dir()
    path.append(path_to_main_dir)

def all_paths_exist(paths_list : list, path_to_main_dir : str):
    for path in paths_list:
        if not os.path.exists(path_to_main_dir + "/" + path):
            return False
    return True

def find_path_to_main_dir():
    files_to_check = ["python/ConfigReaderModules/BlockReaderCommon.py", "CMakeLists.txt", "FastFrames"]
    paths_to_check = [".", "../", "../../", "../../../"]

    for path_to_check in paths_to_check:
        if all_paths_exist(files_to_check, path_to_check):
            return path_to_check

    raise ValueError("Could not find path to main directory, please run this script from the main directory")

def find_path_to_shared_libs():
    path_to_main_dir = find_path_to_main_dir()
    paths_to_check = ["build/lib", "bin/lib"]
    for path_to_check in paths_to_check:
        if os.path.exists(path_to_main_dir + "/" + path_to_check):
            return path_to_main_dir + "/" + path_to_check
    raise ValueError("Could not find path to shared libraries, please run this script from the main directory")