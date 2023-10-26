#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/HistoContainer.h"
#include "FastFrames/MetadataManager.h"
#include "FastFrames/SystematicReplacer.h"

#include "ROOT/RDataFrame.hxx"

#include <memory>

class Variable;

class MainFrame {
public:
  explicit MainFrame(const std::shared_ptr<ConfigSetting>& config);

  virtual ~MainFrame() = default;

  virtual void init();

  virtual void executeHistograms();

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDataFrame& df) {return df;}

private:

  std::vector<SystematicHisto> processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID);

  std::string systematicFilter(/*const std::shared_ptr<Sample>& sample,*/
                               const std::shared_ptr<Systematic>& systematic,
                               const std::shared_ptr<Region>& region) const;
                                  
  std::string systematicVariable(const Variable& Variable,
                                 const std::shared_ptr<Systematic>& systematic) const;

  std::string systematicWeight(const std::shared_ptr<Systematic>& systematic) const;

  ROOT::RDF::RNode addWeightColumns(ROOT::RDF::RNode mainNode,
                                    const std::shared_ptr<Sample>& sample,
                                    const std::shared_ptr<Systematic>& systematic,
                                    const UniqueSampleID& id) const;

  void writeHistosToFile(const std::vector<SystematicHisto>& histos,
                         const std::shared_ptr<Sample>& sample) const;

  MetadataManager m_metadataManager;
  std::shared_ptr<ConfigSetting> m_config;
  SystematicReplacer m_systReplacer;
};
