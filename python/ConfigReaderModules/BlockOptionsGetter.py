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
        self.config = config_dict
        self.option_used = deepcopy(config_dict)
        for option in self.option_used:
            self.option_used[option] = False


    def get(self, option : str, default_value = None, allowed_types : list = None):
        """!Get option from the config file. If the option is not present, return default_value. If the option is present, check if it matches allowed_types.
        @param option: option to be read
        @param default_value: value to be returned if the option is not present
        @param allowed_types: list of allowed types for the option
        """
        if option not in self.config:
            return default_value
        self.option_used[option] = True
        return_value = self.config[option]
        if allowed_types is not None and type(return_value) not in allowed_types:
            Logger.log_message("ERROR", "Option {} has invalid type: {} (allowed types: {})".format(option, type(return_value), allowed_types))
            exit(1)
        return self.config[option]

    def get_unused_options(self):
        """!Get list of options that were not read
        """
        result = []
        for option in self.option_used:
            if not self.option_used[option]:
                result.append(option)
        return result

    # define in operator
    def __contains__(self, option : str):
        return option in self.config

    # define [] operator
    def __getitem__(self, option : str):
        return self.config[option]

    def __str__(self):
        return str(self.config)



class VariationsOptionsGetter:
    """!Class used to extract options defined in variation block of the systematics. Since the up and down variations are read separately, but their properties are defined in the same block, one cannot use BlockOptionsGetter to read them and check for unused options.
    """

    def __init__(self, config_dict : dict):
        """!Constructor of the VariationsOptionsGetter class
        @param config_dict: dictionary with options from the config file
        """
        self.config = config_dict
        self.option_used = {}
        for option in self.config:
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
            self.option_used[option_wo_variation_suffix] = False

    def get(self, option : str, variation : str, default_value = None, allowed_types : list = None):
        """!Get option for the given variation from the config file. If the option is not present, return default_value. If the option is present, check if it matches allowed_types.
        @param option: option to be read
        @param variation: variation to be read (up or down)
        @param default_value: value to be returned if the option is not present
        @param allowed_types: list of allowed types for the option
        """
        key = option + "_"*(len(option) != 0) + variation
        if key not in self.config:
            return default_value
        self.option_used[option] = True
        return_value = self.config.get(key, default_value)
        if allowed_types is not None and type(return_value) not in allowed_types:
            Logger.log_message("ERROR", "Option {} has invalid type: {} (allowed types: {})".format(option, type(return_value), allowed_types))
            exit(1)
        return return_value

    def get_unused_options(self):
        """!Get list of options that were not read"""
        result = []
        for option in self.option_used:
            if not self.option_used[option]:
                result.append(option)
        return result

