
from sys import path
from inspect import currentframe, getframeinfo, stack

# module "lives" in bin folder
path.append("../../bin/lib")
path.append("../../build/lib")

import cppLogger


class Logger:
    def log_message(log_level : int, message : str) -> None:
        caller = getframeinfo(stack()[1][0])
        file_name = caller.filename.split("/")[-1]
        cppLogger.log_message(
            Logger._convert_level_str_to_int(log_level),
            message+'\n',
            file_name,
            caller.lineno
        )

    def set_log_level(log_level) -> None:
        if type(log_level) == int:
            cppLogger.set_log_level(log_level)
        if type(log_level) == str:
            cppLogger.set_log_level(
                Logger._convert_level_str_to_int(log_level)
            )

    def log_level() -> str:
        return Logger._convert_level_int_to_str(cppLogger.log_level())

    def current_level() -> str:
        return Logger._convert_level_int_to_str(cppLogger.current_level())


    def _convert_level_str_to_int(log_level : str) -> int:
        if (log_level.upper() == "ERROR"):
            return 0
        elif (log_level.upper() == "WARNING"):
            return 1
        elif (log_level.upper() == "INFO"):
            return 2
        elif (log_level.upper() == "DEBUG"):
            return 3
        elif (log_level.upper() == "VERBOSE"):
            return 4
        else:
            error_message = "Unknown log level: {}".format(log_level)
            Logger.log_message("ERROR", error_message)
            raise Exception(error_message)

    def _convert_level_int_to_str(log_level : int) -> str:
        if (log_level == 0):
            return "ERROR"
        elif (log_level == 1):
            return "WARNING"
        elif (log_level == 2):
            return "INFO"
        elif (log_level == 3):
            return "DEBUG"
        elif (log_level == 4):
            return "VERBOSE"
        else:
            error_message = "Unknown log level: {}".format(log_level)
            Logger.log_message("ERROR", error_message)
            raise Exception(error_message)



