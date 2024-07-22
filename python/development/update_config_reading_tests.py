"""!Script for automated updates of the reference files for the config reading tests.
The script has to be run from the root directory of the project.

Usage:
    python3 python/development/update_config_reading_tests.py
"""

import yaml
import os

def get_config_reading_test_dict(gitlab_ci_yaml_address : str) -> dict[str, list[str]]:
    """!read the .gitlab-ci.yml file and return a dictionary with all tests that are related to the ConfigReader.py script
    @param gitlab_ci_yaml_address: path to the .gitlab-ci.yml file

    @return dictionary with test names as keys and their list of commands as values
    """
    result = {}
    with open(gitlab_ci_yaml_address, "r") as f:
        data_full = yaml.load(f, Loader=yaml.FullLoader)
        ci_yaml = dict([(x, y) for x, y in data_full.items() if not x.startswith('.')])

        for test_name in ci_yaml:
            test_dict = ci_yaml[test_name]

            if type(test_dict) != dict:
                continue

            if test_dict.get('stage','') != 'compare_results':
                continue

            commands = test_dict.get('script',[])
            is_config_reader_test = False
            for command in commands:
                if "ConfigReader.py" in command:
                    is_config_reader_test = True
                    break

            if not is_config_reader_test:
                continue

            result[test_name] = test_dict
        return result

def get_run_command(commands : list[str]) -> str:
    """!Loops over the commands and returns the command that runs the ConfigReader.py script
    @param commands: list of commands from the .gitlab-ci.yml file

    @return command that runs the ConfigReader.py script
    """
    for command in commands:
        elements = command.split()
        if elements[0] != 'python3' and elements[0] != 'python':
            continue
        if 'ConfigReader.py' in elements[1]:
            return command
    return ''

def get_reference_file_and_test_output_file(commands : list[str]) -> tuple[str, str]:
    """!Loops over the commands and returns the reference file and the test output file
    @param commands: list of commands from the .gitlab-ci.yml file for a given test

    @return tuple with reference file and test output file
    """
    reference_file = ''
    test_output_file = ''
    for command in commands:
        elements = command.split()
        if len(elements) != 4:
            continue

        if elements[0] != 'python3' and elements[0] != 'python':
            continue

        if not elements[1].endswith("compare_two_files.py"):
            continue

        for element in elements[2:]:
            if "test/reference_files/config_reading/" in element:
                reference_file = element
            else:
                test_output_file = element

    if reference_file == '' or test_output_file == '':
        return None, None

    return reference_file, test_output_file

def get_keyboard_bool(question : str) -> bool:
    """!Asks a question and returns True if the answer is 'y' and False if the answer is 'n'
    @param question: question to ask the user

    @return True if the answer is 'y', False if the answer is 'n', exits with code 1 otherwise
    """
    answer = input(question)
    answer = answer.strip().lower()
    if answer == 'y':
        return True
    elif answer == 'n':
        return False
    else:
        print("Only 'y' or 'n' is accepted.")
        exit(1)

if __name__ == "__main__":
    test_dictionary = get_config_reading_test_dict('.gitlab-ci.yml')

    for test_name in test_dictionary:
        test_dict = test_dictionary[test_name]
        run_command = get_run_command(test_dict['script'])
        reference_file, test_output_file = get_reference_file_and_test_output_file(test_dict['script'])
        if run_command == '' or reference_file == '' or test_output_file == '':
            print(f"Error: Could not read {test_name} dictionary.")
            continue

        print("\n\n\nChecking diff for test:", test_name)

        os.system(run_command)
        files_different = os.system(f"python3 test/python/compare_two_files.py {reference_file} {test_output_file}")

        if files_different == 0:
            continue

        answer = get_keyboard_bool(f"The files for test '{test_name}' are different. Do you want to update the test? (y/n): ")
        if not answer:
            continue
        else:
            print("Updating the test.")
            os.system(f"cp {test_output_file} {reference_file}")

        print("\n\n")


