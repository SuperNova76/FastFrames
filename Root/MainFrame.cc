/**
 * @file MainFrame.cc
 * @brief Main class responsible for histogramming and ntupling
 *
 */


#include "FastFrames/MainFrame.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include "TSystem.h"

#include "Math/Vector4D.h"
#include "ROOT/RDFHelpers.hxx"

#include <iostream>
#include <exception>

void MainFrame::init() {
    ROOT::EnableImplicitMT(m_config->numCPU());
    m_metadataManager.readFileList( m_config->inputFilelistPath() );
    m_metadataManager.readSumWeights( m_config->inputSumWeightsPath() );
    m_metadataManager.readXSectionFiles( m_config->xSectionFiles() );

    // propagate luminosity information fomr config
    for (const auto& ilumi : m_config->luminosityMap()) {
        m_metadataManager.addLuminosity(ilumi.first, ilumi.second);
    }

    if (m_config->nominalOnly() && m_config->automaticSystematics()) {
        LOG(ERROR) << "Nominal only and automatically read systematics from a file are both set to true, please fix!\n";
        throw std::invalid_argument("");
    }

    // check systematics need to be cleared
    if (m_config->nominalOnly() || m_config->automaticSystematics()) {
        m_config->clearSystematics();
    }
}

void MainFrame::executeHistograms() {

    if (m_config->nominalOnly() || m_config->automaticSystematics()) {
        for (auto& isample : m_config->samples()) {
            this->readAutomaticSystematics(isample, m_config->nominalOnly());
        }
    }

    LOG(INFO) << "Started the main histogram processing\n";
    std::size_t sampleN(1);
    for (const auto& isample : m_config->samples()) {
        LOG(INFO) << "Processing sample: " << sampleN << " out of " << m_config->samples().size() << " samples\n";
        std::vector<SystematicHisto> systHistos;
        std::size_t uniqueSampleN(1);
        ROOT::RDF::RNode* node(nullptr);
        for (const auto& iUniqueSampleID : isample->uniqueSampleIDs()) {
            LOG(INFO) << "Processing unique sample: " << iUniqueSampleID << ", " << uniqueSampleN << " out of " << isample->uniqueSampleIDs().size() << " unique samples\n";

            auto currentHistos = this->processUniqueSample(isample, iUniqueSampleID);
            // this happens when there are no files provided
            if (currentHistos.first.empty()) continue;
            node = &currentHistos.second;

            // merge the histograms or take them if it is the first set
            if (systHistos.empty())  {
                LOG(DEBUG) << "First set of histograms for this sample, this will NOT trigger event loop\n";
                systHistos = std::move(currentHistos.first);
            } else {
                if (currentHistos.first.size() != systHistos.size()) {
                    LOG(ERROR) << "Number of the systematic histograms do not match\n";
                    LOG(ERROR) << "Size of the current histograms: " << currentHistos.first.size() << ", final histograms: " << systHistos.size() << "\n";
                    throw std::runtime_error("");
                }

                LOG(INFO) << "Merging samples, triggers event loop!\n";
                for (std::size_t isyst = 0; isyst < systHistos.size(); ++isyst) {
                    systHistos.at(isyst).merge(currentHistos.first.at(isyst));
                }
                LOG(INFO) << "Number of event loops: " << node->GetNRuns() << ". For an optimal run, this number should be 1\n";
            }
            ++uniqueSampleN;
        }

        bool printEventLoop = isample->uniqueSampleIDs().size() == 1;
        this->writeHistosToFile(systHistos, isample, node, printEventLoop && node);
        ++sampleN;
    }
}

