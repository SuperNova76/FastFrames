#include "FastFrames/MainFrame.h"

#include "FastFrames/HistoContainer.h"
#include "FastFrames/Logger.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include <iostream>
#include <exception>

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

void MainFrame::executeHistograms() {
    LOG(INFO) << "Started the main histogram processing\n";
    std::size_t sampleN(1);
    for (const auto& isample : m_config->samples()) {
        LOG(INFO) << "Processing sample: " << sampleN << " out of " << m_config->samples().size() << " samples\n"; 
        std::vector<SystematicHisto> systHistos;
        std::size_t uniqueSampleN(1);
        for (const auto& iUniqueSampleID : isample->uniqueSampleIDs()) {
            LOG(INFO) << "Processing unique sample: " << iUniqueSampleID << ", " << uniqueSampleN << " out of " << isample->uniqueSampleIDs().size() << " unique samples\n";

            auto currentHistos = this->processUniqueSample(isample, iUniqueSampleID);
            // this happens when there are no files provided
            if (currentHistos.empty()) continue;

            // merge the histograms or take them if it is the first set
            if (systHistos.empty())  {
                systHistos = std::move(currentHistos);
            } else {
                if (currentHistos.size() != systHistos.size()) {
                    LOG(ERROR) << "Number of the systematic histograms do not match\n";
                    LOG(ERROR) << "Size of the current histograms: " << currentHistos.size() << ", final histograms: " << systHistos.size() << "\n";
                    throw std::runtime_error("");
                }

                LOG(INFO) << "Merging samples\n"; 
                for (std::size_t isyst = 0; isyst < systHistos.size(); ++isyst) {
                    systHistos.at(isyst).merge(currentHistos.at(isyst));
                }
            }

            ++uniqueSampleN;
        }
        this->writeHistosToFile(systHistos, isample);
        ++sampleN;
    }
} 

std::vector<SystematicHisto> MainFrame::processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID) {
    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(uniqueSampleID);
    if (filePaths.empty()) return std::vector<SystematicHisto>{};

    // we could use any file from the list, use the first one
    m_systReplacer.readSystematicMapFromFile(filePaths.at(0), sample->recoTreeName(), m_config->systematics());

    std::vector<std::vector<ROOT::RDF::RNode> > filterStore;

    ROOT::RDataFrame df(sample->recoTreeName(), filePaths);
    
    // this is the method users will be able to override
    ROOT::RDF::RNode mainNode = this->defineVariables(df);

    ROOT::RDF::RNode weightNode = mainNode;

    for (const auto& isyst : sample->systematics()) {
        weightNode = this->addWeightColumns(weightNode, sample, isyst, uniqueSampleID);
    }

    for (const auto& isyst : sample->systematics()) {
        std::vector<ROOT::RDF::RNode> perSystFilter;
        for (const auto& ireg : sample->regions()) {
            // TODO skip wrong region/systematic combination
            auto filter = weightNode.Filter(this->systematicFilter(/*sample,*/ isyst, ireg));
            perSystFilter.emplace_back(std::move(filter));
        }
        filterStore.emplace_back(std::move(perSystFilter));
    }

    // retrieve the histograms;
    std::vector<SystematicHisto> histoContainer;
    std::size_t systIndex(0);
    for (const auto& isyst : sample->systematics()) {
        SystematicHisto systematicHisto(isyst->name());

        std::size_t regIndex(0);
        for (const auto& ireg : sample->regions()) {
            RegionHisto regionHisto(ireg->name());

            for (const auto& ivariable : ireg->variables()) {
                VariableHisto variableHisto(ivariable.name());

                ROOT::RDF::RResultPtr<TH1D> histogram;
                if (ivariable.hasRegularBinning()) {
                    histogram = filterStore.at(systIndex).at(regIndex).
                                    Histo1D({"", ivariable.title().c_str(), ivariable.axisNbins(), ivariable.axisMin(), ivariable.axisMax()},
                                    this->systematicVariable(ivariable, isyst), this->systematicWeight(isyst));
                } else {
                    const std::vector<double> binEdges = ivariable.binEdges();
                    histogram = filterStore.at(systIndex).at(regIndex).
                                    Histo1D({"", ivariable.title().c_str(), (int)binEdges.size(), binEdges.data()},
                                    this->systematicVariable(ivariable, isyst), this->systematicWeight(isyst));
                }

                if (!histogram) {
                    LOG(ERROR) << "Histogram for sample: " << sample->name() << ", systematic: "
                               << isyst->name() << ", region: " << ireg->name() << " and variable: " << ivariable.name() << " is empty!\n";
                    throw std::runtime_error("");

                }
                variableHisto.setHisto(histogram);

                regionHisto.addVariableHisto(std::move(variableHisto));
            }

            systematicHisto.addRegionHisto(std::move(regionHisto));
            ++regIndex;
        }
        histoContainer.emplace_back(std::move(systematicHisto));
        ++systIndex;
    }

    return histoContainer;
}

