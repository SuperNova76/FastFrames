/**
 * @file UniqueSampleID.cc
 * @brief Sample identifier
 *
 */

#include "FastFrames/UniqueSampleID.h"

UniqueSampleID::UniqueSampleID(const int dsid, const std::string& campaign, const std::string& simulation) noexcept :
m_dsid(dsid),
m_campaign(campaign),
m_simulation(simulation)
{
}
