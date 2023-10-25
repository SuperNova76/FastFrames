#include "FastFrames/MainFrame.h"

#include <iostream>

MainFrame::MainFrame() :
m_metadataManager{}
{
}

void MainFrame::init() {
    m_metadataManager.readFileList("test/input/filelist.txt");
    m_metadataManager.readSumWeights("test/input/sum_of_weights.txt");
}

void MainFrame::execute() {
    UniqueSampleID id(410470, "mc20e", "mc");
    std::cout << "xSec: " << m_metadataManager.crossSection(id) << ", luminosity: " << m_metadataManager.luminosity(id.campaign()) << "\n";
    std::cout << "nominal sumWeights: " << m_metadataManager.sumWeights(id, "NOSYS") << ", normalisation: " << m_metadataManager.normalisation(id, "NOSYS") << "\n";
    std::cout << "GEN_0p5muF_0p5muR_CT14 sumWeights: " << m_metadataManager.sumWeights(id, "GEN_0p5muF_0p5muR_CT14") << ", normalisation: " << m_metadataManager.normalisation(id, "GEN_0p5muF_0p5muR_CT14") << "\n";
} 
