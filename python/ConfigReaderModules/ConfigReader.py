import yaml
from sys import argv
from BlockReaderGeneral import BlockReaderGeneral
from BlockReaderRegion import BlockReaderRegion



if __name__ == "__main__":
    with open(argv[1]) as f:
        data = yaml.load(f, Loader=yaml.FullLoader)
        #print(data)

        block_general = BlockReaderGeneral(data["general"])
        block_region = BlockReaderRegion(data["regions"][0])

        #print(block_general.config_reader_cpp.getLuminosity("mc20a"))
        print("Config reader testing\n\n")

        print("\t\tGeneral block:")
        print("inputPath: ", block_general.config_reader_cpp.inputPath())
        print("inputSumWeightsPath: ", block_general.config_reader_cpp.inputSumWeightsPath())
        print("outputPath: ", block_general.config_reader_cpp.outputPath())
        print("inputFilelistPath: ", block_general.config_reader_cpp.inputFilelistPath())
        print("defaultRecoTreeName: ", block_general.config_reader_cpp.defaultRecoTreeName())
        print("numCPU: ", block_general.config_reader_cpp.numCPU())
        print("luminosity, mc20a: ", block_general.config_reader_cpp.getLuminosity("mc20a"))
        print("luminosity, mc23c: ", block_general.config_reader_cpp.getLuminosity("mc23c"))

