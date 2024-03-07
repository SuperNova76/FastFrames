#include "FastFrames/CutflowContainer.h"

#include "FastFrames/Logger.h"

#include <exception>

CutflowContainer::CutflowContainer(const std::string& name) noexcept :
m_name(name)
{
}

void CutflowContainer::copyValues(CutflowContainer& other) {
    m_titles = other.m_titles;
    for (auto& value : other.m_bookedYields) {
        m_yields.emplace_back(value.GetValue());
    }
    for (auto& value : other.m_bookedYieldErrors) {
        m_yieldErrors.emplace_back(value.GetValue());
    }
}

void CutflowContainer::mergeValues(CutflowContainer& other) {
    if (m_titles.size() != other.m_titles.size()) {
        LOG(ERROR) << "Incompatible size of titles\n";
        throw std::runtime_error("");
    }

    if (m_yields.size() != other.m_bookedYields.size()) {
        LOG(ERROR) << "Incompatible size of yields\n";
        throw std::runtime_error("");
    }

    if (m_yieldErrors.size() != other.m_bookedYieldErrors.size()) {
        LOG(ERROR) << "Incompatible size of errors\n";
        throw std::runtime_error("");
    }

    for (std::size_t i = 0; i < m_yields.size(); ++i) {
        m_yields.at(i)      += other.m_bookedYields.at(i).GetValue();
        m_yieldErrors.at(i) += other.m_bookedYieldErrors.at(i).GetValue();
    }
}

std::unique_ptr<TH1D> CutflowContainer::cutflowHisto() const {
    const int nbins = m_yields.size();
    std::unique_ptr<TH1D> h = std::make_unique<TH1D>("","", nbins, 0, nbins);

    for (int ibin = 1; ibin <= nbins; ++ibin) {
        h->SetBinContent(ibin, m_yields.at(ibin - 1));
        h->SetBinError  (ibin, std::sqrt(m_yieldErrors.at(ibin - 1)));
        h->GetXaxis()->SetBinLabel(ibin, m_titles.at(ibin - 1).c_str());
    }

    return h;
}