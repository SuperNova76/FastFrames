#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/MetadataManager.h"

#include <memory>

class MainFrame {
public:
  explicit MainFrame(const std::shared_ptr<ConfigSetting>& config);

  virtual ~MainFrame() = default;

  virtual void init();

  virtual void execute();

private:

  void processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID) const;

  MetadataManager m_metadataManager;
  std::shared_ptr<ConfigSetting> m_config;
};
