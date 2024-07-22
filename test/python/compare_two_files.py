from sys import argv
from os import system

import hashlib

if __name__ == "__main__":
    if len(argv) < 3:
        print("Usage: python compare_two_files.py <file1> <file2>")
        exit(1)

    hash1 = hashlib.sha256(open(argv[1],'rb').read())
    hash2 = hashlib.sha256(open(argv[2],'rb').read())

    if hash1.hexdigest() == hash2.hexdigest():
        print("Files are identical")
        exit()

    print("Files are different:\n\n")

    print("Diff:")
    colordiff_installed = system("command -v colordiff > /dev/null") == 0

    if colordiff_installed:
        system("colordiff {} {}".format(argv[1], argv[2]))
    else:
        system("diff {} {}".format(argv[1], argv[2]))
    exit(1)