std::pair<std::vector<SystematicHisto>, ROOT::RDF::RNode> MainFrame::processUniqueSample(const std::shared_ptr<Sample>& sample,
                                                                                         const UniqueSampleID& uniqueSampleID) {
    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(uniqueSampleID);

    ROOT::RDataFrame df(sample->recoTreeName(), filePaths);
    if (filePaths.empty()) return std::make_pair(std::vector<SystematicHisto>{}, df);

    // we could use any file from the list, use the first one
    m_systReplacer.readSystematicMapFromFile(filePaths.at(0), sample->recoTreeName(), m_config->systematics());

    ROOT::RDF::RNode mainNode = df;
    //ROOT::RDF::Experimental::AddProgressBar(mainNode);

    mainNode = this->addWeightColumns(mainNode, sample, uniqueSampleID);

    // add TLorentzVectors for objects
    mainNode = this->addTLorentzVectors(mainNode);

    // this is the method users will be able to override
    mainNode = this->defineVariables(mainNode, uniqueSampleID);

    std::vector<std::vector<ROOT::RDF::RNode> > filterStore = this->applyFilters(mainNode, sample);

    // retrieve the histograms;
    std::vector<SystematicHisto> histoContainer = this->processHistograms(filterStore, sample);

    return std::make_pair(std::move(histoContainer), mainNode);
}

std::string MainFrame::systematicFilter(const std::shared_ptr<Sample>& sample,
                                        const std::shared_ptr<Systematic>& systematic,
                                        const std::shared_ptr<Region>& region) const {

    std::string nominalSelection = region->selection();
    if (!sample->selectionSuffix().empty()) {
        nominalSelection = "(" + nominalSelection + ") && (" + sample->selectionSuffix() + ")";
    }
    const std::string systSelection = m_systReplacer.replaceString(nominalSelection, systematic);
    LOG(VERBOSE) << "Sample: " << sample->name() << ", region: " << region->name() << ", systematic: "
                 << systematic->name() << ", original selection: " << nominalSelection << ", systematic selection: " << systSelection << "\n";

    return systSelection;
}

std::string MainFrame::systematicVariable(const Variable& variable,
                                          const std::shared_ptr<Systematic>& systematic) const {

    const std::string& nominalVariable = variable.definition();
    const std::string systVariable = m_systReplacer.replaceString(nominalVariable, systematic);

    return systVariable;
}

std::string MainFrame::systematicWeight(const std::shared_ptr<Systematic>& systematic) const {

    const std::string branchName = "weight_total_" + systematic->name();

    if (!m_systReplacer.branchExists(branchName)) {
        return "weight_total_NOSYS";
    }

    return branchName;
}

std::vector<std::vector<ROOT::RDF::RNode> > MainFrame::applyFilters(ROOT::RDF::RNode mainNode,
                                                                    const std::shared_ptr<Sample>& sample) const {

    std::vector<std::vector<ROOT::RDF::RNode> > result;

    for (const auto& isyst : sample->systematics()) {
        std::vector<ROOT::RDF::RNode> perSystFilter;
        for (const auto& ireg : sample->regions()) {

            if (sample->skipSystematicRegionCombination(isyst, ireg)) {
                LOG(DEBUG) << "Skipping region: " << ireg->name() << ", systematic: " << isyst->name() << " combination for sample: " << sample->name() << " (filter)\n";
                continue;
            }

            auto filter = mainNode.Filter(this->systematicFilter(sample, isyst, ireg));
            perSystFilter.emplace_back(std::move(filter));
        }
        result.emplace_back(std::move(perSystFilter));
    }

    return result;
}

ROOT::RDF::RNode MainFrame::addWeightColumns(ROOT::RDF::RNode node,
                                             const std::shared_ptr<Sample>& sample,
                                             const UniqueSampleID& id) {

    for (const auto& isyst : sample->systematics()) {
        node = this->addSingleWeightColumn(node, sample, isyst, id);
    }

    return node;
}

