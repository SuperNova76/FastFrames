"""
@file Main config reader source file.
"""
import yaml
from sys import argv
import argparse

from BlockReaderGeneral import BlockReaderGeneral, vector_to_list
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from BlockReaderNtuple import BlockReaderNtuple
from BlockOptionsGetter import BlockOptionsGetter
from BlockReaderSystematic import BlockReaderSystematic, read_systematics_variations
from CommandLineOptions import CommandLineOptions

from python_wrapper.python.logger import Logger
from ConfigReaderCpp import VariableWrapper

class ConfigReader:
    """!Main class for reading the config file and connecting all its blocks.

    Reads the config file performs basic checks, create lists of the corresponding objects based on the objects in the config file, set all the necessary properties of the objects and then
    it propagates all the information to the C++ classes through BlockReaderGeneral class.
    """
    def __init__(self, config_file_path : str):
        with open(config_file_path, "r") as f:
            data = yaml.load(f, Loader=yaml.FullLoader)
            self.block_getter = BlockOptionsGetter(data)

            self.block_general = BlockReaderGeneral(self.block_getter.get("general"))

            self.regions = {}
            for region_dict in self.block_getter.get("regions"):
                region = BlockReaderRegion(region_dict, self.block_general)
                region_name = region.cpp_class.name()
                if region_name in self.regions:
                    Logger.log_message("ERROR", "Duplicate region name: {}".format(region_name))
                    exit(1)
                self.regions[region_name] = region
                self.block_general.add_region(region)

            self.samples = {}
            CommandLineOptions().check_samples_existence(self.block_getter.get("samples"))
            CommandLineOptions().keep_only_selected_samples(self.block_getter.get("samples"))
            for sample_dict in self.block_getter.get("samples"):
                sample = BlockReaderSample(sample_dict, self.block_general)
                sample.adjust_regions(self.regions)
                sample_name = sample.cpp_class.name()
                if sample_name in self.samples:
                    Logger.log_message("ERROR", "Duplicate sample name: {}".format(sample_name))
                    exit(1)
                self.samples[sample_name] = sample

            self.systematics = {}
            # nominal
            nominal_dict = {"variation": {"up": "NOSYS"}}
            nominal = BlockReaderSystematic(nominal_dict, "up", self.block_general)
            nominal.adjust_regions(self.regions)
            self.systematics[nominal.cpp_class.name()] = nominal

            for systematic_dict in self.block_getter.get("systematics"):
                systematic_list = read_systematics_variations(systematic_dict, self.block_general)
                for systematic in systematic_list:
                    systematic.adjust_regions(self.regions)
                    systematic_name = systematic.cpp_class.name()
                    if systematic_name in self.systematics:
                        Logger.log_message("ERROR", "Duplicate systematic name: {}".format(systematic_name))
                        exit(1)
                    self.systematics[systematic_name] = systematic

            for systematic_name,systematic in self.systematics.items():
                systematic.check_samples_existence(self.samples)
                systematic.check_regions_existence(self.regions)
                self.block_general.cpp_class.addSystematic(systematic.cpp_class.getPtr())

            for sample_name,sample in self.samples.items():
                Logger.log_message("INFO", "Sample {} has {} regions".format(sample_name, len(sample.cpp_class.regionsNames())))
                sample.adjust_systematics(self.systematics)
                sample.resolve_variables()
                self.block_general.cpp_class.addSample(sample.cpp_class.getPtr())

            self.block_ntuple = BlockReaderNtuple([])
            self.has_ntuple_block = "ntuples" in self.block_getter
            if self.has_ntuple_block:
                self.block_ntuple = BlockReaderNtuple(self.block_getter.get("ntuples"))
                self.block_ntuple.adjust_regions(self.regions)
                self.block_ntuple.adjust_samples(self.samples)
            self.block_general.cpp_class.setNtuple(self.block_ntuple.cpp_class.getPtr())

            unused_blocks = self.block_getter.get_unused_options()
            if unused_blocks:
                Logger.log_message("ERROR", "Unused blocks: {}".format(unused_blocks))
                exit(1)

