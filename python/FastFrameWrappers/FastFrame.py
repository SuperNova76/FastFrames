import os
import sys

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
print(this_dir)
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppGeneral, MainFrame
from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader


if __name__ == "__main__":

    config_reader = None
    if len(sys.argv) > 1:
        config_file_path = sys.argv[1]
        config_reader = ConfigReader(config_file_path)
        Logger.set_log_level(config_reader.block_general.debug_level)
        Logger.log_message("Config file path: " + config_file_path)


    main_frame = MainFrame()
    if config_reader is not None:
        main_frame.setConfigReader(config_reader.getPtr())
    main_frame.init()
    main_frame.executeHistograms()