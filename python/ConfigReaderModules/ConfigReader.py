"""
@file Main config reader source file.
"""
import yaml
from sys import argv
import argparse

from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample
from BlockReaderNtuple import BlockReaderNtuple
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

            self.block_general = BlockReaderGeneral(data["general"])

            self.regions = {}
            for region_dict in data["regions"]:
                region = BlockReaderRegion(region_dict, self.block_general)
                region_name = region.name
                if region_name in self.regions:
                    Logger.log_message("ERROR", "Duplicate region name: {}".format(region_name))
                    exit(1)
                self.regions[region_name] = region
                self.block_general.add_region(region)

            self.samples = {}
            CommandLineOptions().check_samples_existence(data["samples"])
            CommandLineOptions().keep_only_selected_samples(data["samples"])
            for sample_dict in data["samples"]:
                sample = BlockReaderSample(sample_dict, self.block_general)
                sample.adjust_regions(self.regions)
                sample_name = sample.name
                if sample_name in self.samples:
                    Logger.log_message("ERROR", "Duplicate sample name: {}".format(sample_name))
                    exit(1)
                self.samples[sample_name] = sample

            self.systematics = {}
            # nominal
            nominal_dict = {"variation": {"up": "NOSYS"}}
            nominal = BlockReaderSystematic(nominal_dict, "up", self.block_general)
            nominal.adjust_regions(self.regions)
            self.systematics[nominal.name] = nominal

            for systematic_dict in data["systematics"]:
                systematic_list = read_systematics_variations(systematic_dict, self.block_general)
                for systematic in systematic_list:
                    systematic.adjust_regions(self.regions)
                    systematic_name = systematic.name
                    if systematic_name in self.systematics:
                        Logger.log_message("ERROR", "Duplicate systematic name: {}".format(systematic_name))
                        exit(1)
                    self.systematics[systematic_name] = systematic

            for systematic_name,systematic in self.systematics.items():
                systematic.check_samples_existence(self.samples)
                self.block_general.cpp_class.addSystematic(systematic.cpp_class.getPtr())

            for sample_name,sample in self.samples.items():
                Logger.log_message("INFO", "Sample {} has {} regions".format(sample_name, len(sample.regions)))
                sample.adjust_systematics(self.systematics)
                self.block_general.cpp_class.addSample(sample.cpp_class.getPtr())

            self.block_ntuple = BlockReaderNtuple([])
            self.has_ntuple_block = "ntuples" in data
            if self.has_ntuple_block:
                self.block_ntuple = BlockReaderNtuple(data["ntuples"])
                self.block_ntuple.adjust_regions(self.regions)
                self.block_ntuple.adjust_samples(self.samples)
            self.block_general.cpp_class.setNtuple(self.block_ntuple.cpp_class.getPtr())


if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    parser.add_argument("--config",  help="Path to the config file")
    parser.add_argument("--samples", help="Comma separated list of samples to run. Default: all", default="all")
    args = parser.parse_args()

    config_path = args.config

    samples     = args.samples
    if samples == "all":
        samples = None
    else:
        samples = samples.split(",")
    command_line_options = CommandLineOptions()
    command_line_options.set_samples(samples)

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
    print("\txSectionFiles: ",  block_general.get_xsection_files())
    print("\tluminosity, mc20a: ", block_general.cpp_class.getLuminosity("mc20a"))
    print("\tluminosity, mc20d: ", block_general.cpp_class.getLuminosity("mc20d"))

    print("\tcreate_tlorentz_vectors_for:")
    tlorentz_vectors = block_general.cpp_class.tLorentzVectors()
    for tlorentz_vector in tlorentz_vectors:
        print("\t\t", tlorentz_vector)

    if config_reader.has_ntuple_block:
        print("\nNtuple block:")
        print("\tselection: ", config_reader.block_ntuple.cpp_class.selection())
        n_samples = config_reader.block_ntuple.cpp_class.nSamples()
        samples = [config_reader.block_ntuple.cpp_class.sampleName(i_sample) for i_sample in range(n_samples)]
        print("\tsamples: [", ",".join(samples), "]")

        n_branches = config_reader.block_ntuple.cpp_class.nBranches()
        branches = [config_reader.block_ntuple.cpp_class.branchName(i_branch) for i_branch in range(n_branches)]
        print("\tbranches: [", ",".join(branches), "]")

        n_excluded_branches = config_reader.block_ntuple.cpp_class.nExcludedBranches()
        excluded_branches = [config_reader.block_ntuple.cpp_class.excludedBranchName(i_branch) for i_branch in range(n_excluded_branches)]
        print("\texcluded_branches: [", ",".join(excluded_branches), "]")

        print("\tcopy_trees: ", config_reader.block_ntuple.get_copy_trees())


    regions = config_reader.regions

    print("\n\nRegions block:\n")
    for region_name,region in regions.items():
        print("\tname: ", region.cpp_class.name())
        print("\tselection: ", region.cpp_class.selection())
        print("\tvariables:")
        variable_cpp_objects = region.get_variable_cpp_objects()
        for variable_cpp_object in variable_cpp_objects:
            print("\t\tname: ", variable_cpp_object.name())
            print("\t\ttitle: ", variable_cpp_object.title())
            print("\t\tdefinition: ", variable_cpp_object.definition())
            #print("\t\tbinning: ", variable_cpp_object.binning())
            if variable_cpp_object.hasRegularBinning():
                print(  "\t\tbinning: ",
                        variable_cpp_object.axisNbins(), ", ",
                        variable_cpp_object.axisMin(), ", ",
                        variable_cpp_object.axisMax())
            else:
                print("\t\tbinning: ", variable_cpp_object.binEdgesString())
            print("\n")
        variable_combinations = region.get_2d_combinations()
        if len(variable_combinations) > 0:
            print("\t2d combinations:")
            for variable_combination in variable_combinations:
                print("\t\t", variable_combination)
            print("\n")

    samples = config_reader.samples
    print("\n\nSamples block:\n")
    for sample_name,sample in samples.items():
        print("\tname: ", sample.cpp_class.name())
        print("\tregions: ", sample.regions)
        print("\tweight: ", sample.cpp_class.weight())
        print("\tsystematic: ", sample.systematic)
        print("\tselection_suffix: \"" + sample.selection_suffix + "\"")
        print("\treco_to_truth_pairing_indices: ", sample.get_reco_to_truth_pairing_indices())
        print("\tUnique samples:")
        n_unique_samples = sample.cpp_class.nUniqueSampleIDs()
        for i_unique_id in range(n_unique_samples):
            print("\t\t", sample.cpp_class.uniqueSampleIDstring(i_unique_id))
        truth_objects = sample.get_truth_cpp_objects()
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
                custom_defines = cpp_truth_object.customDefines()
                if len(custom_defines) > 0:
                    print("\t\tCustom defines:")
                    for custom_define in custom_defines:
                        print("\t\t\t", custom_define)
                    print("\n")
        custom_defines = sample.get_custom_defines()
        if len(custom_defines) > 0:
            print("\tCustom defines:")
            for custom_define in custom_defines:
                print("\t\t", custom_define)
            print("\n")

        print("\n")

    systematics = config_reader.systematics
    print("\n\nSystematics block:\n")
    for systematic_name,systematic in systematics.items():
        print("\tname: ", systematic.cpp_class.name())
        print("\tregions: ", systematic.regions)
        print("\tweight_suffix: ", systematic.cpp_class.weightSuffix())
        print("\tsum_weights: ", systematic.cpp_class.sumWeights())
        print("\n")

