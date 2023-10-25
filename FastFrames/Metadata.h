#pragma once

#include <string>
#include <map>
#include <vector>

class Metadata {
public:
  explicit Metadata();
  
  ~Metadata() = default;

  inline void setCrossSection(const double xSec) {m_crossSection = xSec;}

  inline double crossSection() const {return m_crossSection;}

  void addSumWeights(const std::string& name, const double value);

  double sumWeight(const std::string& name) const;

  void addFilePath(const std::string& path);

  inline const std::vector<std::string>& filePaths() {return m_filePaths;}

private:
  double m_crossSection;
  std::map<std::string, double> m_sumWeights;
  std::vector<std::string> m_filePaths;
};
