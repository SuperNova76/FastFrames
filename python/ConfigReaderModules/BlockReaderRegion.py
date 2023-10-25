from BlockReaderVariable import BlockReaderVariable

class BlockReaderRegion:
    def __init__(self, input_dict : dict):
        self.name = input_dict.get("name")
        self.selection = input_dict.get("selection")
        self.variables = []
        for variable in input_dict.get("variables"):
            self.variables.append(BlockReaderVariable(variable))