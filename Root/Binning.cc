#include "FastFrames/Binning.h"

Binning::Binning(const double min, const double max, const int nbins) noexcept :
m_min(min),
m_max(max),
m_nbins(nbins),
m_binEdges({}),
m_hasRegularBinning(true)
{
}

Binning::Binning(const std::vector<double>& binEdges) noexcept :
m_min(0),
m_max(0),
m_nbins(0),
m_binEdges(binEdges),
m_hasRegularBinning(true)
{
}

Binning::Binning() noexcept :
m_min(0),
m_max(0),
m_nbins(0),
m_binEdges(),
m_hasRegularBinning(true)
{
}
