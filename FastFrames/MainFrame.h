#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/HistoContainer.h"
#include "FastFrames/MetadataManager.h"
#include "FastFrames/StringOperations.h"
#include "FastFrames/SystematicReplacer.h"

#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>
#include <string>

class Variable;

class MainFrame {
public:
  explicit MainFrame() : m_config(std::make_shared<ConfigSetting>()) {};

  virtual ~MainFrame() = default;

  virtual void setConfig(const std::shared_ptr<ConfigSetting>& config) {m_config = config;}

  virtual void init();

  virtual void executeHistograms();

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDF::RNode& node,
                                           const UniqueSampleID& /*sampleID*/) {return node;}

  template<typename F>
  ROOT::RDF::RNode systematicDefine(ROOT::RDF::RNode node,
                                    const std::string& newVariable,
                                    F defineFunction,
                                    const std::vector<std::string>& branches) {

    if (newVariable.find("NOSYS") == std::string::npos) {
      LOG(ERROR) << "The new variable name does not contain \"NOSYS\"\n";
      throw std::invalid_argument("");
    }

    if (m_systReplacer.branchExists(newVariable)) {
      LOG(WARNING) << "Variable: " << newVariable << " is already in the input, ignoring\n";
      return node;
    }

    // first add the nominal define
    node = node.Define(newVariable, defineFunction, branches);

    // add systematics
    // get list of all systeamtics affecting the inputs
    std::vector<std::string> effectiveSystematics = m_systReplacer.getListOfEffectiveSystematics(branches);

    for (const auto& isystematic : effectiveSystematics) {
      if (isystematic == "NOSYS") continue;
      const std::string systName = StringOperations::replaceString(newVariable, "NOSYS", isystematic);
      const std::vector<std::string> systBranches = StringOperations::replaceVector(branches, "NOSYS", isystematic);
      node = node.Define(systName, defineFunction, systBranches);
    }

    // tell the replacer about the new columns
    m_systReplacer.addVariableAndEffectiveSystematics(newVariable, effectiveSystematics);

    return node;
  }

private:

  std::vector<SystematicHisto> processUniqueSample(const std::shared_ptr<Sample>& sample, const UniqueSampleID& uniqueSampleID);

  std::string systematicFilter(/*const std::shared_ptr<Sample>& sample,*/
                               const std::shared_ptr<Systematic>& systematic,
                               const std::shared_ptr<Region>& region) const;

  std::string systematicVariable(const Variable& Variable,
                                 const std::shared_ptr<Systematic>& systematic) const;

  std::string systematicWeight(const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief apply RDF filters (selections)
   *
   * @param mainNode current ROOT node
   * @param sample current Sample
   * @return std::vector<std::vector<ROOT::RDF::RNode> > Filter stored per region, per systematic
   */
  std::vector<std::vector<ROOT::RDF::RNode> > applyFilters(ROOT::RDF::RNode mainNode,
                                                           const std::shared_ptr<Sample>& sample) const;

  /**
   * @brief Add columns representing the systematic event weights
   *
   * @param mainNode current ROOT note
   * @param sample current sample
   * @param id current sample identificatotr
   * @return ROOT::RDF::RNode  node with weights added
   */
  ROOT::RDF::RNode addWeightColumns(ROOT::RDF::RNode mainNode,
                                    const std::shared_ptr<Sample>& sample,
                                    const UniqueSampleID& id) const;

  /**
   * @brief Add one systematic weight to the RDF nodes
   * The weight contains all event weights as well as normalisation
   * (luminosity * cross_section / sumWeights)
   * The new column is called "weight_total_<SYSTSUFFIX>"
   *
   * @param mainNode current ROOT node
   * @param sample current sample
   * @param systematic current systematic
   * @param id current sample identificator
   * @return ROOT::RDF::RNode node iwth added weight
   */
  ROOT::RDF::RNode addSingleWeightColumn(ROOT::RDF::RNode mainNode,
                                         const std::shared_ptr<Sample>& sample,
                                         const std::shared_ptr<Systematic>& systematic,
                                         const UniqueSampleID& id) const;

  /**
   * @brief Adds ROOT::Math::PtEtaPhiEVector for provided objects to RDF
   *
   * @param mainNode current ROOT node
   * @return ROOT::RDF::RNode node with the addedd vectors
   */
  ROOT::RDF::RNode addTLorentzVectors(ROOT::RDF::RNode mainNode);

  /**
   * @brief Adds a single ROOT::Math::PtEtaPhiEVector for provided object
   *
   * @param mainNode current ROOT node
   * @param object object name, e.g. "jet" or "el_tight"
   * @return ROOT::RDF::RNode node with the added vector for the object
   */
  ROOT::RDF::RNode addSingleTLorentzVector(ROOT::RDF::RNode mainNode,
                                           const std::string& object);

  /**
   * @brief Main code that calls the event loop
   *
   * @param filters List of nodes, each node represents per region, per systematic filter
   * @param sample current sample
   * @return std::vector<SystematicHisto> container of the histograms
   */
  std::vector<SystematicHisto> processHistograms(std::vector<std::vector<ROOT::RDF::RNode> >& filters,
                                                 const std::shared_ptr<Sample>& sample) const;

  /**
   * @brief Write histogram container to a ROOT file
   *
   * @param histos histogram container
   * @param sample current sample
   */
  void writeHistosToFile(const std::vector<SystematicHisto>& histos,
                         const std::shared_ptr<Sample>& sample) const;

protected:

  /**
   * @brief class responsible for managing metadata (lumminosity, cross-section, sumWeights, ...)
   *
   */
  MetadataManager m_metadataManager;

  /**
   * @brief holds all configuration settings
   *
   */
  std::shared_ptr<ConfigSetting> m_config;

  /**
   * @brief main tool to do the string operations for systematic variations
   *
   */
  SystematicReplacer m_systReplacer;


  /**
   * @brief Needed for ROOT to generate the dictionary
   *
   */
  ClassDef(MainFrame, 1);
};
