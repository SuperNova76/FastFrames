"""
The main script for running the whole framework from python.
It takes the path to the config file as the first argument.
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


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--config",  help="Path to the config file")
    parser.add_argument("--step",    help="Step to run: 'n' (ntuples) or 'h' (histograms). Default: 'h'", nargs = '?',  default="h")
    parser.add_argument("--samples", help="Comma separated list of samples to run. Default: all",                       default="all")
    args = parser.parse_args()

    config_path = args.config
    step        = args.step
    if step != "h" and step != "n":
        Logger.log_message("ERROR", "Unknown step: {}".format(step))

    samples     = args.samples
    if samples == "all":
        samples = None
    else:
        samples = samples.split(",")

    command_line_options = CommandLineOptions()
    command_line_options.set_samples(samples)

    Logger.log_message("INFO", "Going to read the config: " + config_path)
    config_reader = ConfigReader(config_path)
    Logger.set_log_level(config_reader.block_general.debug_level)

    fast_frames_executor = FastFramesExecutor(config_reader.block_general.cpp_class.getPtr())

    if len(sys.argv) > 2:
        step = sys.argv[2].lower()
        if step == "ntuple" or step == "ntuples":
            fast_frames_executor.setRunNtuples(True)
    fast_frames_executor.runFastFrames()
