/**
 * @file Sample.cc
 * @brief Sample
 *
 */

#include "FastFrames/Sample.h"

#include "FastFrames/Systematic.h"
#include "FastFrames/Region.h"

#include <algorithm>

Sample::Sample(const std::string& name) noexcept :
  m_name(name),
  m_recoTreeName("reco")
{
}

bool Sample::skipSystematicRegionCombination(const std::shared_ptr<Systematic>& syst,
                                             const std::shared_ptr<Region>& reg) const {

  auto itr = std::find_if(syst->regions().begin(), syst->regions().end(),
            [&reg](const auto& element){return reg->name() == element->name();});

  return itr == syst->regions().end();
}
