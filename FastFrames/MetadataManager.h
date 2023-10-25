#pragma once

#include "FastFrames/UniqueSampleID.h"
#include "FastFrames/Metadata.h"

#include <map>
#include <string>

class MetadataManager {
public:
  explicit MetadataManager();

  ~MetadataManager() = default;

  void readFileList(const std::string& path);

  void readSumWeights(const std::string& path);

  void addLuminosity(const std::string& campaign, const double lumi);

  double sumWeights(const UniqueSampleID& id, const std::string& systematic) const;

  double luminosity(const std::string& campaign) const;

  double crossSection(const UniqueSampleID& id) const;

  double normalisation(const UniqueSampleID& id, const std::string& systematic) const;

private:

  std::map<UniqueSampleID, Metadata> m_metadata;
  std::map<std::string, double> m_luminosity;
};
