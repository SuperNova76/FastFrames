"""
@file Source file with BlockReaderSimpleONNXInference class.
"""
from typing import Optional
from BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import SimpleONNXInferenceWrapper
from BlockOptionsGetter import BlockOptionsGetter

def vector_to_list(cpp_vector) -> list:
    """!Convert C++ vector to python list
    @param cpp_vector: C++ vector
    @return python list
    """
    result = []
    for element in cpp_vector:
        result.append(element)
    return result

class BlockReaderSimpleONNXInference:
    """!!Class for reading simple_onnx_inference block of the config, equivalent of C++ class SimpleONNXInference
    """

    def __init__(self, input_dict : dict):
        """!Constructor of the BlockReaderSimpleONNXInference class. It reads all the options from the simple_onnx_inference block, sets the properties of the C++ ConfigSetting class and check for user's errors
        @param input_dict: dictionary with options from the config file
        """
        self._options_getter = BlockOptionsGetter(input_dict)
        self._name = self._options_getter.get("name", None, [str])

        self.cpp_class = SimpleONNXInferenceWrapper(self._name)
        self.cpp_class.setModelPaths(self._options_getter.get("model_paths", [], [list]))
        self.cpp_class.setFoldFormula(self._options_getter.get("fold_formula", "", [str]))
        inputs_dict = self._options_getter.get("inputs", {}, [dict])
        for key, value in inputs_dict.items():
            self.cpp_class.addInput(key, value)
        outputs_dict = self._options_getter.get("outputs", {}, [dict])
        for key, value in outputs_dict.items():
            self.cpp_class.addOutput(key, value)
