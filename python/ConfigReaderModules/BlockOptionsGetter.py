"""
@file Source file with BlockOptionsGetter and VariationsOptionsGetter classes.
"""
from copy import deepcopy
from python_wrapper.python.logger import Logger

class BlockOptionsGetter:
    """!Class used to extract options defined in a block of the config file.
    It works like a dictionary, but allows to check if all options from config were used in the code.
    """

    def __init__(self, config_dict : dict):
        """!Constructor of the BlockOptionsGetter class
        @param self
        @param config_dict: dictionary with options from the config file
        """
        self._config = config_dict
        self._option_used = deepcopy(config_dict)
        for option in self._option_used:
            self._option_used[option] = False


    def get(self, option : str, default_value = None, allowed_types : list = None, allowed_elements : list = None):
        """!Get option from the config file. If the option is not present, return default_value. If the option is present, check if it matches allowed_types.
        @param option: option to be read
        @param default_value: value to be returned if the option is not present
        @param allowed_types: list of allowed types for the option
        @param allowed_elements: if the value is iterable, you can specify allowed types for its elements
        """
        if option not in self._config:
            return default_value
        self._option_used[option] = True
        return_value = self._config[option]
        if allowed_types is not None and type(return_value) not in allowed_types:
            Logger.log_message("ERROR", "Option {} has invalid type: {} (allowed types: {})".format(option, type(return_value), allowed_types))
            exit(1)
        if allowed_elements is not None and hasattr(return_value, "__iter__"):
            for element in return_value:
                if type(element) not in allowed_elements:
                    Logger.log_message("ERROR", "Option {} has invalid type of element: {} (allowed types: {}). The problematic value: {}".format(option, type(element), allowed_elements, return_value))
                    exit(1)
        return self._config[option]

    def get_unused_options(self):
        """!Get list of options that were not read
        """
        result = []
        for option in self._option_used:
            if not self._option_used[option]:
                result.append(option)
        return result

    def __contains__(self, option : str):
        """
        Defines in operator
        """
        return option in self._config

    def __getitem__(self, option : str):
        """
        Defines [] operator
        """
        return self._config[option]

    def __str__(self):
        """
        Conversion to string
        """
        return str(self._config)



class VariationsOptionsGetter:
    """!Class used to extract options defined in variation block of the systematics. Since the up and down variations are read separately, but their properties are defined in the same block, one cannot use BlockOptionsGetter to read them and check for unused options.
    """

    def __init__(self, config_dict : dict):
        """!Constructor of the VariationsOptionsGetter class
        @param config_dict: dictionary with options from the config file
        """
        self._config = config_dict
        self._option_used = {}
        for option in self._config:
            option_wo_variation_suffix = None
            if option.endswith("_up"):
                option_wo_variation_suffix = option[:-3]
            elif option.endswith("_down"):
                option_wo_variation_suffix = option[:-5]
            elif option == "up" or option == "down":
                option_wo_variation_suffix = ""
            else:
                Logger.log_message("Warning", "Unknown variation option: {}".format(option))
                continue
            self._option_used[option_wo_variation_suffix] = False

    def get(self, option : str, variation : str, default_value = None, allowed_types : list = None):
        """!Get option for the given variation from the config file. If the option is not present, return default_value. If the option is present, check if it matches allowed_types.
        @param option: option to be read
        @param variation: variation to be read (up or down)
        @param default_value: value to be returned if the option is not present
        @param allowed_types: list of allowed types for the option
        """
        key = option + "_"*(len(option) != 0) + variation
        if key not in self._config:
            return default_value
        self._option_used[option] = True
        return_value = self._config.get(key, default_value)
        if allowed_types is not None and type(return_value) not in allowed_types:
            Logger.log_message("ERROR", "Option {} has invalid type: {} (allowed types: {})".format(option, type(return_value), allowed_types))
            exit(1)
        return return_value

    def get_unused_options(self):
        """!Get list of options that were not read"""
        result = []
        for option in self._option_used:
            if not self._option_used[option]:
                result.append(option)
        return result

    def use_option(self, option : str):
        """!Mark option as used"""
        self._option_used[option] = True

