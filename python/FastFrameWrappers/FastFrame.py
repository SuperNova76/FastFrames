import os
import sys

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppGeneral, FastFramesExecutor
from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader


if __name__ == "__main__":

    if len(sys.argv) < 2:
        raise ValueError("Please specify the path to the config file as the first argument")

    config_file_path = sys.argv[1]
    config_reader = ConfigReader(config_file_path)
    Logger.set_log_level(config_reader.block_general.debug_level)
    Logger.log_message("INFO", "Config file path: " + config_file_path)


    fast_frames_executor = FastFramesExecutor(config_reader.block_general.config_reader_cpp_general.getPtr())
    fast_frames_executor.runFastFrames()