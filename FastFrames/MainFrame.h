#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/HistoContainer.h"
#include "FastFrames/MetadataManager.h"
#include "FastFrames/SystematicReplacer.h"

#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>

class Variable;

class MainFrame {
public:
  explicit MainFrame() = default;

  virtual ~MainFrame() = default;

  virtual void setConfig(const std::shared_ptr<ConfigSetting>& config) {m_config = config;}

  virtual void init();

  virtual void executeHistograms();

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDF::RNode& node,
                                           const UniqueSampleID& /*sampleID*/) {return node;}

private:

  std::vector<SystematicHisto> processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID);

  std::string systematicFilter(/*const std::shared_ptr<Sample>& sample,*/
                               const std::shared_ptr<Systematic>& systematic,
                               const std::shared_ptr<Region>& region) const;
                                  
  std::string systematicVariable(const Variable& Variable,
                                 const std::shared_ptr<Systematic>& systematic) const;

  std::string systematicWeight(const std::shared_ptr<Systematic>& systematic) const;

  std::vector<std::vector<ROOT::RDF::RNode> > applyFilters(ROOT::RDF::RNode mainNode,
                                                           const std::shared_ptr<Sample>& sample) const;

  ROOT::RDF::RNode addWeightColumns(ROOT::RDF::RNode mainNode,
                                    const std::shared_ptr<Sample>& sample,
                                    const UniqueSampleID& id) const;

  ROOT::RDF::RNode addSingleWeightColumn(ROOT::RDF::RNode mainNode,
                                         const std::shared_ptr<Sample>& sample,
                                         const std::shared_ptr<Systematic>& systematic,
                                         const UniqueSampleID& id) const;

  std::vector<SystematicHisto> processHistograms(std::vector<std::vector<ROOT::RDF::RNode> >& filters,
                                                 const std::shared_ptr<Sample>& sample) const;

  void writeHistosToFile(const std::vector<SystematicHisto>& histos,
                         const std::shared_ptr<Sample>& sample) const;

protected:
  MetadataManager m_metadataManager;
  std::shared_ptr<ConfigSetting> m_config;
  SystematicReplacer m_systReplacer;


  ClassDef(MainFrame, 1);
};
