
#include <boost/python.hpp>
#include <string>

#include "FastFrames/ConfigSetting.h"

using namespace std;

std::string _outputPath(const ConfigSetting &self) {
    return self.outputPath();
}

std::string _inputPath(const ConfigSetting &self) {
    return self.inputPath();
}

std::string _inputSumWeightsPath(const ConfigSetting &self) {
    return self.inputSumWeightsPath();
}

std::string _inputFilelistPath(const ConfigSetting &self) {
    return self.inputFilelistPath();
}

std::string _defaultRecoTreeName(const ConfigSetting &self) {
    return self.defaultRecoTreeName();
}

BOOST_PYTHON_MODULE(ConfigReaderCpp) {
    // An established convention for using boost.python.
    using namespace boost::python;

    class_<ConfigSetting>("ConfigReaderCpp",
        init<>())
        .add_property("outputPath", &ConfigSetting::outputPath, &ConfigSetting::setOutputPath)

        // inputPath
        .def("inputPath",       _inputPath)
        .def("setInputPath",    &ConfigSetting::setInputPath)

        // outputPath
        .def("outputPath",      _outputPath)
        .def("setOutputPath",   &ConfigSetting::setOutputPath)

        // inputSumWeightsPath
        .def("inputSumWeightsPath",    _inputSumWeightsPath)
        .def("setInputSumWeightsPath", &ConfigSetting::setInputSumWeightsPath)

        // inputFilelistPath
        .def("inputFilelistPath",   _inputFilelistPath)
        .def("setInputFilelistPath",&ConfigSetting::setInputFilelistPath)

        // defaultRecoTreeName
        .def("defaultRecoTreeName", _defaultRecoTreeName)
        .def("setDefaultRecoTreeName", &ConfigSetting::setDefaultRecoTreeName)

        // numCPU
        .def("numCPU",      &ConfigSetting::numCPU)
        .def("setNumCPU",   &ConfigSetting::setNumCPU)


        // addLuminosityInformation
        .def("setLuminosity", &ConfigSetting::addLuminosityInformation)
        .def("getLuminosity", &ConfigSetting::getLuminosity)
    ;
}
