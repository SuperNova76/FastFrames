import yaml
from sys import argv
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderRegion import BlockReaderRegion
from BlockReaderSample import BlockReaderSample

from python_wrapper.python.logger import Logger

class ConfigReader:
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
            for sample_dict in data["samples"]:
                sample = BlockReaderSample(sample_dict, self.block_general)
                sample.adjust_regions(self.regions)
                sample_name = sample.name
                if sample_name in self.samples:
                    Logger.log_message("ERROR", "Duplicate sample name: {}".format(sample_name))
                    exit(1)
                self.samples[sample_name] = sample


if __name__ == "__main__":

    if len(argv) < 2:
        print("Please provide path to config file")
        exit(1)

    config_reader = ConfigReader(argv[1])
    block_general = config_reader.block_general

    print("\nGeneral block:")
    print("\tinputPath: ", block_general.config_reader_cpp_general.inputPath())
    print("\tinputSumWeightsPath: ", block_general.config_reader_cpp_general.inputSumWeightsPath())
    print("\toutputPath: ", block_general.config_reader_cpp_general.outputPath())
    print("\tinputFilelistPath: ", block_general.config_reader_cpp_general.inputFilelistPath())
    print("\tnumCPU: ", block_general.config_reader_cpp_general.numCPU())
    print("\tcustomFrameName: ", block_general.config_reader_cpp_general.customFrameName())
    print("\tluminosity, mc20a: ", block_general.config_reader_cpp_general.getLuminosity("mc20a"))
    print("\tluminosity, mc23c: ", block_general.config_reader_cpp_general.getLuminosity("mc23c"))

    regions = config_reader.regions

    print("\n\nRegions block:\n")
    for region_name,region in regions.items():
        print("\tname: ", region.config_reader_cpp_region.name())
        print("\tselection: ", region.config_reader_cpp_region.selection())
        print("\tvariables:")
        variables = region.variables
        for variable in variables:
            print("\t\tname: ", variable.config_reader_cpp_variable.name())
            print("\t\ttitle: ", variable.config_reader_cpp_variable.title())
            print("\t\tdefinition: ", variable.config_reader_cpp_variable.definition())
            #print("\t\tbinning: ", variable.config_reader_cpp_variable.binning())
            if variable.config_reader_cpp_variable.hasRegularBinning():
                print(  "\t\tbinning: ",
                        variable.config_reader_cpp_variable.axisNbins(), ", ",
                        variable.config_reader_cpp_variable.axisMin(), ", ",
                        variable.config_reader_cpp_variable.axisMax())
            else:
                print("\t\tbinning: ", variable.config_reader_cpp_variable.binEdgesString())
            print("\n")


