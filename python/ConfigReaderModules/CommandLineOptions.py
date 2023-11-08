"""
@file Source file with CommandLineOptions and SingletonMeta class
"""
from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger

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
        self.samples_terminal = None

    def set_samples(self, samples : list) -> None:
        """
        Set list of samples specified from command line.
        @param samples - List of samples specified from command line.
        """
        self.samples_terminal = samples

    def get_samples(self) -> list:
        """
        Get list of samples specified from command line.
        """
        return self.samples_terminal

    def check_samples_existence(self, samples_all) -> None:
        """
        Check if all samples specified from command line exist.
        """
        if self.samples_terminal is None:
            return
        if samples_all is None:
            return
        samples_names = self._get_list_of_sample_names(samples_all)
        for sample in self.samples_terminal:
            if sample not in samples_names:
                Logger.log_message("ERROR", "Sample {} specified from command line does not exist".format(sample))
                exit(1)

    def keep_only_selected_samples(self, samples) -> None:
        """
        Remove all samples that are not specified from command line from the input list of samples.
        """
        if self.samples_terminal is None:
            return
        if samples is None:
            return
        samples_names = self._get_list_of_sample_names(samples)
        keep_sample = [sample in self.samples_terminal for sample in samples_names]

        for i in range(len(samples), 0, -1):
            if not keep_sample[i - 1]:
                del samples[i - 1]


    def _get_list_of_sample_names(self, samples) -> list:
        if type(samples) == list:
            if len(samples) == 0:
                return []
            if type(samples[0]) == dict:
                return [sample["name"] for sample in samples]
            else:
                return samples
        Logger.log_message("ERROR", "Unknown type of samples: {}".format(type(samples)))
