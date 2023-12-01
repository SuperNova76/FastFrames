/**
 * @file MainFrame.cc
 * @brief Main class responsible for histogramming and ntupling
 *
 */

#include "FastFrames/MainFrame.h"

#include "FastFrames/Logger.h"
#include "FastFrames/ObjectCopier.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"
#include "FastFrames/Utils.h"

#include "TChain.h"
#include "TSystem.h"
#include "Math/Vector4D.h"
#include "ROOT/RDFHelpers.hxx"

#include <iostream>
#include <exception>

void MainFrame::init() {
    if (m_config->minEvent() >= 0 || m_config->maxEvent() >= 0) {
        ROOT::DisableImplicitMT();
        LOG(WARNING) << "Disabling implicit MT as it is not allowed for Range() call\n";
    } else {
        ROOT::EnableImplicitMT(m_config->numCPU());
        LOG(INFO) << "Enabling implicit MT with " << m_config->numCPU() << " threads\n";
    }
    m_metadataManager.readFileList( m_config->inputFilelistPath() );
    m_metadataManager.readSumWeights( m_config->inputSumWeightsPath() );
    m_metadataManager.readXSectionFiles( m_config->xSectionFiles() );

    // propagate luminosity information from config
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

    LOG(INFO) << "-------------------------------------\n";
    LOG(INFO) << "Started the main histogram processing\n";
    LOG(INFO) << "-------------------------------------\n";
    std::size_t sampleN(1);
    for (const auto& isample : m_config->samples()) {
        LOG(INFO) << "\n";
        LOG(INFO) << "Processing sample: " << sampleN << " out of " << m_config->samples().size() << " samples\n";

        std::vector<SystematicHisto> finalSystHistos;
        std::vector<VariableHisto> finalTruthHistos;
        std::size_t uniqueSampleN(1);
        ROOT::RDF::RNode* node(nullptr);
        for (const auto& iUniqueSampleID : isample->uniqueSampleIDs()) {
            LOG(INFO) << "\n";
            LOG(INFO) << "Processing unique sample: " << iUniqueSampleID << ", " << uniqueSampleN << " out of " << isample->uniqueSampleIDs().size() << " unique samples\n";

            auto currentHistos = this->processUniqueSample(isample, iUniqueSampleID);
            auto& systematicHistos = std::get<0>(currentHistos);
            auto& truthHistos      = std::get<1>(currentHistos);
            node                   = &std::get<2>(currentHistos);
            // this happens when there are no files provided
            if (systematicHistos.empty()) continue;

            // merge the histograms or take them if it is the first set
            if (finalSystHistos.empty())  {
                LOG(DEBUG) << "First set of histograms for this sample, this will NOT trigger event loop\n";
                finalSystHistos = std::move(systematicHistos);
            } else {
                if (systematicHistos.size() != finalSystHistos.size()) {
                    LOG(ERROR) << "Number of the systematic histograms do not match\n";
                    LOG(ERROR) << "Size of the current histograms: " << systematicHistos.size() << ", final histograms: " << finalSystHistos.size() << "\n";
                    throw std::runtime_error("");
                }

                LOG(INFO) << "Merging samples, triggers event loop for the reco tree!\n";
                for (std::size_t isyst = 0; isyst < finalSystHistos.size(); ++isyst) {
                    finalSystHistos.at(isyst).merge(systematicHistos.at(isyst));
                }
                LOG(INFO) << "Number of event loops: " << node->GetNRuns() << ". For an optimal run, this number should be 1\n";
            }
            if (!truthHistos.empty()) {
                if (finalTruthHistos.empty()) {
                    LOG(DEBUG) << "First set of histograms for this sample for truth, this will NOT trigger event loop\n";
                    finalTruthHistos = std::move(truthHistos);
                } else {
                    LOG(INFO) << "Merging truth, triggers event loop for the truth trees!\n";
                    if (finalTruthHistos.size() != truthHistos.size()) {
                        LOG(ERROR) << "Sizes of truth histograms do not match!\n";
                        throw std::runtime_error("");
                    }
                    for (std::size_t ihist = 0; ihist < truthHistos.size(); ++ihist) {
                        finalTruthHistos.at(ihist).mergeHisto(truthHistos.at(ihist).histo());
                    }
                }
            }
            ++uniqueSampleN;
        }

        bool printEventLoop = isample->uniqueSampleIDs().size() == 1;
        this->writeHistosToFile(finalSystHistos, finalTruthHistos, isample, node, printEventLoop && node);
        ++sampleN;
    }
}

