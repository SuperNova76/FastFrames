from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import SampleWrapper
from BlockReaderGeneral import BlockReaderGeneral


class BlockReaderSample:
    def __init__(self, input_dict : dict, block_reader_general : BlockReaderGeneral = None):
        self.name = input_dict.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for sample: " + str(input_dict))
            exit(1)

        self.simulation_type = input_dict.get("simulation_type",None)
        is_data = (self.simulation_type.upper() == "DATA")
        if self.simulation_type is None:
            Logger.log_message("ERROR", "No simulation_type specified for sample {}".format(self.name))
            exit(1)

        self.dsids = input_dict.get("dsids",None)
        if self.dsids is None and not is_data:
            Logger.log_message("ERROR", "No dsids specified for sample {}".format(self.name))
            exit(1)

        self.campaigns = input_dict.get("campaigns",None)
        if self.campaigns is None:
            Logger.log_message("ERROR", "No campaigns specified for sample {}".format(self.name))
            exit(1)

        self.selection = input_dict.get("selection","true")

        self.regions = input_dict.get("regions",None)

        self.event_weights = input_dict.get("event_weights",None)

        self.cpp_class = SampleWrapper(self.name)