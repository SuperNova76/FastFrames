/**
 * @file HistoContainer.cc
 * @brief File containing classes for hsitogram storage
 *
 */

#include "FastFrames/HistoContainer.h"

#include "FastFrames/Logger.h"

#include <exception>

void VariableHisto::mergeHisto(ROOT::RDF::RResultPtr<TH1D> h) {
    m_histoUniquePtr->Add(h.GetPtr());
}

void VariableHisto2D::mergeHisto(ROOT::RDF::RResultPtr<TH2D> h) {
    m_histoUniquePtr->Add(h.GetPtr());
}
  
void VariableHisto::copyHisto(ROOT::RDF::RResultPtr<TH1D> h) {
    m_histoUniquePtr.reset(static_cast<TH1D*>(h->Clone()));
}

void VariableHisto2D::copyHisto(ROOT::RDF::RResultPtr<TH2D> h) {
    m_histoUniquePtr.reset(static_cast<TH2D*>(h->Clone()));
}

void SystematicHisto::merge(const SystematicHisto& other) {
    if (m_name != other.name()) {
        LOG(ERROR) << "Something went wrong with the merging of the histograms\n";
        throw std::runtime_error("");
    }

    if (m_regions.size() != other.m_regions.size()) {
        LOG(ERROR) << "Sizes of the regions do not match!\n";
        throw std::runtime_error("");
    }

    for (std::size_t ireg = 0; ireg < m_regions.size(); ++ireg) {
        if (m_regions.at(ireg).variableHistos().size() != other.m_regions.at(ireg).variableHistos().size()) {
            LOG(ERROR) << "Sizes of the variables do not match!\n";
            throw std::runtime_error("");
        }
        if (m_regions.at(ireg).variableHistos2D().size() != other.m_regions.at(ireg).variableHistos2D().size()) {
            LOG(ERROR) << "Sizes of the 2D variables do not match!\n";
            throw std::runtime_error("");
        }

        // merge 1D histos
        for (std::size_t ivariable = 0; ivariable < m_regions.at(ireg).variableHistos().size(); ++ivariable) {
            m_regions.at(ireg).variableHistos().at(ivariable)
                     .mergeHisto(other.regionHistos().at(ireg).variableHistos().at(ivariable).histo());
        }

        // merge 2D histos
        for (std::size_t ivariable2D = 0; ivariable2D < m_regions.at(ireg).variableHistos2D().size(); ++ivariable2D) {
            m_regions.at(ireg).variableHistos2D().at(ivariable2D)
                     .mergeHisto(other.regionHistos().at(ireg).variableHistos2D().at(ivariable2D).histo());
        }
    }
}

SystematicHisto SystematicHisto::copy() const {
    SystematicHisto result(name());
    for (const auto& ireg : m_regions) {
        result.m_regions.emplace_back(RegionHisto(ireg.name()));
        for (const auto& ivariable : ireg.variableHistos()) {
            result.m_regions.back().variableHistos().emplace_back(ivariable.name());
            result.m_regions.back().variableHistos().back().copyHisto(ivariable.histo());
        }
        for (const auto& ivariable : ireg.variableHistos2D()) {
            result.m_regions.back().variableHistos2D().emplace_back(ivariable.name());
            result.m_regions.back().variableHistos2D().back().copyHisto(ivariable.histo());
        }
    }

    return result;
}