void MainFrame::executeNtuples() {
    if (m_config->nominalOnly() || m_config->automaticSystematics()) {
        for (auto& isample : m_config->ntuple()->samples()) {
            this->readAutomaticSystematics(isample, m_config->nominalOnly());
        }
    }

    LOG(INFO) << "----------------------------------\n";
    LOG(INFO) << "Started the main ntuple processing\n";
    LOG(INFO) << "----------------------------------\n";
    std::size_t sampleN(1);
    for (const auto& isample : m_config->ntuple()->samples()) {
        LOG(INFO) << "\n";
        LOG(INFO) << "Processing sample: " << sampleN << " out of " << m_config->samples().size() << " samples\n";
        std::size_t uniqueSampleN(1);
        for (const auto& iUniqueSampleID : isample->uniqueSampleIDs()) {
            LOG(INFO) << "\n";
            LOG(INFO) << "Processing unique sample: " << iUniqueSampleID << ", " << uniqueSampleN << " out of " << isample->uniqueSampleIDs().size() << " unique samples\n";
            this->processUniqueSampleNtuple(isample, iUniqueSampleID);
            ++uniqueSampleN;
        }
        ++sampleN;
    }
}

std::tuple<std::vector<SystematicHisto>,
           std::vector<VariableHisto>,
           ROOT::RDF::RNode> MainFrame::processUniqueSample(const std::shared_ptr<Sample>& sample,
                                                            const UniqueSampleID& uniqueSampleID) {

    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(uniqueSampleID);
    std::vector<std::string> selectedFilePaths(filePaths);
    if (Utils::sampleHasUnfolding(sample) && m_config->totalJobSplits() > 0) {
        LOG(WARNING) << "Sample: " << sample->name() << " has unfolding set, cannot split into more jobs\n";
    } else if (m_config->totalJobSplits() > 0) {
        selectedFilePaths = Utils::selectedFileList(filePaths, m_config->totalJobSplits(), m_config->currentJobIndex());
    }
    if (selectedFilePaths.empty()) {
        LOG(WARNING) << "UniqueSample: " << uniqueSampleID << " has no files, will not produce histograms\n";
        ROOT::RDataFrame tmp("", {});
        return std::make_tuple(std::vector<SystematicHisto>{}, std::vector<VariableHisto>{}, tmp);
    }

    std::unique_ptr<TChain> recoChain = Utils::chainFromFiles(sample->recoTreeName(), selectedFilePaths);

    if (sample->hasTruth()) {
        this->connectTruthTrees(recoChain, sample, selectedFilePaths);
    }

    std::vector<VariableHisto> truthHistos;

    if (sample->hasTruth()) {
        truthHistos = std::move(this->processTruthHistos(selectedFilePaths, sample, uniqueSampleID));
    }

    // we could use any file from the list, use the first one
    m_systReplacer.readSystematicMapFromFile(selectedFilePaths.at(0), sample->recoTreeName(), sample->systematics());

    ROOT::RDataFrame df(*recoChain.release());
    ROOT::RDF::RNode mainNode = df;

    #if ROOT_VERSION_CODE > ROOT_VERSION(6,29,0)
    ROOT::RDF::Experimental::AddProgressBar(mainNode);
    #endif

    mainNode = this->minMaxRange(mainNode);

    mainNode = this->addWeightColumns(mainNode, sample, uniqueSampleID);

    // add TLorentzVectors for objects
    mainNode = this->addTLorentzVectors(mainNode);

    mainNode = this->addCustomDefinesFromConfig(mainNode, sample);

    // we also need to add truth variables if provided
    for (const auto& itruth : sample->truths()) {
        mainNode = this->addCustomTruthDefinesFromConfig(mainNode, itruth);
        mainNode = this->defineVariablesTruth(mainNode, itruth, uniqueSampleID);
    }

    // this is the method users will be able to override
    mainNode = this->defineVariables(mainNode, uniqueSampleID);

    m_systReplacer.printMaps();

    std::vector<std::vector<ROOT::RDF::RNode> > filterStore = this->applyFilters(mainNode, sample);

    // retrieve the histograms;
    std::vector<SystematicHisto> histoContainer = this->processHistograms(filterStore, sample);

    return std::make_tuple(std::move(histoContainer), std::move(truthHistos), mainNode);
}

