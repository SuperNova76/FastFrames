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

        print(block_general.config_reader_cpp.getLuminosity("mc20a"))

