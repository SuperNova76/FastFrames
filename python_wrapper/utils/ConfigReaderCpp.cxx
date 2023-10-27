
#include <boost/python.hpp>
#include <string>

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Region.h"
#include "FastFrames/Variable.h"
#include "FastFrames/Binning.h"
#include "FastFrames/StringOperations.h"

using namespace std;

template <typename T>
unsigned long long int getPtr(T &ptr) {
    return reinterpret_cast<unsigned long long int>(&ptr);
}


// general block:

std::string _outputPath_general(const ConfigSetting &self) {
    return self.outputPath();
}

std::string _inputPath_general(const ConfigSetting &self) {
    return self.inputPath();
}

std::string _inputSumWeightsPath_general(const ConfigSetting &self) {
    return self.inputSumWeightsPath();
}

std::string _inputFilelistPath_general(const ConfigSetting &self) {
    return self.inputFilelistPath();
}



// region block:
std::string _name_region(const Region &self) {
    return self.name();
}

std::string _selection_region(const Region &self) {
    return self.selection();
}

std::vector<Variable> _variables_region(const Region &self) {
    return self.variables();
}

void _add_variable_region(Region &self, unsigned long long int variable_ptr) {
    const Variable *variable = reinterpret_cast<Variable*>(variable_ptr);
    self.addVariable(*variable);
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

    class_<ConfigSetting>("ConfigReaderCppGeneral",
        init<>())
        // getPtr
        .def("getPtr",          &getPtr<ConfigSetting>)

        // inputPath
        .def("inputPath",       _inputPath_general)
        .def("setInputPath",    &ConfigSetting::setInputPath)

        // outputPath
        .def("outputPath",      _outputPath_general)
        .def("setOutputPath",   &ConfigSetting::setOutputPath)

        // inputSumWeightsPath
        .def("inputSumWeightsPath",    _inputSumWeightsPath_general)
        .def("setInputSumWeightsPath", &ConfigSetting::setInputSumWeightsPath)

        // inputFilelistPath
        .def("inputFilelistPath",   _inputFilelistPath_general)
        .def("setInputFilelistPath",&ConfigSetting::setInputFilelistPath)

        // numCPU
        .def("numCPU",      &ConfigSetting::numCPU)
        .def("setNumCPU",   &ConfigSetting::setNumCPU)


        // addLuminosityInformation
        .def("setLuminosity", &ConfigSetting::addLuminosityInformation)
        .def("getLuminosity", &ConfigSetting::getLuminosity)
    ;

    class_<Region>("ConfigReaderCppRegion",
        init<std::string>())
        // getPtr
        .def("getPtr",          &getPtr<Region>)

        // name
        .def("name",        _name_region)

        // selection
        .def("selection",   _selection_region)
        .def("setSelection",&Region::setSelection)

        // addVariable
        .def("addVariable", _add_variable_region)
        .def("variables",   _variables_region)
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
