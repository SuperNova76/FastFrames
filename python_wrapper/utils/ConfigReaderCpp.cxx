
#include <boost/python.hpp>
#include <string>

#include "python_wrapper/headers/ConfigSettingWrapper.h"
#include "python_wrapper/headers/RegionWrapper.h"
#include "python_wrapper/headers/VariableWrapper.h"
#include "python_wrapper/headers/MainFrameWrapper.h"
#include "python_wrapper/headers/MainFrameWrapper.h"

#include "FastFrames/Binning.h"

using namespace std;


BOOST_PYTHON_MODULE(ConfigReaderCpp) {
    // An established convention for using boost.python.
    using namespace boost::python;

    class_<MainFrameWrapper>("MainFrame",
        init<>())
        // setConfig
        .def("setConfig",           &MainFrameWrapper::setConfig)

        // init
        .def("init",                &MainFrameWrapper::init)

        // executeHistograms
        .def("executeHistograms",   &MainFrameWrapper::executeHistograms)
    ;

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

    class_<VariableWrapper>("ConfigReaderCppVariable",
        init<std::string>())
        // getPtr
        .def("getPtr",          &VariableWrapper::getPtr)

        // name
        .def("name",            &VariableWrapper::name)

        // definition
        .def("definition",      &VariableWrapper::definition)
        .def("setDefinition",   &VariableWrapper::setDefinition)

        // title
        .def("title",               &VariableWrapper::title)
        .def("setTitle",            &VariableWrapper::setTitle)

        // binning
        .def("setBinningRegular",   &VariableWrapper::setBinningRegular)
        .def("setBinningIrregular", &VariableWrapper::setBinningIrregular)
        .def("hasRegularBinning",   &VariableWrapper::hasRegularBinning)

        .def("binEdges",            &VariableWrapper::binEdges)
        .def("binEdgesString",      &VariableWrapper::binEdgesString)
        .def("axisMin",             &VariableWrapper::axisMin)
        .def("axisMax",             &VariableWrapper::axisMax)
        .def("axisNbins",           &VariableWrapper::axisNbins)
    ;

}
