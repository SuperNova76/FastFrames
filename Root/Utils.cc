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

std::vector<double> fromRegularToEdges(const Variable& v){
    // We create a TH1D just as a proxy to get the bin edges
    TH1D histo("", "", v.axisNbins(), v.axisMin(), v.axisMax());
    std::vector<double> binEdges{};
    

    // Add each bin edge one by one
    std::size_t nBins = static_cast<std::size_t>(histo.GetNbinsX());
    for(std::size_t i = 1; i <= nBins; ++i) {
        binEdges.push_back(histo.GetXaxis()->GetBinLowEdge(i));
    }
    // Add the last bin edge
    binEdges.push_back(histo.GetXaxis()->GetBinUpEdge(nBins));

    return binEdges;
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

ROOT::RDF::TH3DModel Utils::histoModel3D(const Variable& v1, const Variable& v2, const Variable& v3) {
    const std::vector<double> binEdges1 = v1.hasRegularBinning() ? fromRegularToEdges(v1) : v1.binEdges();
    const std::vector<double> binEdges2 = v2.hasRegularBinning() ? fromRegularToEdges(v2) : v2.binEdges();
    const std::vector<double> binEdges3 = v3.hasRegularBinning() ? fromRegularToEdges(v3) : v3.binEdges();
    return ROOT::RDF::TH3DModel("", "", binEdges1.size() -1, binEdges1.data(), binEdges2.size() - 1, binEdges2.data(), binEdges3.size() - 1, binEdges3.data());
}

std::unique_ptr<TH1D> Utils::copyHistoFromVariableHistos(const std::vector<VariableHisto>& histos,
                                                         const std::string& name) {

    auto itr = std::find_if(histos.begin(), histos.end(), [&name](const auto& element){return element.name() == name;});
    if (itr == histos.end()) {
        LOG(ERROR) << "Cannot find histogram: " << name << "\n";
        throw std::runtime_error("");
    }

    std::unique_ptr<TH1D> result(static_cast<TH1D*>(itr->histoUniquePtr()->Clone()));
    result->SetDirectory(nullptr);

    return result;
}

std::unique_ptr<TH2D> Utils::copyHistoFromVariableHistos2D(const std::vector<VariableHisto2D>& histos,
                                                           const std::string& name) {

    auto itr = std::find_if(histos.begin(), histos.end(), [&name](const auto& element){return element.name() == name;});
    if (itr == histos.end()) {
        LOG(ERROR) << "Cannot find histogram: " << name << "\n";
        throw std::runtime_error("");
    }

    std::unique_ptr<TH2D> result(static_cast<TH2D*>(itr->histoUniquePtr()->Clone()));
    result->SetDirectory(nullptr);

    return result;
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

void Utils::capHisto0And1(TH1D* h, const std::string& name) {
    for (int ibin = 0; ibin <= h->GetNbinsX(); ++ibin) {
        const double content = h->GetBinContent(ibin);
        if (content < 0) {
            h->SetBinContent(ibin, 0.);
            LOG(VERBOSE) << "Negative bin content of " << content << " found in " << ibin << " bin of " << name << " histogram, setting it to 0\n";
        }
        if (content > 1.) {
            h->SetBinContent(ibin, 1.);
            LOG(VERBOSE) << "Bin content of " << content << " found in " << ibin << " bin of " << name << " histogram, setting it to 1\n";
        }
    }
}
  
const Variable& Utils::getVariableByName(const std::vector<std::shared_ptr<Region> >& regions,
                                         const std::string& regionName,
                                         const std::string& variableName) {

    // first find the region
    auto itrReg = std::find_if(regions.begin(), regions.end(), [&regionName](const auto& element){return element->name() == regionName;});
    if (itrReg == regions.end()) {
        LOG(ERROR) << "Cannot find region: " << regionName << "\n";
        throw std::runtime_error("");
    }

    auto itrVar = std::find_if((*itrReg)->variables().begin(), (*itrReg)->variables().end(),
                                [&variableName](const auto& element){return element.name() == variableName;});

    if (itrVar == (*itrReg)->variables().end()) {
        LOG(ERROR) << "Cannot find variable: " << variableName << " in region: " << regionName <<"\n";
        throw std::runtime_error("");
    }

    return *itrVar;
}