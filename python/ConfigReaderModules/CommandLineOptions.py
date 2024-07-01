"""
@file Source file with CommandLineOptions and SingletonMeta class
"""

import argparse

from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from copy import deepcopy

class SingletonMeta(type):
    """!Singleton metaclass.
    """
    _instances = {}

    def __call__(cls, *args, **kwargs):
        """
        Make sure that only one instance of the class exists.
        """
        if cls not in cls._instances:
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        return cls._instances[cls]


class CommandLineOptions(metaclass=SingletonMeta):
    """!Singleton class for storing command line options.
    """
    def __init__(self):
        self._samples_terminal = None
        self._min_event = None
        self._max_event = None
        self._config_path = None
        self._step = None
        self._split_n_jobs = None
        self._job_index = None
        self._regions = None
        self._input_path = None
        self._output_path_ntuples = None
        self._output_path_histograms = None
        self._set_options()

    def _set_options(self) -> None:
        parser = argparse.ArgumentParser()

        parser.add_argument("-c", "--config",  help="Path to the config file")
        parser.add_argument("--samples", help="Comma separated list of samples to run. Default: all", default="all")
        parser.add_argument("--step",    help="Step to run: 'n' (ntuples) or 'h' (histograms). Default: 'h'", nargs = '?',  default="h")
        parser.add_argument("--min_event", help="Minimal index of event", default="")
        parser.add_argument("--max_event", help="Maximal index of event", default="")
        parser.add_argument("--split_n_jobs", help="Number of jobs to split one sample into", default="")
        parser.add_argument("--job_index",    help="ID of the job", default="")
        parser.add_argument("--trex_settings", help="Path to the yaml with TRExFitter specific settings, it is used to create config", default="")
        parser.add_argument("-o", "--output", help="Output config file for TRExFitter", default = "trex_config.config")
        parser.add_argument("-u", "--unfolding", help="Unfolding configuration: 'sample:truth_block_name:truth_variable_name'", default = "")
        parser.add_argument("--regions", help="Comma separated list of regions include to TRExFitter config. Regular expressions are allowed. Default: .*", default=".*")
        parser.add_argument("-i", "--input_path", help="Path to the directory containing filelist.txt and sum_of_weights.txt. If not supplied, defaults to value in config file", default="")
        parser.add_argument("--output_path_ntuples", help="Path to the directory where ntuples are stored. If not supplied, defaults to value in config file", default="")
        parser.add_argument("--output_path_histograms", help="Path to the directory where histograms are stored. If not supplied, defaults to value in config file", default="")

        args = parser.parse_args()

        self._config_path = args.config
        if (self._config_path is None):
            Logger.log_message("ERROR", "config argument is not specified, please use --config <path_to_config_file>")
            exit(1)

        self._step        = args.step

        samples     = args.samples
        if samples == "all":
            self._samples_terminal = None
        else:
            self._samples_terminal = samples.split(",")

        if (args.min_event):
            self._min_event = int(args.min_event)
        if (args.max_event):
            self._max_event = int(args.max_event)

        self._trex_settings = args.trex_settings
        self._trex_config = args.output

        self._unfolding_settings = None
        if args.unfolding:
            if args.unfolding.count(":") != 3:
                Logger.log_message("ERROR", "unfolding argument must have format 'sample:truth_block_name:truth_variable_name:reco_variable_name'")
                exit(1)
            self._unfolding_settings = tuple(args.unfolding.split(":"))

        if args.input_path:
            self._input_path = args.input_path

        self._read_splits(args)
        self._read_regions(args)
        self._read_output_paths(args)

    def _read_splits(self, args) -> None:
        if (args.split_n_jobs):
            if (args.split_n_jobs.isnumeric() == False):
                Logger.log_message("ERROR", "split_n_jobs must be a number")
                exit(1)
            self._split_n_jobs = int(args.split_n_jobs)
        if (args.job_index):
            if (args.job_index.isnumeric() == False):
                Logger.log_message("ERROR", "job_index must be a number")
                exit(1)
            self._job_index = int(args.job_index)
        if ((self._split_n_jobs is None) != (self._job_index is None)):
            Logger.log_message("ERROR", "Only one of the following options has been specified. Please specify either both or none: split_n_jobs, job_index")
            exit(1)
        if (self._split_n_jobs is None):
            return
        if (self._job_index >= self._split_n_jobs):
            Logger.log_message("ERROR", "job_index must be smaller than split_n_jobs")
            exit(1)
        if (self._job_index < 0 | self._split_n_jobs < 0):
            Logger.log_message("ERROR", "job_index and split_n_jobs must be positive")
            exit(1)

    def _read_regions(self, args) -> None:
        self._regions = [reg.strip() for reg in args.regions.split(",")]

    def _read_output_paths(self, args) -> None:
        if args.output_path_ntuples:
            self._output_path_ntuples = args.output_path_ntuples
        if args.output_path_histograms:
            self._output_path_histograms = args.output_path_histograms

    def get_output_path_ntuples(self) -> str:
        """
        Get path to the directory where ntuples are stored.
        """
        return self._output_path_ntuples

    def get_output_path_histograms(self) -> str:
        """
        Get path to the directory where histograms are stored.
        """
        return self._output_path_histograms

    def get_regions(self) -> list:
        """
        Get list of regions specified from command line.
        """
        return self._regions

    def get_split_n_jobs(self) -> int:
        """
        Get number of jobs to split one sample into.
        """
        return self._split_n_jobs

    def get_job_index(self) -> int:
        """
        Get ID of the job.
        """
        return self._job_index

    def get_step(self) -> str:
        """
        Get step to run.
        """
        return self._step

    def get_config_path(self) -> str:
        """
        Get path to the config file.
        """
        return self._config_path

    def get_min_event(self) -> int:
        """
        Get minimal index of event.
        """
        return self._min_event

    def get_max_event(self) -> int:
        """
        Get maximal index of event.
        """
        return self._max_event

    def get_samples(self) -> list:
        """
        Get list of samples specified from command line.
        """
        return self._samples_terminal

    def check_samples_existence(self, samples_all) -> None:
        """
        Check if all samples specified from command line exist.
        """
        if self._samples_terminal is None:
            return
        if samples_all is None:
            return
        samples_names = self._get_list_of_sample_names(samples_all)
        for sample in self._samples_terminal:
            if sample not in samples_names:
                Logger.log_message("ERROR", "Sample {} specified from command line does not exist".format(sample))
                exit(1)

    def keep_only_selected_samples(self, samples) -> None:
        """
        Remove all samples that are not specified from command line from the input list of samples.
        """
        if self._samples_terminal is None:
            return
        if samples is None:
            return
        samples_names = self._get_list_of_sample_names(samples)
        keep_sample = [sample in self._samples_terminal for sample in samples_names]

        for i in range(len(samples), 0, -1):
            if not keep_sample[i - 1]:
                del samples[i - 1]

    def set_default_samples(self, samples : list[str]) -> None:
        """
        Set default samples if none are specified from command line.
        """
        if self._samples_terminal is None:
            self._samples_terminal = deepcopy(samples)

    def get_trex_settings_yaml(self) -> str:
        """
        Get path to the yaml with TRExFitter specific settings.
        """
        return self._trex_settings

    def get_trex_fitter_output_config(self) -> str:
        """
        Get path to the output config file for TRExFitter.
        """
        return self._trex_config

    def get_unfolding_settings(self) -> tuple:
        """
        Get unfolding settings.
        """
        return self._unfolding_settings

    def _get_list_of_sample_names(self, samples) -> list:
        if type(samples) == list:
            if len(samples) == 0:
                return []
            if type(samples[0]) == dict:
                return [sample["name"] for sample in samples]
            else:
                return samples
        Logger.log_message("ERROR", "Unknown type of samples: {}".format(type(samples)))

    def get_input_path(self) -> str:
        """
        Get path to the directory containing filelist.txt and sum_of_weights.txt.
        """
        return self._input_path