void MainFrame::processUniqueSampleNtuple(const std::shared_ptr<Sample>& sample,
                                          const UniqueSampleID& id) {

    const std::vector<std::string>& filePaths = m_metadataManager.filePaths(id);
    std::vector<std::string> selectedFilePaths(filePaths);
    if (m_config->totalJobSplits() > 0) {
        selectedFilePaths = Utils::selectedFileList(filePaths, m_config->totalJobSplits(), m_config->currentJobIndex());
    }
    if (selectedFilePaths.empty()) {
        LOG(WARNING) << "UniqueSample: " << id << " has no files, will not produce output ntuple\n";
        return;
    }

    auto chain = Utils::chainFromFiles(sample->recoTreeName(), selectedFilePaths);

    if (sample->hasTruth()) {
        this->connectTruthTrees(chain, sample, selectedFilePaths);
    }
    // we could use any file from the list, use the first one
    m_systReplacer.readSystematicMapFromFile(selectedFilePaths.at(0), sample->recoTreeName(), sample->systematics());

    ROOT::RDataFrame df(*chain.release());

    ROOT::RDF::RNode mainNode = df;
    #if ROOT_VERSION_CODE > ROOT_VERSION(6,29,0)
    ROOT::RDF::Experimental::AddProgressBar(mainNode);
    #endif
    mainNode = this->minMaxRange(mainNode);

    mainNode = this->addWeightColumns(mainNode, sample, id);

    // add TLorentzVectors for objects
    mainNode = this->addTLorentzVectors(mainNode);

    mainNode = this->addCustomDefinesFromConfig(mainNode, sample);

    // this is the method users will be able to override
    mainNode = this->defineVariablesNtuple(mainNode, id);

    m_systReplacer.printMaps();

    // apply filter
    if (!m_config->ntuple()->selection().empty()) {
        mainNode.Filter(this->systematicOrFilter(sample));
    }

    //store the file
    const std::string folder = m_config->outputPathNtuples().empty() ? "" : m_config->outputPathNtuples() + "/";
    const std::string fileName = folder + sample->name() + "_" + std::to_string(id.dsid())+"_" + id.campaign() + "_"+id.simulation() + ".root";
    const std::vector<std::string> selectedBranches = m_config->ntuple()->listOfSelectedBranches(m_systReplacer.allBranches());
    LOG(VERBOSE) << "List of selected branches:\n";
    for (const auto& iselected : selectedBranches) {
        LOG(VERBOSE) << "\t" << iselected << "\n";
    }
    LOG(INFO) << "Triggering event loop for the reco tree!\n";
    mainNode.Snapshot(sample->recoTreeName(), fileName, selectedBranches);
    LOG(INFO) << "Number of event loops: " << mainNode.GetNRuns() << ". For an optimal run, this number should be 1\n";

    ObjectCopier copier(selectedFilePaths);
    copier.readObjectInfo();
    if (!m_config->ntuple()->copyTrees().empty()) {
        copier.copyTreesTo(fileName, m_config->ntuple()->copyTrees());
    }
    LOG(INFO) << "Copying metadata from the original files\n";
    copier.copyObjectsTo(fileName);
    LOG(INFO) << "Finished copying metadata from the original files\n";
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

std::string MainFrame::systematicOrFilter(const std::shared_ptr<Sample>& sample) const {
    const std::string& nominalSelection = m_config->ntuple()->selection();

    std::string result = "(" + nominalSelection + ")";
    for (const auto& isyst : sample->systematics()) {
        const std::string systSelection = m_systReplacer.replaceString(nominalSelection, isyst);
        if (systSelection == nominalSelection) continue;

        result += "||(" + systSelection + ")";
    }

    LOG(DEBUG) << "Final selection used for filtering ntuples: " << result << "\n";

    return result;
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

    const std::string systName = "weight_total_" + systematic->name();
    const std::string nominalTotalWeight = "(" + nominalWeight + ")*(" + ss.str() +")";
    std::string formula = m_systReplacer.replaceString("("+nominalWeight, systematic) + ")*(" + ss.str() + ")";
    if (!systematic->weightSuffix().empty()) {
        formula = "(" + formula + ")*(" + systematic->weightSuffix() + ")";
    }
    if (!systematic->isNominal() && formula == nominalTotalWeight) {
        LOG(DEBUG) << "Sample: " << id << ", systematic: " << systematic->name() << ", does not impact the weight\n";
        return mainNode;
    }
    LOG(VERBOSE) << "Unique sample: " << id << ", systematic: " << systematic->name() << ", weight formula: " << formula << ", new weight name: " << systName << "\n";

    if (m_systReplacer.branchExists(systName)) {
        LOG(DEBUG) << "Branch: " << systName << " already exists, not adding it\n";
        return mainNode;
    }

    // add it to the list of branches
    m_systReplacer.addBranch(systName);

    auto node = mainNode.Define(systName, formula);
    return node;
}

ROOT::RDF::RNode MainFrame::addTLorentzVectors(ROOT::RDF::RNode mainNode) {
    const std::vector<std::string>& objects = m_config->tLorentzVectors();
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
    if (m_systReplacer.branchExists(vectorName)) {
        LOG(DEBUG) << "Branch: " << vectorName << " already exists, not adding it (nor its uncertainty variations)\n";
        return mainNode;
    }
    mainNode = this->systematicDefine(mainNode, vectorName, createTLV, objectColumns);

    return mainNode;
}

std::vector<SystematicHisto> MainFrame::processHistograms(std::vector<std::vector<ROOT::RDF::RNode> >& filters,
                                                          const std::shared_ptr<Sample>& sample) {

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

            ROOT::RDF::RNode& node = filters.at(systIndex).at(regIndex);

            this->processHistograms1D(&regionHisto, node, sample, ireg, isyst);

            this->processHistograms2D(&regionHisto, node, sample, ireg, isyst);

            if (sample->hasTruth()) {
                this->processTruthHistograms1D(&regionHisto, node, sample, ireg, isyst);
            }

            this->processTruthHistograms2D(&regionHisto, node, sample, ireg, isyst);

            systematicHisto.addRegionHisto(std::move(regionHisto));
            ++regIndex;
        }
        result.emplace_back(std::move(systematicHisto));
        ++systIndex;
    }

    return result;
}

