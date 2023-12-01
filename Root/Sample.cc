/**
 * @file Sample.cc
 * @brief Sample
 *
 */

#include "FastFrames/Sample.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Region.h"
#include "FastFrames/Systematic.h"

#include <algorithm>
#include <exception>

Sample::Sample(const std::string& name) noexcept :
  m_name(name),
  m_recoTreeName("reco"),
  m_reco_to_truth_pairing_indices({"eventNumber"})
{
}

bool Sample::skipSystematicRegionCombination(const std::shared_ptr<Systematic>& syst,
                                             const std::shared_ptr<Region>& reg) const {

  auto itr = std::find_if(syst->regions().begin(), syst->regions().end(),
            [&reg](const auto& element){return reg->name() == element->name();});

  return itr == syst->regions().end();
}

std::vector<std::string> Sample::uniqueTruthTreeNames() const {

  std::vector<std::string> result;

  for (const auto& itruth : m_truths) {
    const std::string& treeName = itruth->truthTreeName();
    auto itr = std::find(result.begin(), result.end(), treeName);
    if (itr == result.end()) {
      result.emplace_back(treeName);
    }
  }

  return result;
}

const std::shared_ptr<Systematic>& Sample::nominalSystematic() const {
  auto itr = std::find_if(m_systematics.begin(), m_systematics.end(), [](const auto& element){return element->isNominal();});
  if (itr == m_systematics.end()) {
    LOG(ERROR) << "Cannot find nominal systematic in the list of systematics for sample: " << m_name << "\n";
    throw std::runtime_error("");
  }

  return *itr;
}

void Sample::addVariable(const std::string& variable) {
  m_variables.push_back(variable);
};