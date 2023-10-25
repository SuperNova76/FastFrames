#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/MetadataManager.h"
#include "FastFrames/SystematicReplacer.h"

#include "ROOT/RDataFrame.hxx"

#include <memory>

class MainFrame {
public:
  explicit MainFrame(const std::shared_ptr<ConfigSetting>& config);

  virtual ~MainFrame() = default;

  virtual void init();

  virtual void execute();

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDataFrame& df) {return df;}

private:

  void processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID);

  ROOT::RDF::RNode filterSystRegion(ROOT::RDF::RNode& node,
                                    /*const std::shared_ptr<Sample>& sample,*/
                                    const std::shared_ptr<Systematic>& systematic,
                                    const std::shared_ptr<Region>& region);

  MetadataManager m_metadataManager;
  std::shared_ptr<ConfigSetting> m_config;
  SystematicReplacer m_systReplacer;
};
