from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

#from ConfigReaderCpp    import TruthWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderVariable import BlockReaderVariable


class BlockReaderSampleTruth:
    def __init__(self, input_dict : dict):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.name = self.options_getter.get("name", None)
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for truth block: " + str(self.options_getter))
            exit(1)

        self.truth_tree_name = self.options_getter.get("truth_tree_name", None)
        if self.truth_tree_name is None:
            Logger.log_message("ERROR", "No truth_tree_name specified for truth block {}".format(self.name))
            exit(1)

        self.selection = self.options_getter.get("selection", "")

        self.event_weight = self.options_getter.get("event_weight", None)
        if self.event_weight is None:
            Logger.log_message("ERROR", "No event_weight specified for truth block {}".format(self.name))
            exit(1)

        self.match_variables = self.options_getter.get("match_variables", None)
        if self.match_variables is None:
            Logger.log_message("ERROR", "No match_variables specified for truth block {}".format(self.name))
            exit(1)

        self.variables = self.options_getter.get("variables", [])
        if self.variables == []:
            Logger.log_message("ERROR", "No variables specified for truth block {}".format(self.name))
            exit(1)

        self._check_unused_options()
        self._read_variables()

    def _read_variables(self) -> None:
        for variable_dict in self.variables:
            variable = BlockReaderVariable(variable_dict)
            # TODO : Add variable object to C++ class

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in truth block is not supported!".format(unused))
            exit(1)