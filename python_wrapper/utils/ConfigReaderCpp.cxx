
#include <boost/python.hpp>
#include <string>

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Region.h"
#include "FastFrames/Variable.h"
#include "FastFrames/Binning.h"

using namespace std;

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
void _set_binning_regular(Binning &self, const double min, const double max, const int nbins) {
    self.setBinning(min, max, nbins);
}

void _set_binning_irregular(Binning &self, const std::vector<double> &edges) {
    self.setBinning(edges);
}

vector<double> _binEdges_binning(const Binning &self) {
    return self.binEdges();
}


BOOST_PYTHON_MODULE(ConfigReaderCpp) {
    // An established convention for using boost.python.
    using namespace boost::python;

    class_<ConfigSetting>("ConfigReaderCppGeneral",
        init<>())
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

        // name
        .def("name",        _name_region)

        // selection
        .def("selection",   _selection_region)
        .def("setSelection",&Region::setSelection)
    ;

    class_<Variable>("ConfigReaderCppVariable",
        init<std::string>())

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

        // min
        .def("min",         &Binning::min)

        // max
        .def("max",         &Binning::max)

        // nbins
        .def("nbins",       &Binning::nbins)

        // setBinning
        .def("setBinningRegular",   _set_binning_regular)
        .def("setBinningIrregular", _set_binning_irregular)

        // binEdges
        .def("binEdges",    &_binEdges_binning)

        // hasRegularBinning
        .def("hasRegularBinning", &Binning::hasRegularBinning)
    ;


}
