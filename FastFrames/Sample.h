#pragma once

#include <memory>
#include <string>
#include <vector>

class Region;
class Systematic;

class Sample {
public:

  explicit Sample(const std::string& name) noexcept;
  ~Sample() = default;

  const std::string& name() const {return m_name;}

private:
  std::string m_name;

  std::vector<std::shared_ptr<Region> > m_regions;
  
  std::vector<std::shared_ptr<Systematic> > m_systematics;
};
