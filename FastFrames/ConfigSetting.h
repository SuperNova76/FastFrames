#pragma once

#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"

#include <memory>
#include <string>
#include <vector>

class ConfigSetting {
public:
  explicit ConfigSetting();
  
  ~ConfigSetting() = default;

  const std::string& outputPath() const {return m_outputPath;}

  const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}
  
  const std::vector<std::shared_ptr<Sample> >& samples() const {return m_samples;}
  
  const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

private:
  std::string m_outputPath;
  std::vector<std::shared_ptr<Region> > m_regions;
  std::vector<std::shared_ptr<Sample> > m_samples;
  std::vector<std::shared_ptr<Systematic> > m_systematics;

};
