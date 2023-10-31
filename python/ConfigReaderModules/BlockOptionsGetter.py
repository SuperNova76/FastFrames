from copy import deepcopy

class BlockOptionsGetter:
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
