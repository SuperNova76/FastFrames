#pragma once

#include "FastFrames/UniqueSampleID.h"

#include <string>
#include <vector>

class UniqueSample {
public:
  UniqueSample(const int dsid, const std::string& campaign, const std::string& simulation);

  ~UniqueSample() = default;  

  inline void setCrossSection(const double xsec) {m_crossSection = xsec;}

  inline double crossSection() const {return m_crossSection;}

private:
  UniqueSampleID m_id;
  double m_crossSection;
  std::vector<std::string> m_filePaths;
};
