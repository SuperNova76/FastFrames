"""
@file Source file with BlockReaderCutflow class.
"""
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import CutflowWrapper

from BlockReaderSample import BlockReaderSample
from BlockOptionsGetter import BlockOptionsGetter
from python_wrapper.python.logger import Logger
from CommandLineOptions import CommandLineOptions


class BlockReaderCutflow:
    """!Class for reading Cutflow block of the config, equivalent of C++ class Cutflow
    """

    def __init__(self, input_dict : dict):
        """!Constructor of the BlockReaderCutflow class
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)

        self._name = self._options_getter.get("name", None)
        if self._name is None:
            Logger.log_message("ERROR", "Name not specified in cutflow block")
            exit(1)

        self._selections_list = self._options_getter.get("selections", None, [list], [dict])
        if self._selections_list is None:
            Logger.log_message("ERROR", "No selections specified in cutflow block " + self._name)
            exit(1)

        self._sample_names = self._options_getter.get("samples", None, [list], [str])
        CommandLineOptions().keep_only_selected_samples(self._sample_names)

        unused_options = self._options_getter.get_unused_options()
        if len(unused_options) > 0:
            Logger.log_message("ERROR", "Unused options in cutflow block: " + str(unused_options))
            exit(1)

        self.cpp_class = CutflowWrapper(self._name)

        self.__set_config_reader_cpp()


    def __set_config_reader_cpp(self) -> None:
        """!Set the options in the C++ class
        """
        for selection_tuple in self._selections_list:
            options_getter = BlockOptionsGetter(selection_tuple)
            selection = options_getter.get("selection", None, [str])
            if selection is None:
                Logger.log_message("ERROR", "No selection specified in cutflow block " + self._name)
                exit(1)

            title = options_getter.get("title", None, [str])
            if title is None:
                Logger.log_message("ERROR", "No title specified in cutflow block " + self._name)
                exit(1)

            self.cpp_class.addSelection(selection, title)

    def adjust_samples(self, samples : dict[str, BlockReaderSample]) -> None:
        # if list of samples is not defined, add all samples
        if self._sample_names is None:
            for sample_name in samples:
                sample_cpp = samples[sample_name].cpp_class
                sample_cpp.addCutflow(self.cpp_class.getPtr())
        else:
            for sample_name in self._sample_names:
                if sample_name not in samples:
                    Logger.log_message("ERROR", "Sample " + sample_name + " not found in cutflow block " + self._name)
                    exit(1)
                sample_cpp = samples[sample_name].cpp_class
                sample_cpp.addCutflow(self.cpp_class.getPtr())

    def print_cutflow(cutflow_shared_ptr : int, indent : str = "") -> None:
        """!Print cutflow
        @param cutflow_shared_ptr: shared pointer to the Cutflow object
        @param indent: indentation
        """
        cpp_class = CutflowWrapper("")
        cpp_class.constructFromSharedPtr(cutflow_shared_ptr)

        selections_definitions = cpp_class.selectionsDefinition()
        selections_titles = cpp_class.selectionsTitles()

        print(indent + cpp_class.name())
        for i in range(len(selections_definitions)):
            print(indent + "- selection:" + selections_definitions[i])
            print(indent + "  title:" + selections_titles[i])