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
#include <regex>

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
  
bool Sample::hasSystematic(const std::string& systematicName) const {
  auto itr = std::find_if(m_systematics.begin(), m_systematics.end(), [&systematicName](const auto& element){return element->name() == systematicName;});

  return itr != m_systematics.end();
}
  
bool Sample::skipExcludedSystematic(const std::string& systematicName) const {
    bool skip(false);
    for (const auto& iexclude : this->m_excludeAutomaticSystematics) {
        std::regex match(iexclude);
        if (std::regex_match(systematicName, match)) {
            skip = true;
            break;
        }
    }

    return skip;
}
  
bool Sample::matchTruthTree(const std::string& treeName) const {
    for (const auto& itruth : m_truths) {
        if (itruth->truthTreeName() != treeName) continue;
        if (!itruth->matchRecoTruth()) continue;

        // this truth matches the tree name and uses truth
        return true;
    }

    return false;
}

bool Sample::hasUnfolding() const {
    for (const auto& itruth : m_truths) {
        if (itruth->produceUnfolding()) return true;
    }

    return false;
}

bool Sample::isData() const {
  for (const auto& id : m_uniqueSampleIDs) {
    if (id.isData()) return true;
  }

  return false;
}