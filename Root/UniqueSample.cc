#include "FastFrames/UniqueSample.h"

UniqueSample::UniqueSample(const int dsid, const std::string& campaign, const std::string& simulation) :
m_id(UniqueSampleID(dsid, campaign, simulation))
{
}
