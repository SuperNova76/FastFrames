
#include <boost/python.hpp>
#include <string>

#include "python_wrapper/headers/ConfigSettingWrapper.h"
#include "python_wrapper/headers/RegionWrapper.h"
#include "python_wrapper/headers/VariableWrapper.h"
#include "python_wrapper/headers/MainFrameWrapper.h"
#include "python_wrapper/headers/FastFramesExecutorWrapper.h"
#include "python_wrapper/headers/UniqueSampleIDWrapper.h"
#include "python_wrapper/headers/SampleWrapper.h"
#include "python_wrapper/headers/SystematicWrapper.h"

#include "FastFrames/Binning.h"

using namespace std;


BOOST_PYTHON_MODULE(ConfigReaderCpp) {
    // An established convention for using boost.python.
    using namespace boost::python;

    class_<FastFramesExecutorWrapper>("FastFramesExecutor",
        init<long long unsigned int>())
        // runFastFrames
        .def("runFastFrames",   &FastFramesExecutorWrapper::runFastFrames)
    ;


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

        // Luminosity
        .def("setLuminosity", &ConfigSettingWrapper::addLuminosityInformation)
        .def("getLuminosity", &ConfigSettingWrapper::getLuminosity)
        .def("campaignIsDefined", &ConfigSettingWrapper::campaignIsDefined)

        // addRegion
        .def("addRegion",   &ConfigSettingWrapper::addRegion)

        // addSample
        .def("addSample",   &ConfigSettingWrapper::addSample)

        // addSystematic
        .def("addSystematic",   &ConfigSettingWrapper::addSystematic)
    ;

    class_<UniqueSampleIDWrapper>("ConfigReaderCppUniqueSampleID",
        init<int, std::string, std::string>())
        // getPtr
        .def("getPtr",          &UniqueSampleIDWrapper::getPtr)

        // dsid
        .def("dsid",            &UniqueSampleIDWrapper::dsid)

        // campaign
        .def("campaign",        &UniqueSampleIDWrapper::campaign)

        // simulation
        .def("simulation",      &UniqueSampleIDWrapper::simulation)
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

    class_<SampleWrapper>("SampleWrapper",
        init<std::string>())

        // getPtr
        .def("getPtr",          &SampleWrapper::getPtr)

        // name
        .def("name",            &SampleWrapper::name)

        // recoTreeName
        .def("recoTreeName",    &SampleWrapper::recoTreeName)

        // addUniqueSampleID
        .def("addUniqueSampleID",   &SampleWrapper::addUniqueSampleID)

        // addSystematic
        .def("addSystematic",       &SampleWrapper::addSystematic)

        // addRegion
        .def("addRegion",           &SampleWrapper::addRegion)

        // setEventWeight
        .def("setEventWeight",      &SampleWrapper::setEventWeight)
        .def("weight",              &SampleWrapper::weight)

        // skipSystematicRegionCombination
        .def("skipSystematicRegionCombination", &SampleWrapper::skipSystematicRegionCombination)
    ;

    class_<SystematicWrapper>("SystematicWrapper",
        init<std::string>())

        // getPtr
        .def("getPtr",          &SystematicWrapper::getPtr)

        // name
        .def("name",            &SystematicWrapper::name)

        // setSumWeights
        .def("setSumWeights",   &SystematicWrapper::setSumWeights)
        .def("sumWeights",      &SystematicWrapper::sumWeights)

        // addRegion
        .def("addRegion",       &SystematicWrapper::addRegion)
    ;
}
