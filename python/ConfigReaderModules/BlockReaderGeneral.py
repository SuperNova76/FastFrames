class BlockReaderGeneral:
    def __init__(self, input_dict : dict):
        self.debug_level = input_dict.get("debug_level", "WARNING")
        self.input_filelist_path = input_dict.get("input_filelist_path")
        self.input_sumweights_path = input_dict.get("input_sumweights_path")
        self.output_path = input_dict.get("output_path")
        self.default_sumweights = input_dict.get("default_sumweights", "NOSYS")
        self.default_event_weights = input_dict.get("default_event_weights")
        self.__set_luminosity_map(input_dict.get("luminosity"))

    def __set_luminosity_map(self, luminosity_map : dict) -> None:
        self.luminosity_map = {}
        for key, value in luminosity_map.items():
            self.luminosity_map[key] = float(value)

