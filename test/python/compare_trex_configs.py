from sys import argv
import os

def compare_trex_configs(app_to_use : str, file1 : str, file2 : str) -> bool:
    exit_code = os.system('{}  -I "HistoPath:" -I "AcceptancePath:" -I "MigrationPath:" -I "SelectionEffPath:" -I "TruthDistributionPath:"  {} {}'.format(app_to_use, file1, file2))
    return exit_code != 0


if __name__ == "__main__":
    colordiff_installed = not os.system("which colordiff")
    diff_app = "colordiff" if colordiff_installed else "diff"

    commands = [
        "python3 python/produce_trexfitter_config.py --c test/reference_files/config_reading/config_testing.yml --u ttbar_FS:particle:truth_jet_pt:jet_pt --output config_testing_unfolding.config",
        "python3 python/produce_trexfitter_config.py --c test/reference_files/config_reading/config_testing.yml --output config_testing_inclusive.config",
        "python3 python/produce_trexfitter_config.py --c test/reference_files/config_reading/config_TRExFitter_test.yml --output config_TRExFitter_test_inclusive_with_settings_file.config --trex_settings test/reference_files/trex_configs/trex_settings.yml",
        "python3 python/produce_trexfitter_config.py --c test/reference_files/config_reading/config_TRExFitter_test.yml --output config_TRExFitter_test_inclusive.config",
        "python3 python/produce_trexfitter_config.py --c test/reference_files/config_reading/config_TRExFitter_test.yml --u ttbar_FS:parton:Ttbar_MC_t_afterFSR_pt:jet_pt --output config_TRExFitter_test_unfolding.config",
    ]

    for command in commands:
        exit_code = os.system(command)
        if exit_code:
            exit(1)


    print("\n\n")

    files_to_compare = ["config_testing_inclusive.config", "config_testing_unfolding.config", "config_TRExFitter_test_inclusive.config", "config_TRExFitter_test_unfolding.config", "config_TRExFitter_test_inclusive_with_settings_file.config"]

    different_files = []
    for file_to_compare in files_to_compare:
        print("Comparing file: ", file_to_compare)
        files_differ = compare_trex_configs(diff_app, file_to_compare, "test/reference_files/trex_configs/output/" + file_to_compare)
        if files_differ:
            different_files.append(file_to_compare)

    if different_files:
        print("\n\nThe following files are different: ", different_files)
        exit(1)
