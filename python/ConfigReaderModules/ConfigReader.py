import yaml
from sys import argv
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderRegion import BlockReaderRegion

class ConfigReader:
    def __init__(self, config_file_path : str):
        with open(config_file_path, "r") as f:
            data = yaml.load(f, Loader=yaml.FullLoader)

            self.block_general = BlockReaderGeneral(data["general"])

            self.regions = []
            for region in data["regions"]:
                self.regions.append(BlockReaderRegion(region, self.block_general))


if __name__ == "__main__":

    if len(argv) < 2:
        print("Please provide path to config file")
        exit(1)

    config_reader = ConfigReader(argv[1])
    block_general = config_reader.block_general

    print("\nGeneral block:")
    print("inputPath: ", block_general.config_reader_cpp_general.inputPath())
    print("inputSumWeightsPath: ", block_general.config_reader_cpp_general.inputSumWeightsPath())
    print("outputPath: ", block_general.config_reader_cpp_general.outputPath())
    print("inputFilelistPath: ", block_general.config_reader_cpp_general.inputFilelistPath())
    print("numCPU: ", block_general.config_reader_cpp_general.numCPU())
    print("luminosity, mc20a: ", block_general.config_reader_cpp_general.getLuminosity("mc20a"))
    print("luminosity, mc23c: ", block_general.config_reader_cpp_general.getLuminosity("mc23c"))

    regions = config_reader.regions

    print("\n\nRegions block:\n")
    for region in regions:
        print("\tname: ", region.config_reader_cpp_region.name())
        print("\tselection: ", region.config_reader_cpp_region.selection())
        print("\tvariables: \n")
        variables = region.variables


