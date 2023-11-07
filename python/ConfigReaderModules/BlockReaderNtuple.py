from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import NtupleWrapper

from BlockReaderVariable import BlockReaderVariable
from BlockReaderGeneral import BlockReaderGeneral
from BlockOptionsGetter import BlockOptionsGetter
from python_wrapper.python.logger import Logger
from CommandLineOptions import CommandLineOptions


class BlockReaderNtuple:
    def __init__(self, input_dict : dict):
        self.options_getter = BlockOptionsGetter(input_dict)

        self.samples = self.options_getter.get("samples",None, [list])
        self.exclude_samples = self.options_getter.get("exclude_samples",None, [list])
        CommandLineOptions().keep_only_selected_samples(self.samples)
        CommandLineOptions().keep_only_selected_samples(self.exclude_samples)

        if not self.samples is None and not self.exclude_samples is None:
            Logger.log_message("ERROR", "Both samples and exclude_samples specified for ntuple block")
            exit(1)

        self.selection = self.options_getter.get("selection",None, [str])
        self.regions = self.options_getter.get("regions",None, [list])
        if self.regions is not None and self.selection is not None:
            Logger.log_message("ERROR", "Both regions and selection specified for ntuple block")
            exit(1)

        self.branches = self.options_getter.get("branches",[], [list])
        self.exclude_branches = self.options_getter.get("exclude_branches",[], [list])
        self.copy_trees = self.options_getter.get("copy_trees",[], [list])

        self._check_unused_options()

        self.__set_config_reader_cpp()

    def __set_config_reader_cpp(self):
        self.cpp_class = NtupleWrapper()
        for branch in self.branches:
            self.cpp_class.addBranch(branch)
        for branch in self.exclude_branches:
            self.cpp_class.addExcludedBranch(branch)
        for tree in self.copy_trees:
            self.cpp_class.addCopyTree(tree)

    def _check_unused_options(self):
        unused = self.options_getter.get_unused_options()
        if len(unused) > 0:
            Logger.log_message("ERROR", "Key {} used in ntuple block is not supported!".format(unused))
            exit(1)


    def adjust_regions(self, regions : dict):
        # combine selections from all regions
        if self.regions:
            selections = []
            for region_name in self.regions:
                if region_name not in regions:
                    Logger.log_message("ERROR", "Unknown region {} specified in ntuple block".format(region_name))
                    exit(1)
                region_selection = regions[region_name].cpp_class.selection()
                if region_selection != "":
                    selections.append("(" + regions[region_name].selection + ")")
            self.selection = "({})".format(" || ".join(selections))
        self.cpp_class.setSelection(self.selection)

    def adjust_samples(self, samples : dict):
        if self.samples is None:
            for sample_name in samples:
                if self.exclude_samples is None or sample_name not in self.exclude_samples:
                    self.cpp_class.addSample(samples[sample_name].cpp_class.getPtr())
        else:
            for sample_name in self.samples:
                if sample_name not in samples:
                    Logger.log_message("ERROR", "Unknown sample {} specified in ntuple block".format(sample_name))
                    exit(1)
                self.cpp_class.addSample(samples[sample_name].cpp_class.getPtr())



    def get_copy_trees(self) -> list:
        vector_trees = self.cpp_class.copyTrees()
        return [tree for tree in vector_trees]