from sys import argv
import os



commands = [
    "python3 python/produce_trexfitter_config.py --c test/configs/config_testing.yml --u ttbar_FS:particle:truth_jet_pt:jet_pt --output config_testing_unfolding.config",
    "python3 python/produce_trexfitter_config.py --c test/configs/config_testing.yml --output config_testing_inclusive.config",
    "python3 python/produce_trexfitter_config.py --c test/configs/config_TRExFitter_test.yml --output config_TRExFitter_test_inclusive.config",
    "python3 python/produce_trexfitter_config.py --c test/configs/config_TRExFitter_test.yml --u ttbar_FS:parton:Ttbar_MC_t_afterFSR_pt:jet_pt --output config_TRExFitter_test_unfolding.config",
]

for command in commands:
    exit_code = os.system(command)
    if exit_code:
        exit(1)


print("\n\n")

files_to_compare = ["config_testing_inclusive.config", "config_testing_unfolding.config", "config_TRExFitter_test_inclusive.config", "config_TRExFitter_test_unfolding.config"]

different_files = []
for file_to_compare in files_to_compare:
    print("Comparing file: ", file_to_compare)
    exit_code = os.system("python3 test/python/compare_two_files.py " + file_to_compare + " test/reference_files/trex_configs/output/" + file_to_compare)
    if exit_code:
        different_files.append(file_to_compare)

if different_files:
    print("\n\nThe following files are different: ", different_files)
    exit(1)
