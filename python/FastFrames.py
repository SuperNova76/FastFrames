"""!The main script for running the whole framework from python. It takes couple of arguments:

    --config: path to the config file

    --step: step to run, 'h' for histograms, 'n' for ntuples. Default: 'h'

    --samples: comma separated list of samples to run. Default: all
"""
import os
import sys
import argparse

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigSettingWrapper, FastFramesExecutor
from python_wrapper.python.logger import Logger
from ConfigReader import ConfigReader
from CommandLineOptions import CommandLineOptions

def prepare_output_folder(folder_path : str) -> None:
    """!Create output folder if it does not exist
    @param folder_path: path to the output folder
    """
    if folder_path == "" or folder_path == ".":
        return

    if not os.path.exists(folder_path):
        os.makedirs(folder_path)


if __name__ == "__main__":
    config_path = CommandLineOptions().get_config_path()
    step        = CommandLineOptions().get_step()

    Logger.log_message("INFO", "Going to read the config: " + config_path)
    config_reader = ConfigReader(config_path)

    fast_frames_executor = FastFramesExecutor(config_reader.block_general.cpp_class.getPtr())

    if step == "n":
        prepare_output_folder(config_reader.block_general.cpp_class.outputPathNtuples())
        fast_frames_executor.setRunNtuples(True)
    elif step == "h":
        prepare_output_folder(config_reader.block_general.cpp_class.outputPathHistograms())

    fast_frames_executor.runFastFrames()
