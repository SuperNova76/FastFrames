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

private:
  std::string m_name;

  std::string m_recoTreeName;

  std::vector<std::shared_ptr<Region> > m_regions;
  
  std::vector<std::shared_ptr<Systematic> > m_systematics;

  std::vector<UniqueSampleID> m_uniqueSampleIDs;
};
