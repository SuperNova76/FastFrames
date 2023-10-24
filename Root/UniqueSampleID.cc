#include "FastFrames/UniqueSampleID.h"

UniqueSampleID::UniqueSampleID(const int dsid, const std::string& campaign, const std::string& simulation) :
m_dsid(dsid),
m_campaign(campaign),
m_simulation(simulation)
{
}