std::string MainFrame::systematicFilter(/*const std::shared_ptr<Sample>& sample,*/
                                        const std::shared_ptr<Systematic>& systematic,
                                        const std::shared_ptr<Region>& region) const {

    const std::string& nominalSelection = region->selection();
    const std::string systSelection = m_systReplacer.replaceString(nominalSelection, systematic);

    return systSelection;
}

std::string MainFrame::systematicVariable(const Variable& variable,
                                          const std::shared_ptr<Systematic>& systematic) const {

    const std::string& nominalVariable = variable.definition();
    const std::string systVariable = m_systReplacer.replaceString(nominalVariable, systematic);

    return systVariable;
}

std::string MainFrame::systematicWeight(const std::shared_ptr<Systematic>& systematic) const {
    
    return "weight_total_" + systematic->name();
}

ROOT::RDF::RNode MainFrame::addWeightColumns(ROOT::RDF::RNode mainNode,
                                             const std::shared_ptr<Sample>& sample,
                                             const std::shared_ptr<Systematic>& systematic,
                                             const UniqueSampleID& id) const {
    
    const std::string& nominalWeight = sample->weight();
    const float normalisation = m_metadataManager.normalisation(id, systematic->sumWeights());

    // to not cut very small numbers to zero
    std::ostringstream ss;
    ss << normalisation;
    
    const std::string& systName = "weight_total_" + systematic->name();
    const std::string formula = m_systReplacer.replaceString(nominalWeight, systematic) + "/" + ss.str();
    LOG(DEBUG) << "Unique sample: " << id << ", systematic: " << systematic->name() << ", weight formula: " << formula << ", new weight name: " << systName << "\n";
    
    auto node = mainNode.Define(systName, formula);
    return node;
}
  
void MainFrame::writeHistosToFile(const std::vector<SystematicHisto>& histos,
                                  const std::shared_ptr<Sample>& sample) const {

    std::string fileName = m_config->outputPath();
    fileName += fileName.empty() ? "" : "/";
    fileName += sample->name() + ".root";

    std::unique_ptr<TFile> out(TFile::Open(fileName.c_str(), "RECREATE"));
    if (!out) {
        LOG(ERROR) << "Cannot open ROOT file at: " << fileName << "\n";
        throw std::invalid_argument("");
    }
    
    LOG(INFO) << "Writing histograms to sample: " << fileName << "\n";

    for (const auto& isystHist : histos) {
        out->mkdir(isystHist.name().c_str());
        out->cd(isystHist.name().c_str());
        for (const auto& iregionHist : isystHist.regionHistos()) {
            for (const auto& ivariableHist : iregionHist.variableHistos()) {
                const std::string histoName = ivariableHist.name() + "_" + iregionHist.name();
                ivariableHist.histo()->Write(histoName.c_str());
            }
        }
    }

    out->Close();
}