ROOT::RDF::RNode MainFrame::addSingleWeightColumn(ROOT::RDF::RNode mainNode,
                                                  const std::shared_ptr<Sample>& sample,
                                                  const std::shared_ptr<Systematic>& systematic,
                                                  const UniqueSampleID& id) {

    const std::string& nominalWeight = sample->weight();
    const float normalisation = m_metadataManager.normalisation(id, systematic);

    // to not cut very small numbers to zero
    std::ostringstream ss;
    ss << normalisation;

    const std::string& systName = "weight_total_" + systematic->name();
    const std::string formula = m_systReplacer.replaceString(nominalWeight, systematic) + "*" + ss.str();
    const std::string nominalTotalWeight = nominalWeight + "*" + ss.str();
    if (!systematic->isNominal() && formula == nominalTotalWeight) {
        LOG(DEBUG) << "Sample: " << id << ", systematic: " << systematic->name() << ", does not impact the weight\n";
        return mainNode;
    }
    LOG(VERBOSE) << "Unique sample: " << id << ", systematic: " << systematic->name() << ", weight formula: " << formula << ", new weight name: " << systName << "\n";

    // add it to the list of branches
    m_systReplacer.addBranch(systName);

    auto node = mainNode.Define(systName, formula);
    return node;
}

ROOT::RDF::RNode MainFrame::addTLorentzVectors(ROOT::RDF::RNode mainNode) {
    std::vector<std::string> objects = {"jet", "el"};
    for (const auto& iobject : objects) {
        mainNode = this->addSingleTLorentzVector(mainNode, iobject);
    }

    return mainNode;
}

ROOT::RDF::RNode MainFrame::addSingleTLorentzVector(ROOT::RDF::RNode mainNode,
                                                    const std::string& object) {

    const std::vector<std::string> kinematics = {"_pt_NOSYS", "_eta", "_phi", "_e_NOSYS"};

    auto createTLV = [&kinematics](const std::vector<float>& pt,
                                   const std::vector<float>& eta,
                                   const std::vector<float>& phi,
                                   const std::vector<float>& e) {


        std::vector<ROOT::Math::PtEtaPhiEVector> result;
        for (std::size_t i = 0; i < pt.size(); ++i) {
            ROOT::Math::PtEtaPhiEVector vector(pt.at(i), eta.at(i), phi.at(i), e.at(i));
            result.emplace_back(vector);
        }

        return result;
    };

    std::vector<std::string> objectColumns;
    for (const auto& ikinematics : kinematics) {
        const std::string variable = object + ikinematics;
        objectColumns.emplace_back(std::move(variable));
    }

    const std::string vectorName = object + "_TLV_NOSYS";
    mainNode = this->systematicDefine(mainNode, vectorName, createTLV, objectColumns);

    return mainNode;
}

