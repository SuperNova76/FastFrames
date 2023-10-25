#include "FastFrames/MainFrame.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include "ROOT/RDataFrame.hxx"

#include <iostream>

MainFrame::MainFrame(const std::shared_ptr<ConfigSetting>& config) :
m_metadataManager{},
m_config(config)
{
}

void MainFrame::init() {
    m_metadataManager.readFileList("test/input/filelist.txt");
    m_metadataManager.readSumWeights("test/input/sum_of_weights.txt");
}

void MainFrame::execute() {
    for (const auto& isample : m_config->samples()) {
        for (const auto& iUniqueSampleID : isample->uniqueSampleIDs()) {
            this->processUniqueSample(isample, iUniqueSampleID);
        }
    }
} 

void MainFrame::processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID) const {
    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(uniqueSampleID);

    ROOT::RDataFrame df(sample->recoTreeName(), filePaths);
}