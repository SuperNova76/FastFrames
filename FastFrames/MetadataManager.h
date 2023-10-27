#pragma once

#include "FastFrames/UniqueSampleID.h"
#include "FastFrames/Metadata.h"

#include <map>
#include <memory>
#include <string>

class Systematic;

class MetadataManager {
public:
  explicit MetadataManager() noexcept;
  ~MetadataManager() = default;

  void readFileList(const std::string& path);

  void readSumWeights(const std::string& path);

  void addLuminosity(const std::string& campaign, const double lumi);

  double sumWeights(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const;

  double luminosity(const std::string& campaign) const;

  double crossSection(const UniqueSampleID& id) const;

  double normalisation(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const;

  const std::vector<std::string>& filePaths(const UniqueSampleID& id) const;

private:

  std::map<UniqueSampleID, Metadata> m_metadata;
  std::map<std::string, double> m_luminosity;
};
