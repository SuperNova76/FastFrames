/**
 * @file Utils.cc
 * @brief Helper functions
 *
 */

#include "FastFrames/Utils.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Sample.h"

#include <algorithm>
#include <exception>

std::unique_ptr<TChain> Utils::chainFromFiles(const std::string& treeName,
                                              const std::vector<std::string>& files) {

    std::unique_ptr<TChain> chain = std::make_unique<TChain>(treeName.c_str());

    for (const auto& ifile : files) {
        chain->AddFile(ifile.c_str());
    }

    return chain;
}

ROOT::RDF::TH2DModel Utils::histoModel2D(const Variable& v1, const Variable& v2) {
    if (v1.hasRegularBinning() && v2.hasRegularBinning()) {
        return ROOT::RDF::TH2DModel("", "", v1.axisNbins(), v1.axisMin(), v1.axisMax(), v2.axisNbins(), v2.axisMin(), v2.axisMax());
    }
    if (v1.hasRegularBinning() && !v2.hasRegularBinning()) {
        const std::vector<double>& binEdges2 = v2.binEdges();
        return ROOT::RDF::TH2DModel("", "", v1.axisNbins(), v1.axisMin(), v1.axisMax(), binEdges2.size() - 1, binEdges2.data());
    }
    if (!v1.hasRegularBinning() && v2.hasRegularBinning()) {
        const std::vector<double>& binEdges1 = v1.binEdges();
        return ROOT::RDF::TH2DModel("", "", binEdges1.size() -1, binEdges1.data(), v2.axisNbins(), v2.axisMin(), v2.axisMax());
    }
    const std::vector<double>& binEdges1 = v1.binEdges();
    const std::vector<double>& binEdges2 = v2.binEdges();
    return ROOT::RDF::TH2DModel("", "", binEdges1.size() -1, binEdges1.data(), binEdges2.size() - 1, binEdges2.data());
}

std::unique_ptr<TH1D> Utils::copyHistoFromVariableHistos(const std::vector<VariableHisto>& histos,
                                                         const std::string& name) {

    auto itr = std::find_if(histos.begin(), histos.end(), [&name](const auto& element){return element.name() == name;});
    if (itr == histos.end()) {
        LOG(ERROR) << "Cannot find histogram: " << name << "\n";
        throw std::runtime_error("");
    }

    std::unique_ptr<TH1D> result(static_cast<TH1D*>(itr->histo()->Clone()));
    result->SetDirectory(nullptr);

    return result;
}

bool Utils::sampleHasUnfolding(const std::shared_ptr<Sample>& sample) {

    for (const auto& itruth : sample->truths()) {
        if (itruth->produceUnfolding()) return true;
    }

    return false;
}

std::vector<std::string> Utils::selectedFileList(const std::vector<std::string>& fileList,
                                                 const int split,
                                                 const int index) {

    if (index < 0) {
        LOG(ERROR) << "Index < 0, cannot proceed\n";
        throw std::invalid_argument("");
    }

    std::vector<std::string> result;
    for (std::size_t i = 0; i < fileList.size(); ++i) {
        static int totalIndex(0);
        if ((int)totalIndex % split == index) {
            LOG(DEBUG) << "Split N: " << split << ", index: " << index << ", adding file: " << fileList.at(i) << "\n";
            result.emplace_back(fileList.at(i));
        }
        ++totalIndex;
    }

    return result;
}