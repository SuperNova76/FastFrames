
#include <boost/python.hpp>
#include <string>

#include "python_wrapper/headers/ConfigSettingWrapper.h"
#include "python_wrapper/headers/RegionWrapper.h"

#include "FastFrames/Variable.h"
#include "FastFrames/Binning.h"
#include "FastFrames/StringOperations.h"

using namespace std;

template <typename T>
unsigned long long int getPtr(T &ptr) {
    return reinterpret_cast<unsigned long long int>(&ptr);
}

//  variable block:
std::string _name_variable(const Variable &self) {
    return self.name();
}

std::string _definition_variable(const Variable &self) {
    return self.definition();
}

std::string _title_variable(const Variable &self) {
    return self.title();
}

//binning block:
void _setBinningRegular(Binning &self, const double min, const double max, const int nbins) {
    self.setBinning(min, max, nbins);
}

void _setBinningIrregular(Binning &self, const std::string &binning_string) {
    const std::vector<double> binEdges = StringOperations::convertStringTo<std::vector<double>>(binning_string);
    return self.setBinning(binEdges);
}

std::string _getBinEdges(const Binning &self) {
    const vector<double> &edges = self.binEdges();
    string result = "";
    for (const double &edge : edges) {
        result += to_string(edge) + ",";
    }
    return result.substr(0, result.size()-1);
}


BOOST_PYTHON_MODULE(ConfigReaderCpp) {
    // An established convention for using boost.python.
    using namespace boost::python;

    class_<ConfigSettingWrapper>("ConfigReaderCppGeneral",
        init<>())
        // getPtr
        .def("getPtr",          &ConfigSettingWrapper::getPtr)

        // inputPath
        .def("inputPath",       &ConfigSettingWrapper::inputPath)
        .def("setInputPath",    &ConfigSettingWrapper::setInputPath)

        // outputPath
        .def("outputPath",      &ConfigSettingWrapper::outputPath)
        .def("setOutputPath",   &ConfigSettingWrapper::setOutputPath)

        // inputSumWeightsPath
        .def("inputSumWeightsPath",    &ConfigSettingWrapper::inputSumWeightsPath)
        .def("setInputSumWeightsPath", &ConfigSettingWrapper::setInputSumWeightsPath)

        // inputFilelistPath
        .def("inputFilelistPath",   &ConfigSettingWrapper::inputFilelistPath)
        .def("setInputFilelistPath",&ConfigSettingWrapper::setInputFilelistPath)

        // customFrameName
        .def("customFrameName",     &ConfigSettingWrapper::customFrameName)
        .def("setCustomFrameName",  &ConfigSettingWrapper::setCustomFrameName)

        // numCPU
        .def("numCPU",      &ConfigSettingWrapper::numCPU)
        .def("setNumCPU",   &ConfigSettingWrapper::setNumCPU)

        // addLuminosityInformation
        .def("setLuminosity", &ConfigSettingWrapper::addLuminosityInformation)
        .def("getLuminosity", &ConfigSettingWrapper::getLuminosity)
    ;

    class_<RegionWrapper>("ConfigReaderCppRegion",
        init<std::string>())
        // getPtr
        .def("getPtr",          &RegionWrapper::getPtr)

        // name
        .def("name",            &RegionWrapper::name)

        // selection
        .def("selection",       &RegionWrapper::selection)
        .def("setSelection",    &RegionWrapper::setSelection)

        // addVariable
        .def("addVariable",     &RegionWrapper::addVariable)
        .def("variables",       &RegionWrapper::variables)
    ;

    class_<Variable>("ConfigReaderCppVariable",
        init<std::string>())
        // getPtr
        .def("getPtr",          &getPtr<Variable>)

        // name
        .def("name",        _name_variable)

        // definition
        .def("definition",  _definition_variable)
        .def("setDefinition",&Variable::setDefinition)

        // title
        .def("title",       _title_variable)
        .def("setTitle",    &Variable::setTitle)
    ;

    class_<Binning>("ConfigReaderCppBinning",
        init<>())
        // getPtr
        .def("getPtr",          &getPtr<Binning>)

        // min
        .def("min",         &Binning::min)

        // max
        .def("max",         &Binning::max)

        // nbins
        .def("nbins",       &Binning::nbins)

        // setBinning
        .def("setBinningRegular",   _setBinningRegular)
        .def("setBinningIrregular", _setBinningIrregular)

        // binEdges
        .def("binEdges",    _getBinEdges)

        // hasRegularBinning
        .def("hasRegularBinning", &Binning::hasRegularBinning)
    ;


}
