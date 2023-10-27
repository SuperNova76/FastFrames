from sys import path

def set_path_to_shared_lib():
    path.append("../build/lib")
    path.append("../../build/lib")
    path.append("../bin/lib")
    path.append("../../bin/lib")
    path.append("../../")