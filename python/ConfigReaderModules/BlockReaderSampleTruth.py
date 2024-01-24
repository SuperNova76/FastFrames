"""
@file Source file with BlockReaderSampleTruth class.
"""
from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

from ConfigReaderCpp    import TruthWrapper, RegionWrapper
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderSystematic import BlockReaderSystematic
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderVariable import BlockReaderVariable


class BlockReaderSampleTruth:
    """!Class for reading truth block of sample block in the config, equivalent of C++ class Truth
    """

    def __init__(self, input_dict : dict):
        """!Constructor of the BlockReaderSampleTruth class.
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)

        self._name = self._options_getter.get("name", None, [str])
        if self._name is None:
            Logger.log_message("ERROR", "No name specified for truth block: " + str(self._options_getter))
            exit(1)

        self._truth_tree_name = self._options_getter.get("truth_tree_name", None, [str])
        if self._truth_tree_name is None:
            Logger.log_message("ERROR", "No truth_tree_name specified for truth block {}".format(self._name))
            exit(1)

        self._selection = self._options_getter.get("selection", "true")

        self._event_weight = self._options_getter.get("event_weight", None, [str])
        if self._event_weight is None:
            Logger.log_message("ERROR", "No event_weight specified for truth block {}".format(self._name))
            exit(1)

        self._match_variables = self._options_getter.get("match_variables", [], [list], [dict])

        self._variables = self._options_getter.get("variables", [], [list], [dict])

        self._produce_unfolding = self._options_getter.get("produce_unfolding", False, [bool])

        self._define_custom_columns = self._options_getter.get("define_custom_columns", [], [list], [dict])

        ## Instance of the TruthWrapper C++ class -> wrapper around C++ Truth class
        self.cpp_class = TruthWrapper(self._name)

        self._set_cpp_class()

        self._check_unused_options()
        self._read_variables()
        self._read_match_variables()

    def check_reco_variables_existence(self, reco_variables : list) -> None:
        """!Check if all reco variables used in match_variables are defined for at least one region
        """
        for match_variable_dict in self._match_variables:
            reco = match_variable_dict.get("reco")
            if reco not in reco_variables:
                Logger.log_message("ERROR", "Reco variable {} specified in truth block {} does not exist".format(reco, self._name))
                exit(1)

    def _set_cpp_class(self):
        self.cpp_class.setTruthTreeName(self._truth_tree_name)
        self.cpp_class.setSelection(self._selection)
        self.cpp_class.setEventWeight(self._event_weight)
        self.cpp_class.setProduceUnfolding(self._produce_unfolding)

        if self._define_custom_columns:
            for custom_column_dict in self._define_custom_columns:
                custom_column_opts = BlockOptionsGetter(custom_column_dict)
                name        = custom_column_opts.get("name", None, [str])
                definition  = custom_column_opts.get("definition", None, [str])
                if name is None or definition is None:
                    Logger.log_message("ERROR", "Invalid custom column definition for truth block {}".format(self._name))
                    exit(1)
                self.cpp_class.addCustomDefine(name, definition)


    def _read_variables(self) -> None:
        for variable_dict in self._variables:
            variable = BlockReaderVariable(variable_dict)
            self.cpp_class.addVariable(variable.cpp_class.getPtr())

    def _read_match_variables(self) -> None:
        for match_variable_dict in self._match_variables:
            options_getter = BlockOptionsGetter(match_variable_dict)
            reco = options_getter.get("reco", None, [str])
            truth = options_getter.get("truth", None, [str])
            if reco is None or truth is None:
                Logger.log_message("ERROR", "No reco or truth specified for match variable in truth block {}".format(self._name))
                exit(1)
            self.cpp_class.addMatchVariables(reco, truth)

    def _check_unused_options(self) -> None:
        unused = self._options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in truth block is not supported!".format(unused))
            exit(1)

    def get_custom_defines(self) -> list:
        """!Get vector of custom defined variables
        """
        return [x for x in self.cpp_class.customDefines()]