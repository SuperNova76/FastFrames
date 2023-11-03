from copy import deepcopy
from python_wrapper.python.logger import Logger

class BlockOptionsGetter:
    """
    Class used to extract options defined in a block of the config file.
    It works like a dictionary, but allows to check if all options from config were used in the code.
    """
    def __init__(self, config_dict : dict):
        self.config = config_dict
        self.option_used = deepcopy(config_dict)
        for option in self.option_used:
            self.option_used[option] = False


    def get(self, option : str, default_value = None):
        if option not in self.config:
            return default_value
        self.option_used[option] = True
        return self.config[option]

    def get_unused_options(self):
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
    """
    Class used to extract options defined in variation block of the systematics.
    Since the up and down variations are read separately, but their properties are defined in the same block,
    one cannot use BlockOptionsGetter to read them and check for unused options.
    """
    def __init__(self, config_dict : dict):
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

    def get(self, option : str, variation : str, default_value = None):
        key = option + "_"*(len(option) != 0) + variation
        if option in self.option_used:
            self.option_used[option] = True
        return self.config.get(key, default_value)

    def get_unused_options(self):
        result = []
        for option in self.option_used:
            if not self.option_used[option]:
                result.append(option)
        return result