if __name__ == "__main__":
    config_path = CommandLineOptions().get_config_path()

    config_reader = ConfigReader(config_path)
    block_general = config_reader.block_general

    print("\nGeneral block:")
    print("\tinputSumWeightsPath: ", block_general.cpp_class.inputSumWeightsPath())
    print("\toutputPathHistograms: ", block_general.cpp_class.outputPathHistograms())
    print("\toutputPathNtuples: ", block_general.cpp_class.outputPathNtuples())
    print("\tinputFilelistPath: ", block_general.cpp_class.inputFilelistPath())
    print("\tnumCPU: ", block_general.cpp_class.numCPU())
    print("\tautomaticSystematics: ", block_general.cpp_class.automaticSystematics())
    print("\tnominalOnly: ", block_general.cpp_class.nominalOnly())
    print("\tcustomFrameName: ", block_general.cpp_class.customFrameName())
    print("\tmin_event: ", block_general.cpp_class.minEvent())
    print("\tmax_event: ", block_general.cpp_class.maxEvent())
    print("\t--split_n_jobs: ", block_general.cpp_class.totalJobSplits())
    print("\t--job_index: ", block_general.cpp_class.currentJobIndex())
    print("\txSectionFiles: ",  block_general.get_xsection_files())
    print("\tluminosity, mc20a: ", block_general.cpp_class.getLuminosity("mc20a"))
    print("\tluminosity, mc20d: ", block_general.cpp_class.getLuminosity("mc20d"))

    print("\tcreate_tlorentz_vectors_for:")
    tlorentz_vectors = block_general.cpp_class.tLorentzVectors()
    for tlorentz_vector in tlorentz_vectors:
        print("\t\t", tlorentz_vector)

    ntuple_cpp_object = block_general.get_ntuple_object()
    if ntuple_cpp_object:
        print("\nNtuple block:")
        print("\tselection: ", ntuple_cpp_object.selection())
        n_samples = ntuple_cpp_object.nSamples()
        samples = [ntuple_cpp_object.sampleName(i_sample) for i_sample in range(n_samples)]
        print("\tsamples: [", ",".join(samples), "]")

        n_branches = ntuple_cpp_object.nBranches()
        branches = [ntuple_cpp_object.branchName(i_branch) for i_branch in range(n_branches)]
        print("\tbranches: [", ",".join(branches), "]")

        n_excluded_branches = ntuple_cpp_object.nExcludedBranches()
        excluded_branches = [ntuple_cpp_object.excludedBranchName(i_branch) for i_branch in range(n_excluded_branches)]
        print("\texcluded_branches: [", ",".join(excluded_branches), "]")

        print("\tcopy_trees: ", config_reader.block_ntuple.get_copy_trees())


    print("\n\nRegions block:\n")
    regions = config_reader.block_general.get_regions_cpp_objects()
    for region in regions:
        print("\tname: ", region.name())
        print("\tselection: ", region.selection())
        print("\tvariables:")
        variable_cpp_objects = BlockReaderRegion.get_variable_cpp_objects(region.getVariableRawPtrs())
        for variable_cpp_object in variable_cpp_objects:
            print("\t\tname: ", variable_cpp_object.name())
            print("\t\ttitle: ", variable_cpp_object.title())
            print("\t\tdefinition: ", variable_cpp_object.definition())
            if variable_cpp_object.hasRegularBinning():
                print(  "\t\tbinning: ",
                        variable_cpp_object.axisNbins(), ", ",
                        variable_cpp_object.axisMin(), ", ",
                        variable_cpp_object.axisMax())
            else:
                print("\t\tbinning: ", variable_cpp_object.binEdgesString())
            print("\n")
        variable_combinations = BlockReaderRegion.get_2d_combinations(region.variableCombinations())
        if len(variable_combinations) > 0:
            print("\t2d combinations:")
            for variable_combination in variable_combinations:
                print("\t\t", variable_combination)
            print("\n")

    print("\n\nSamples block:\n")
    samples = config_reader.block_general.get_samples_objects()
    for sample in samples:
        print("\tname: ", sample.name())
        print("\tregions: ", vector_to_list(sample.regionsNames()))
        print("\tweight: ", sample.weight())
        print("\tsystematic: ", vector_to_list(sample.systematicsNames()))
        print("\tselection_suffix: \"" + sample.selectionSuffix() + "\"")
        print("\treco_to_truth_pairing_indices: ", vector_to_list(sample.recoToTruthPairingIndices()))
        print("\tvariables:")
        variable_names = vector_to_list(sample.variables())
        for variable_name in variable_names:
            print("\t\t", variable_name)
        print("\tUnique samples:")
        n_unique_samples = sample.nUniqueSampleIDs()
        for i_unique_id in range(n_unique_samples):
            print("\t\t", sample.uniqueSampleIDstring(i_unique_id))
        truth_objects = BlockReaderSample.get_truth_cpp_objects(sample.getTruthSharedPtrs())
        if len(truth_objects) > 0:
            print("\tTruth objects:")
            for cpp_truth_object in truth_objects:
                print("\t\tname: ", cpp_truth_object.name())
                print("\t\tproduce_unfolding: ", cpp_truth_object.produceUnfolding())
                print("\t\ttruth_tree_name: ", cpp_truth_object.truthTreeName())
                print("\t\tselection: ", cpp_truth_object.selection())
                print("\t\tevent_weight: ", cpp_truth_object.eventWeight())
                print("\t\tmatch_variables:")
                n_matched_variables = cpp_truth_object.nMatchedVariables()
                for i_match_variable in range(n_matched_variables):
                    print("\t\t\t", cpp_truth_object.matchedVariables(i_match_variable))
                variable_raw_ptrs = cpp_truth_object.getVariableRawPtrs()
                print("\t\tvariables:")
                for variable_ptr in variable_raw_ptrs:
                    variable = VariableWrapper("")
                    variable.constructFromRawPtr(variable_ptr)
                    print("\t\t\tname: ", variable.name())
                    print("\t\t\ttitle: ", variable.title())
                    print("\t\t\tdefinition: ", variable.definition())
                    if variable.hasRegularBinning():
                        print(  "\t\t\tbinning: ",
                                variable.axisNbins(), ", ",
                                variable.axisMin(), ", ",
                                variable.axisMax())
                    else:
                        print("\t\t\tbinning: ", variable.binEdgesString())
                custom_defines = vector_to_list(cpp_truth_object.customDefines())
                if len(custom_defines) > 0:
                    print("\t\tCustom defines:")
                    for custom_define in custom_defines:
                        print("\t\t\t", custom_define)
                    print("\n")
        custom_defines = vector_to_list(sample.customDefines())
        if len(custom_defines) > 0:
            print("\tCustom defines:")
            for custom_define in custom_defines:
                print("\t\t", custom_define)
            print("\n")

        print("\n")

    systematics = config_reader.block_general.get_systematics_objects()
    print("\n\nSystematics block:\n")
    for systematic in systematics:
        print("\tname: ", systematic.name())
        print("\tregions: ", vector_to_list(systematic.regionsNames()))
        print("\tweight_suffix: ", systematic.weightSuffix())
        print("\tsum_weights: ", systematic.sumWeights())
        print("\n")