std::vector<SystematicHisto> MainFrame::processHistograms(std::vector<std::vector<ROOT::RDF::RNode> >& filters,
                                                          const std::shared_ptr<Sample>& sample) const {

    std::vector<SystematicHisto> result;

    std::size_t systIndex(0);
    for (const auto& isyst : sample->systematics()) {
        SystematicHisto systematicHisto(isyst->name());

        std::size_t regIndex(0);
        for (const auto& ireg : sample->regions()) {
            if (sample->skipSystematicRegionCombination(isyst, ireg)) {
                LOG(DEBUG) << "Skipping region: " << ireg->name() << ", systematic: " << isyst->name() << " combination for sample: " << sample->name() << " (histogram)\n";
                continue;
            }
            RegionHisto regionHisto(ireg->name());

            for (const auto& ivariable : ireg->variables()) {
                VariableHisto variableHisto(ivariable.name());

                ROOT::RDF::RResultPtr<TH1D> histogram;
                if (ivariable.hasRegularBinning()) {
                    histogram = filters.at(systIndex).at(regIndex).
                                    Histo1D({"", ivariable.title().c_str(), ivariable.axisNbins(), ivariable.axisMin(), ivariable.axisMax()},
                                    this->systematicVariable(ivariable, isyst), this->systematicWeight(isyst));
                } else {
                    const std::vector<double> binEdges = ivariable.binEdges();
                    histogram = filters.at(systIndex).at(regIndex).
                                    Histo1D({"", ivariable.title().c_str(), (int)(binEdges.size()-1), binEdges.data()},
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
        result.emplace_back(std::move(systematicHisto));
        ++systIndex;
    }

    return result;
}

void MainFrame::writeHistosToFile(const std::vector<SystematicHisto>& histos,
                                  const std::shared_ptr<Sample>& sample,
                                  const ROOT::RDF::RNode* node,
                                  const bool printEventLoopCount) const {

    if (histos.empty()) {
        LOG(WARNING) << "No histograms available for sample: " << sample->name() << "\n";
    }

    std::string fileName = m_config->outputPath();
    fileName += fileName.empty() ? "" : "/";
    fileName += sample->name() + ".root";

    std::unique_ptr<TFile> out(TFile::Open(fileName.c_str(), "RECREATE"));
    if (!out) {
        LOG(ERROR) << "Cannot open ROOT file at: " << fileName << "\n";
        throw std::invalid_argument("");
    }

    LOG(INFO) << "Writing histograms to file: " << fileName << "\n";
    if (printEventLoopCount) {
        LOG(INFO) << "Triggering event loop!\n";
    }

    for (const auto& isystHist : histos) {
        if (isystHist.regionHistos().empty()) {
            LOG(WARNING) << "No histograms available for sample: " << sample->name() << ", systematic: " << isystHist.name() << "\n";
            continue;
        }
        out->mkdir(isystHist.name().c_str());
        out->cd(isystHist.name().c_str());
        for (const auto& iregionHist : isystHist.regionHistos()) {
            for (const auto& ivariableHist : iregionHist.variableHistos()) {
                const std::string histoName = StringOperations::replaceString(ivariableHist.name(), "_NOSYS", "") + "_" + iregionHist.name();
                ivariableHist.histo()->Write(histoName.c_str());
            }
        }
    }
    if (printEventLoopCount) {
        LOG(INFO) << "Number of event loops: " << node->GetNRuns() << ". For an optimal run, this number should be 1\n";
    }

    out->Close();
}

void MainFrame::readAutomaticSystematics(std::shared_ptr<Sample>& sample, const bool isNominalOnly) const {

    // clear current systematics
    sample->clearSystematics();

    // add nominal "systematic"
    auto nominal = std::make_shared<Systematic>("NOSYS");
    nominal->setSumWeights("NOSYS");
    for (const auto& ireg : m_config->regions()) {
        nominal->addRegion(ireg);
    }
    sample->addSystematic(nominal);

    m_config->addUniqueSystematic(nominal);

    if (isNominalOnly) return;

    // add systematics now
    for (const auto& iuniqueSample : sample->uniqueSampleIDs()) {
        if (iuniqueSample.simulation() == "data") return; // nothing to add for data
        const auto fileList = m_metadataManager.filePaths(iuniqueSample);
        if (fileList.empty()) continue;

        const std::vector<std::string> listOfSystematics = this->automaticSystematicNames(fileList.at(0));
        // now add the systematics
        for (const auto& isyst : listOfSystematics) {
            auto syst = std::make_shared<Systematic>(isyst);
            if (m_metadataManager.sumWeightsExist(iuniqueSample, syst)) {
                syst->setSumWeights(isyst);
            } else {
                syst->setSumWeights("NOSYS");
            }
            for (const auto& ireg : m_config->regions()) {
                syst->addRegion(ireg);
            }
            sample->addSystematic(syst);
            m_config->addUniqueSystematic(syst);
        }

        break;
    }
}

std::vector<std::string> MainFrame::automaticSystematicNames(const std::string& path) const {
    std::unique_ptr<TFile> in(TFile::Open(path.c_str(), "READ"));
    if (!in) {
        LOG(ERROR) << "Cannot open ROOT file at: " << path << "\n";
        throw std::invalid_argument("");
    }

    std::unique_ptr<TH1F> hist(in->Get<TH1F>("listOfSystematics"));
    if (!hist) {
        LOG(ERROR) << "Cannot read histogram: listOfSystematics\n";
        throw std::invalid_argument("");
    }
    hist->SetDirectory(nullptr);

    std::vector<std::string> result;

    for (int ibin = 1; ibin < hist->GetNbinsX(); ++ibin) {
        const std::string name = hist->GetXaxis()->GetBinLabel(ibin);
        if (name == "NOSYS") continue;

        LOG(VERBOSE) << "Adding systematic from histogram: " << name << "\n";
        result.emplace_back(name);
    }

    return result;
}
