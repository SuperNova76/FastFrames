/**
 * @file HistoContainer.cc
 * @brief File containing classes for hsitogram storage
 *
 */

#include "FastFrames/HistoContainer.h"

#include "FastFrames/Logger.h"

#include <exception>

void VariableHisto::mergeHisto(ROOT::RDF::RResultPtr<TH1D> h) {
    m_histo->Add(h.GetPtr());
}

void VariableHisto2D::mergeHisto(ROOT::RDF::RResultPtr<TH2D> h) {
    m_histo->Add(h.GetPtr());
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
