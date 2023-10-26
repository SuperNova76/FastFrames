#pragma once

#include <memory>
#include <string>
#include <vector>

class Region;

class Systematic {
public:

  explicit Systematic(const std::string& name) noexcept;

  ~Systematic() = default;

  inline const std::string& name() const {return m_name;} 

  inline void setSumWeights(const std::string& sumWeights) {m_sumWeights = sumWeights;};

  inline const std::string& sumWeights() const {return m_sumWeights;}

  inline void addRegion(const std::shared_ptr<Region>& reg) {m_regions.emplace_back(reg);}

  inline const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}  

private:

  std::string m_name;
  std::string m_sumWeights;
  std::vector<std::shared_ptr<Region> > m_regions;
};
