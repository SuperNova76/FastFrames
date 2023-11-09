"""
@file Source file with BlockReaderNtuple class.
"""
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import NtupleWrapper

from BlockReaderVariable import BlockReaderVariable
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter
from python_wrapper.python.logger import Logger
from CommandLineOptions import CommandLineOptions


class BlockReaderNtuple:
    """!Class for reading ntuple block of the config, equivalent of C++ class Ntuple
    """

    def __init__(self, input_dict : dict):
        """!Constructor of the BlockReaderNtuple class
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)

        self._samples = self._options_getter.get("samples",None, [list], [str])
        self._exclude_samples = self._options_getter.get("exclude_samples",None, [list], [str])
        CommandLineOptions().keep_only_selected_samples(self._samples)
        CommandLineOptions().keep_only_selected_samples(self._exclude_samples)

        if not self._samples is None and not self._exclude_samples is None:
            Logger.log_message("ERROR", "Both samples and exclude_samples specified for ntuple block")
            exit(1)

        self._selection = self._options_getter.get("selection",None, [str])
        self._regions   = self._options_getter.get("regions",None, [list], [str])
        if self._regions is not None and self._selection is not None:
            Logger.log_message("ERROR", "Both regions and selection specified for ntuple block")
            exit(1)

        self._branches = self._options_getter.get("branches",[], [list], [str])
        self._exclude_branches = self._options_getter.get("exclude_branches",[], [list], [str])
        self._copy_trees = self._options_getter.get("copy_trees",[], [list], [str])

        self._check_unused_options()

        ## Instance of the NtupleWrapper C++ class -> wrapper around C++ Ntuple class
        self.cpp_class = NtupleWrapper()

        self.__set_config_reader_cpp()

    def __set_config_reader_cpp(self) -> None:
        for branch in self._branches:
            self.cpp_class.addBranch(branch)
        for branch in self._exclude_branches:
            self.cpp_class.addExcludedBranch(branch)
        for tree in self._copy_trees:
            self.cpp_class.addCopyTree(tree)

    def _check_unused_options(self) -> None:
        unused = self._options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in ntuple block is not supported!".format(unused))
            exit(1)


    def adjust_regions(self, regions : dict) -> None:
        """!Initialize selection based on regions specified in the ntuple block - combined all of them with OR
        @param regions: dictionary with all regions (keys are region names, values are BlockReaderRegion objects)
        """
        # combine selections from all regions
        if self._regions:
            selections = []
            for region_name in self._regions:
                if region_name not in regions:
                    Logger.log_message("ERROR", "Unknown region {} specified in ntuple block".format(region_name))
                    exit(1)
                region_selection = regions[region_name].cpp_class.selection()
                if region_selection != "":
                    selections.append("(" + regions[region_name].cpp_class.selection() + ")")
            self._selection = "({})".format(" || ".join(selections))
        self.cpp_class.setSelection(self._selection)

    def adjust_samples(self, samples : dict) -> None:
        """!Adjust list of samples for which ntuple step should be run. If samples are specified, check if they exist. If no samples are specified, take all samples.
        @param samples: dictionary with all samples (keys are sample names)
        """
        if self._exclude_samples is not None:
            for sample_name in self._exclude_samples:
                if sample_name not in samples:
                    Logger.log_message("ERROR", "Unknown sample {} specified in ntuple block".format(sample_name))
                    exit(1)

        if self._samples is None:
            for sample_name in samples:
                if self._exclude_samples is None or sample_name not in self._exclude_samples:
                    self.cpp_class.addSample(samples[sample_name].cpp_class.getPtr())
        else:
            for sample_name in self._samples:
                if sample_name not in samples:
                    Logger.log_message("ERROR", "Unknown sample {} specified in ntuple block".format(sample_name))
                    exit(1)
                self.cpp_class.addSample(samples[sample_name].cpp_class.getPtr())

    def get_copy_trees(self) -> list:
        """!Get list of trees that should be copied
        """
        vector_trees = self.cpp_class.copyTrees()
        return [tree for tree in vector_trees]