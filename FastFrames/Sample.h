#pragma once

#include "FastFrames/UniqueSampleID.h"

#include <memory>
#include <string>
#include <vector>

class Region;
class Systematic;

class Sample {
public:

  explicit Sample(const std::string& name) noexcept;
  ~Sample() = default;

  inline const std::string& name() const {return m_name;}

  inline const std::string& recoTreeName() const {return m_recoTreeName;}

  inline const std::vector<UniqueSampleID>& uniqueSampleIDs() const {return m_uniqueSampleIDs;}

  inline void addUniqueSampleID(const UniqueSampleID& id) {m_uniqueSampleIDs.emplace_back(id);}

  inline void addSystematic(const std::shared_ptr<Systematic>& syst) {m_systematics.emplace_back(syst);}

  inline const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

  inline void addRegion(const std::shared_ptr<Region>& region) {m_regions.emplace_back(region);}

  inline const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

  inline void setEventWeight(const std::string& weight) {m_eventWeight = weight;}

  inline const std::string& weight() const {return m_eventWeight;} 

private:
  std::string m_name;

  std::string m_recoTreeName;

  std::vector<std::shared_ptr<Region> > m_regions;
  
  std::vector<std::shared_ptr<Systematic> > m_systematics;

  std::vector<UniqueSampleID> m_uniqueSampleIDs;

  std::string m_eventWeight;
};
