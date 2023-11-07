from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import TruthWrapper, RegionWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderVariable import BlockReaderVariable


class BlockReaderSampleTruth:
    def __init__(self, input_dict : dict):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.name = self.options_getter.get("name", None, [str])
        if self.name is None:
            Logger.log_message("ERROR", "No name specified for truth block: " + str(self.options_getter))
            exit(1)

        self.truth_tree_name = self.options_getter.get("truth_tree_name", None, [str])
        if self.truth_tree_name is None:
            Logger.log_message("ERROR", "No truth_tree_name specified for truth block {}".format(self.name))
            exit(1)

        self.selection = self.options_getter.get("selection", "")

        self.event_weight = self.options_getter.get("event_weight", None, [str])
        if self.event_weight is None:
            Logger.log_message("ERROR", "No event_weight specified for truth block {}".format(self.name))
            exit(1)

        self.match_variables = self.options_getter.get("match_variables", None, [list])
        if self.match_variables is None:
            Logger.log_message("ERROR", "No match_variables specified for truth block {}".format(self.name))
            exit(1)

        self.variables = self.options_getter.get("variables", [], [list])
        if self.variables == []:
            Logger.log_message("ERROR", "No variables specified for truth block {}".format(self.name))
            exit(1)

        self.produce_unfolding = self.options_getter.get("produce_unfolding", False, [bool])

        self.define_custom_columns = self.options_getter.get("define_custom_columns", [], [list])

        self.cpp_class = TruthWrapper(self.name)
        self._set_cpp_class()

        self._check_unused_options()
        self._read_variables()
        self._read_match_variables()

    def check_reco_variables_existence(self, reco_variables : list) -> None:
        for match_variable_dict in self.match_variables:
            reco = match_variable_dict.get("reco")
            if reco not in reco_variables:
                Logger.log_message("ERROR", "Reco variable {} specified in truth block {} does not exist".format(reco, self.name))
                exit(1)

    def _set_cpp_class(self):
        self.cpp_class.setTruthTreeName(self.truth_tree_name)
        self.cpp_class.setSelection(self.selection)
        self.cpp_class.setEventWeight(self.event_weight)
        self.cpp_class.setProduceUnfolding(self.produce_unfolding)

        if self.define_custom_columns:
            for custom_column_dict in self.define_custom_columns:
                custom_column_opts = BlockOptionsGetter(custom_column_dict)
                name        = custom_column_opts.get("name", None, [str])
                definition  = custom_column_opts.get("definition", None, [str])
                if name is None or definition is None:
                    Logger.log_message("ERROR", "Invalid custom column definition for truth block {}".format(self.name))
                    exit(1)
                self.cpp_class.addCustomDefine(name, definition)


    def _read_variables(self) -> None:
        for variable_dict in self.variables:
            variable = BlockReaderVariable(variable_dict)
            self.cpp_class.addVariable(variable.cpp_class.getPtr())

    def _read_match_variables(self) -> None:
        for match_variable_dict in self.match_variables:
            options_getter = BlockOptionsGetter(match_variable_dict)
            reco = options_getter.get("reco", None, [str])
            truth = options_getter.get("truth", None, [str])
            if reco is None or truth is None:
                Logger.log_message("ERROR", "No reco or truth specified for match variable in truth block {}".format(self.name))
                exit(1)
            self.cpp_class.addMatchVariables(reco, truth)

    def _check_unused_options(self) -> None:
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in truth block is not supported!".format(unused))
            exit(1)

    def get_custom_defines(self) -> list:
        return [x for x in self.cpp_class.customDefines()]