void MainFrame::writeHistosToFile(const std::vector<SystematicHisto>& histos,
                                  const std::vector<VariableHisto>& truthHistos,
                                  const std::shared_ptr<Sample>& sample,
                                  const ROOT::RDF::RNode* node,
                                  const bool printEventLoopCount) const {

    if (histos.empty()) {
        LOG(WARNING) << "No histograms available for sample: " << sample->name() << "\n";
    }

    std::string suffix("");
    if (!Utils::sampleHasUnfolding(sample) && m_config->totalJobSplits() > 0) {
        suffix = "_Njobs_" + std::to_string(m_config->totalJobSplits()) + "_jobIndex_" + std::to_string(m_config->currentJobIndex());
    }

    std::string fileName = m_config->outputPathHistograms();
    fileName += fileName.empty() ? "" : "/";
    fileName += sample->name() + suffix + ".root";


    std::unique_ptr<TFile> out(TFile::Open(fileName.c_str(), "RECREATE"));
    if (!out) {
        LOG(ERROR) << "Cannot open ROOT file at: " << fileName << "\n";
        throw std::invalid_argument("");
    }

    LOG(INFO) << "Writing histograms to file: " << fileName << "\n";

    this->writeUnfoldingHistos(out.get(), histos, truthHistos, sample);

    if (printEventLoopCount && sample->truths().empty()) {
        LOG(INFO) << "Triggering event loop for the reco tree!\n";
    }
    bool first(true);
    for (const auto& isystHist : histos) {
        if (isystHist.regionHistos().empty()) {
            LOG(WARNING) << "No histograms available for sample: " << sample->name() << ", systematic: " << isystHist.name() << "\n";
            continue;
        }
        if (!out->GetDirectory(isystHist.name().c_str())) {
            out->cd();
            out->mkdir(isystHist.name().c_str());
        }
        for (const auto& iregionHist : isystHist.regionHistos()) {

            // 1D histograms
            for (const auto& ivariableHist : iregionHist.variableHistos()) {
                const std::string histoName = StringOperations::replaceString(ivariableHist.name(), "_NOSYS", "") + "_" + iregionHist.name();
                if (first) {
                    std::unique_ptr<TH1D> histoCopy(static_cast<TH1D*>(ivariableHist.histo()->Clone()));
                    histoCopy->SetDirectory(nullptr);
                    out->cd(isystHist.name().c_str());
                    histoCopy->Write(histoName.c_str());
                    first = false;
                } else {
                    out->cd(isystHist.name().c_str());
                    ivariableHist.histo()->Write(histoName.c_str());
                }
            }

            // 2D histograms
            for (const auto& ivariableHist2D : iregionHist.variableHistos2D()) {
                const std::string histo2DName = StringOperations::replaceString(ivariableHist2D.name(), "_NOSYS", "") + "_" + iregionHist.name();
                out->cd(isystHist.name().c_str());
                ivariableHist2D.histo()->Write(histo2DName.c_str());
            }
        }
    }

    // Write truth histograms
    for (const auto& itruthHist : truthHistos) {
        const std::string truthHistoName = StringOperations::replaceString(itruthHist.name(), "_NOSYS", "");
        out->cd();
        itruthHist.histo()->Write(truthHistoName.c_str());
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
        if (iuniqueSample.isData()) return; // nothing to add for data
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

void MainFrame::processHistograms1D(RegionHisto* regionHisto,
                                    ROOT::RDF::RNode& node,
                                    const std::shared_ptr<Sample>& sample,
                                    const std::shared_ptr<Region>& region,
                                    const std::shared_ptr<Systematic>& systematic) const {

    for (const auto& ivariable : region->variables()) {
        const std::vector<std::string>& variables = sample->variables();
        auto itrVar = std::find(variables.begin(), variables.end(), ivariable.name());
        if (itrVar == variables.end()) {
            LOG(DEBUG) << "Skippping variable: " << ivariable.name() << " for sample: " << sample->name() << "\n";
            continue;
        }
        VariableHisto variableHisto(ivariable.name());

        ROOT::RDF::RResultPtr<TH1D> histogram = node.Histo1D(
                                                    ivariable.histoModel1D(),
                                                    this->systematicVariable(ivariable, systematic),
                                                    this->systematicWeight(systematic)
                                                );

        if (!histogram) {
            LOG(ERROR) << "Histogram for sample: " << sample->name() << ", systematic: "
                       << systematic->name() << ", region: " << region->name() << " and variable: " << ivariable.name() << " is empty!\n";
            throw std::runtime_error("");

        }
        variableHisto.setHisto(histogram);

        regionHisto->addVariableHisto(std::move(variableHisto));
    }
}

void MainFrame::processHistograms2D(RegionHisto* regionHisto,
                                    ROOT::RDF::RNode& node,
                                    const std::shared_ptr<Sample>& sample,
                                    const std::shared_ptr<Region>& region,
                                    const std::shared_ptr<Systematic>& systematic) const {

    for (const auto& combinations : region->variableCombinations()) {
        const Variable& v1 = region->variableByName(combinations.first);
        const Variable& v2 = region->variableByName(combinations.second);
        const std::string name = v1.name() + "_vs_" + v2.name();
        
        const std::vector<std::string>& variables = sample->variables();
        auto itrVar1 = std::find(variables.begin(), variables.end(), v1.name());
        auto itrVar2 = std::find(variables.begin(), variables.end(), v2.name());
        if (itrVar1 == variables.end() || itrVar2 == variables.end()) {
            LOG(DEBUG) << "Skippping variable (2D): " << name << " for sample: " << sample->name() << "\n";
            continue;
        }

        VariableHisto2D variableHisto2D(name);
        ROOT::RDF::RResultPtr<TH2D> histogram2D = node.Histo2D(
                                                    Utils::histoModel2D(v1, v2),
                                                    this->systematicVariable(v1, systematic),
                                                    this->systematicVariable(v2, systematic),
                                                    this->systematicWeight(systematic)
                                                );

        if (!histogram2D) {
            LOG(ERROR) << "Histogram for sample: " << sample->name() << ", systematic: "
                       << systematic->name() << ", region: " << region->name() << " and variable combination: " << v1.name() << " & " << v2.name() << " is empty!\n";
            throw std::runtime_error("");

        }

        variableHisto2D.setHisto(histogram2D);

        regionHisto->addVariableHisto2D(std::move(variableHisto2D));
    }
}

void MainFrame::processTruthHistograms1D(RegionHisto* regionHisto,
                                         ROOT::RDF::RNode& node,
                                         const std::shared_ptr<Sample>& sample,
                                         const std::shared_ptr<Region>& region,
                                         const std::shared_ptr<Systematic>& systematic) {

    for (const auto& itruth : sample->truths()) {
        if (!itruth->produceUnfolding()) continue;
        ROOT::RDF::RNode passedNode = node;
        ROOT::RDF::RNode failedNode = node;
        if (!itruth->selection().empty()) {
            passedNode = passedNode.Filter(itruth->selection());
            const std::string failedSelection = "!(" + itruth->selection() + ")";
            failedNode = failedNode.Filter(failedSelection);
        } else {
            // should be empty if there is no selection
            failedNode = failedNode.Filter([](){return false;}, {});
        }
        for (const auto& imatch : itruth->matchedVariables()) {
            const Variable& truthVariable = itruth->variableByName(imatch.second);

            VariableHisto variableHistoPassed(truthVariable.name() + "_passed");
            VariableHisto variableHistoFailed(truthVariable.name() + "_failed");

            ROOT::RDF::RResultPtr<TH1D> histogramPassed = passedNode.Histo1D(
                                                            truthVariable.histoModel1D(),
                                                            this->systematicVariable(truthVariable, systematic),
                                                            this->systematicWeight(systematic)
                                                          );
            ROOT::RDF::RResultPtr<TH1D> histogramFailed = failedNode.Histo1D(
                                                            truthVariable.histoModel1D(),
                                                            this->systematicVariable(truthVariable, systematic),
                                                            this->systematicWeight(systematic)
                                                          );

            if (!histogramPassed || !histogramFailed) {
                LOG(ERROR) << "Histogram for sample: " << sample->name() << ", systematic: "
                           << systematic->name() << ", region: " << region->name() << " and variable: " << truthVariable.name() << " is empty!\n";
                throw std::runtime_error("");

            }
            variableHistoPassed.setHisto(histogramPassed);
            variableHistoFailed.setHisto(histogramFailed);

            regionHisto->addVariableHisto(std::move(variableHistoPassed));
            regionHisto->addVariableHisto(std::move(variableHistoFailed));
        }
    }
}

void MainFrame::processTruthHistograms2D(RegionHisto* regionHisto,
                                         ROOT::RDF::RNode& node,
                                         const std::shared_ptr<Sample>& sample,
                                         const std::shared_ptr<Region>& region,
                                         const std::shared_ptr<Systematic>& systematic) {

    for (const auto& itruth : sample->truths()) {
        ROOT::RDF::RNode passedNode = node;
        if (!itruth->selection().empty()) {
            passedNode = node.Filter(itruth->selection());
        }
        for (const auto& imatch : itruth->matchedVariables()) {
            const Variable& recoVariable  = region->variableByName(imatch.first);
            const Variable& truthVariable = itruth->variableByName(imatch.second);

            const std::string name = recoVariable.name() + "_vs_" + truthVariable.name();
            VariableHisto2D variableHistoPassed(name);

            ROOT::RDF::RResultPtr<TH2D> histogramPassed = passedNode.Histo2D(
                                                            Utils::histoModel2D(truthVariable, recoVariable),
                                                            this->systematicVariable(truthVariable, systematic),
                                                            this->systematicVariable(recoVariable, systematic),
                                                            this->systematicWeight(systematic)
                                                          );

            if (!histogramPassed) {
                LOG(ERROR) << "Histogram for sample: " << sample->name() << ", systematic: "
                           << systematic->name() << ", region: " << region->name() << " and variable: " << truthVariable.name() << " is empty!\n";
                throw std::runtime_error("");

            }
            variableHistoPassed.setHisto(histogramPassed);

            regionHisto->addVariableHisto2D(std::move(variableHistoPassed));
        }
    }
}

void MainFrame::connectTruthTrees(std::unique_ptr<TChain>& chain,
                                  const std::shared_ptr<Sample>& sample,
                                  const std::vector<std::string>& filePaths) const {

    for (const auto& itruth : sample->uniqueTruthTreeNames()) {

        const std::vector<std::string>& indexNames = sample->recoToTruthPairingIndices();
        if (indexNames.empty() || indexNames.size() > 2) {
            LOG(ERROR) << "Reco to truth index names for sample: " << sample->name() << " are 0 or > 2\n";
            throw std::invalid_argument("");
        }

        LOG(INFO) << "Attaching tree: " << itruth << " to the reco tree\n";
        TChain* truthChain = Utils::chainFromFiles(itruth, filePaths).release();
        if (indexNames.size() == 1 ) {
            LOG(INFO) << "Building reco truth index with: " << indexNames.at(0) << "\n";
            truthChain->BuildIndex(indexNames.at(0).c_str());
        } else {
            LOG(INFO) << "Building reco truth index with: " << indexNames.at(0) << " and " << indexNames.at(1) << "\n";
            truthChain->BuildIndex(indexNames.at(0).c_str(), indexNames.at(1).c_str());
        }
        chain->AddFriend(truthChain);
    }
}

std::vector<VariableHisto> MainFrame::processTruthHistos(const std::vector<std::string>& filePaths,
                                                         const std::shared_ptr<Sample>& sample,
                                                         const UniqueSampleID& id) {

    std::vector<VariableHisto> result;

    const std::vector<std::string>& uniqueTreeNames = sample->uniqueTruthTreeNames();
    std::map<std::string, ROOT::RDF::RNode> rdfNodes;
    for (const auto& iTree : uniqueTreeNames) {
        rdfNodes.insert(std::make_pair(iTree, ROOT::RDataFrame(iTree, filePaths)));
    }

    for (const auto& itruth : sample->truths()) {
        // add MC weight
        const float normalisation = m_metadataManager.normalisation(id, sample->nominalSystematic());

        auto itr = rdfNodes.find(itruth->truthTreeName());
        if (itr == rdfNodes.end()) {
            LOG(ERROR) << "Cannot find truth tree name: " << itruth->truthTreeName() << "in the map!\n";
            throw std::runtime_error("");
        }

        ROOT::RDF::RNode mainNode = itr->second;
        #if ROOT_VERSION_CODE > ROOT_VERSION(6,29,0)
        ROOT::RDF::Experimental::AddProgressBar(mainNode);
        #endif
        mainNode = this->minMaxRange(mainNode);

        // to not cut very small numbers to zero
        std::ostringstream ss;
        ss << normalisation;
        const std::string totalWeight = "(" + itruth->eventWeight() + ")*(" + ss.str() +")";
        LOG(DEBUG) << "Adding column: weight_truth_TOTAL with formula " << totalWeight << "\n";
        mainNode = mainNode.Define("weight_truth_TOTAL", totalWeight);

        mainNode = this->addCustomTruthDefinesFromConfig(mainNode, itruth);

        auto customNode = this->defineVariablesTruth(mainNode, itruth, id);

        // apply truth filter
        if (!itruth->selection().empty()) {
            customNode = customNode.Filter(itruth->selection());
        }

        // add histograms
        for (const auto& ivariable : itruth->variables()) {
            // get histograms (will NOT trigger event loop)
            const std::string name = itruth->name() + "_" + ivariable.name();
            VariableHisto hist(name);
            auto rdfHist = customNode.Histo1D(ivariable.histoModel1D(), ivariable.definition(), "weight_truth_TOTAL");

            hist.setHisto(rdfHist);

            result.emplace_back(std::move(hist));
        }
    }

    return result;
}

void MainFrame::writeUnfoldingHistos(TFile* outputFile,
                                     const std::vector<SystematicHisto>& histos,
                                     const std::vector<VariableHisto>& truthHistos,
                                     const std::shared_ptr<Sample>& sample) const {

    bool first(true);
    for (const auto& itruth : sample->truths()) {
        LOG(INFO) << "Triggering event loop for truth histograms for truth tree: " << itruth->name() << "\n";
        if (!itruth->produceUnfolding()) continue;
        for (const auto& imatch : itruth->matchedVariables()) {
            const std::string& truthName = itruth->name() + "_" + imatch.second;
            const std::string& selectionPassed = imatch.second + "_passed";
            const std::string& selectionFailed = imatch.second + "_failed";

            std::unique_ptr<TH1D> truth = Utils::copyHistoFromVariableHistos(truthHistos, truthName);
            for (const auto& isystHist : histos) {
                if (isystHist.regionHistos().empty()) {
                    LOG(WARNING) << "No histograms available for sample: " << sample->name() << ", systematic: " << isystHist.name() << "\n";
                    continue;
                }
                outputFile->cd();
                if (!outputFile->GetDirectory(isystHist.name().c_str())) {
                    outputFile->mkdir(isystHist.name().c_str());
                }
                for (const auto& iregionHist : isystHist.regionHistos()) {
                    if (first) {
                        LOG(INFO) << "Triggering event loop for the reco tree\n";
                        first = false;
                    }
                    std::unique_ptr<TH1D> passed = Utils::copyHistoFromVariableHistos(iregionHist.variableHistos(), selectionPassed);
                    std::unique_ptr<TH1D> failed = Utils::copyHistoFromVariableHistos(iregionHist.variableHistos(), selectionFailed);

                    std::unique_ptr<TH1D> selectionEff(static_cast<TH1D*>(passed->Clone()));
                    // selection eff = truth events passing reco selection/all
                    selectionEff->Divide(truth.get());

                    // acceptance = events passing reco and truth selection / events passing reco (failed truth + passed truth)
                    std::unique_ptr<TH1D> total(static_cast<TH1D*>(passed->Clone()));
                    total->Add(failed.get());

                    // this is now the acceptance
                    passed->Divide(total.get());

                    const std::string selectionEffName = "selection_eff_" + itruth->name() + "_" + truthName + "_" + iregionHist.name();
                    const std::string acceptanceName   = "acceptance_"    + itruth->name() + "_" + truthName + "_" + iregionHist.name();

                    outputFile->cd(isystHist.name().c_str());
                    selectionEff->Write(selectionEffName.c_str());
                    passed->Write(acceptanceName.c_str());
                }
            }
        }
    }
}

ROOT::RDF::RNode MainFrame::addCustomDefinesFromConfig(ROOT::RDF::RNode mainNode,
                                                       const std::shared_ptr<Sample>& sample) {

    if (sample->customDefines().empty()) return mainNode;

    for (const auto& idefine : sample->customDefines()) {
        const std::string& name = idefine.first;
        const std::string& expression = idefine.second;
        LOG(DEBUG) << "Adding custom variable from config: " << name << ", formula: " << expression << "\n";
        mainNode = this->systematicStringDefine(mainNode, name, expression);
    }

    return mainNode;
}

ROOT::RDF::RNode MainFrame::systematicStringDefine(ROOT::RDF::RNode mainNode,
                                                   const std::string& name,
                                                   const std::string& formula) {

    if (name.find("NOSYS") == std::string::npos) {
        LOG(ERROR) << "The variable: " << name << ", does not contain \"NOSYS\"\n";
        throw std::invalid_argument("");
    }

    // add nominal
    mainNode = mainNode.Define(name, formula);

    // first find on which variables the formula depends that are affected by systematics
    const std::vector<std::string> affectedVariables = m_systReplacer.listOfVariablesAffected(formula);

    const std::vector<std::string> systematicList = m_systReplacer.getListOfEffectiveSystematics(affectedVariables);

    // now propagate
    for (const auto& isyst : systematicList) {
        if (isyst == "NOSYS") continue; // we already added nominal
        const std::string newName = StringOperations::replaceString(name, "NOSYS", isyst);
        const std::string newFormula = m_systReplacer.replaceString(formula, isyst);
        LOG(VERBOSE) << "Adding custom variable from config: " << newName << ", formula: " << newFormula << "\n";

        mainNode = mainNode.Define(newName, newFormula);

    }
    m_systReplacer.addVariableAndEffectiveSystematics(name, systematicList);

    return mainNode;
}

ROOT::RDF::RNode MainFrame::addCustomTruthDefinesFromConfig(ROOT::RDF::RNode mainNode,
                                                            const std::shared_ptr<Truth>& truth) {

    for (const auto& ivariable : truth->customDefines()) {
        mainNode = mainNode.Define(ivariable.first, ivariable.second);
    }

    return mainNode;
}

ROOT::RDF::RNode MainFrame::minMaxRange(ROOT::RDF::RNode node) const {
    if (m_config->minEvent() >= 0 || m_config->maxEvent() >= 0) {
        long long int min = m_config->minEvent() >= 0 ? m_config->minEvent() : 0;
        long long int max = m_config->maxEvent() >= 0 ? m_config->maxEvent() : 0;
        LOG(INFO) << "Will only run for range: [" << min << "," << max << ")\n";
        return node.Range(min, max);
    }

    return node;
}