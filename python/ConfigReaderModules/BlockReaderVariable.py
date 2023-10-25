from BlockReaderBinning import BlockReaderBinning

class BlockReaderVariable:
    def __init__(self, variable_dict : dict):
        self.name = variable_dict.get("name")
        self.definition = variable_dict.get("definition")
        self.variables = BlockReaderBinning(variable_dict.get("binning"))
