#include "FastFrames/MainFrame.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include <iostream>

MainFrame::MainFrame(const std::shared_ptr<ConfigSetting>& config) :
m_metadataManager{},
m_config(config),
m_systReplacer()
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

void MainFrame::processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID) {
    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(uniqueSampleID);
    if (filePaths.empty()) return;

    // we could use any file from the list, use the first one
    m_systReplacer.readSystematicMapFromFile(filePaths.at(0), sample->recoTreeName(), m_config->systematics());

    std::vector<std::vector<ROOT::RDF::RNode> > filterStore;

    ROOT::RDataFrame df(sample->recoTreeName(), filePaths);
    
    // this is the method users will be able to override
    ROOT::RDF::RNode mainNode = this->defineVariables(df);

    for (const auto& isyst : sample->systematics()) {
        std::vector<ROOT::RDF::RNode> perSystFilter;
        for (const auto& ireg : sample->regions()) {
            // TODO skip wrong region/systematic combination
            auto filter = this->filterSystRegion(mainNode, /*sample,*/ isyst, ireg);
            perSystFilter.emplace_back(std::move(filter));
        }
        filterStore.emplace_back(std::move(perSystFilter));
    }
}

ROOT::RDF::RNode MainFrame::filterSystRegion(ROOT::RDF::RNode& node,
                                             /*const std::shared_ptr<Sample>& sample,*/
                                             const std::shared_ptr<Systematic>& systematic,
                                             const std::shared_ptr<Region>& region) {

    const std::string& nominalSelection = region->selection();
    const std::string systSelection = m_systReplacer.replaceString(nominalSelection, systematic);

    return node.Filter(systSelection);
}
