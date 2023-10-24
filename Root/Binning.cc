#include "FastFrames/Binning.h"

Binning::Binning(const int min, const int max, const int nbins) :
m_min(min),
m_max(max),
m_nbins(nbins),
m_binEdges({})
{
}

Binning::Binning(const std::vector<double>& binEdges) :
m_min(0),
m_max(0),
m_nbins(0),
m_binEdges(binEdges)
{
}

Binning::Binning() :
m_min(0),
m_max(0),
m_nbins(0),
m_binEdges()
{
}
