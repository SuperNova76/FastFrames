/**
 * @file MainFrame.h
 * @brief Main class responsible for histogramming and ntupling
 *
 */

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

/**
 * @brief Main class that does all the hard work
 *
 */
class MainFrame {
public:

  /**
   * @brief Construct a new Main Frame object
   *
   */
  explicit MainFrame() : m_config(std::make_shared<ConfigSetting>()) {};

  /**
   * @brief Destroy the Main Frame object
   *
   */
  virtual ~MainFrame() = default;

  /**
   * @brief Set the Config object
   *
   * @param config
   */
  virtual void setConfig(const std::shared_ptr<ConfigSetting>& config) {m_config = config;}

  /**
   * @brief Run all the steps needed at the beggining of the code
   *
   */
  virtual void init();

  /**
   * @brief Method to process histograms
   *
   */
  virtual void executeHistograms();

  /**
   * @brief Allows to define new observables
   * Users can override this and add their own variables
   *
   * @param node The input RDF node
   * @return ROOT::RDF::RNode the output node containg the new columns
   */
  virtual ROOT::RDF::RNode defineVariables(ROOT::RDF::RNode node,
                                           const UniqueSampleID& /*sampleID*/) {return node;}

  /**
   * @brief A helper method that make systematic copies of a provided nominal column
   * Name of the new varaible has to contain _NOSYS
   *
   * @tparam F
   * @param node Input node
   * @param newVariable Name of the new variable
   * @param defineFunction Actual function to be used for the variable definition
   * @param branches List of branch names that the function processes
   * @return ROOT::RDF::RNode Output node with the new columns
   */
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
      const std::vector<std::string> systBranches = m_systReplacer.replaceVector(branches, isystematic);
      node = node.Define(systName, defineFunction, systBranches);
    }

    // tell the replacer about the new columns
    m_systReplacer.addVariableAndEffectiveSystematics(newVariable, effectiveSystematics);

    return node;
  }

private:

  /**
   * @brief Process one UniqueSample (dsid, campaign, simulation type)
   * This stil ldoes not trigger the event loop as the histograms just contain the pointers.
   *
   * @param sample
   * @param uniqueSampleID
   * @return std::pair<std::vector<SystematicHisto>, ROOT::RDF::RNode> The histograms and the main RDF node for logging
   */
  std::pair<std::vector<SystematicHisto>, ROOT::RDF::RNode> processUniqueSample(const std::shared_ptr<Sample>& sample,
                                                                                const UniqueSampleID& uniqueSampleID);

  /**
   * @brief Get name of a filter after applying the systematic replacements
   *
   * @param sample Sample for which the replacement happens (needed for selection suffix)
   * @param systematic Systematic to be used for the replacement
   * @param region Region to be used for the replacement
   * @return std::string
   */
  std::string systematicFilter(const std::shared_ptr<Sample>& sample,
                               const std::shared_ptr<Systematic>& systematic,
                               const std::shared_ptr<Region>& region) const;

  /**
   * @brief Get name of a variable after applying the systematic replacements
   *
   * @param systematic Variable to be used for the replacement
   * @return std::string
   */
  std::string systematicVariable(const Variable& variable,
                                 const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief Get name of a systematic weight after applying the systematic replacements
   * The name is "weight_total_<SUFFIX>". If this does not exist for a given systematic,
   * uses "weigt_total_NOSYS"
   *
   * @param systematic Systematic to be used for the replacement
   * @return std::string
   */
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
                                    const UniqueSampleID& id);

  /**
   * @brief Add one systematic weight to the RDF nodes
   * The weight contains all event weights as well as normalisation
   * (luminosity * cross_section / sumWeights)
   * The new column is called "weight_total_<SYSTSUFFIX>"
   * Adds the new colum to the list of the available variables/columns
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
                                         const UniqueSampleID& id);

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
   * This triggers event loop in case of just one UniqueSampleID per Sample!
   *
   * @param histos histogram container
   * @param sample current sample
   * @param node Main RDF node - needed for printouts
   * @param printEventLoopCount Print event loop count? This is interesting only when just one UniqueSampleID was processed per Sample
   * @param
   */
  void writeHistosToFile(const std::vector<SystematicHisto>& histos,
                         const std::shared_ptr<Sample>& samplei,
                         const ROOT::RDF::RNode* node,
                         const bool printEventLoopCount) const;

  /**
   * @brief Add systematics from a file
   *
   * @param sample Sample to be added
   * @param isNominalOnly Flag to tell the code to only add nominal or all systematics
   */
  void readAutomaticSystematics(std::shared_ptr<Sample>& sample, const bool isNominalOnly) const;

  /**
   * @brief Read all systematics from a ROOT file (stored in listOfSystematics histogram)
   *
   * @param filePath Path to the ROOT file
   * @return std::vector<std::string>
   */
  std::vector<std::string> automaticSystematicNames(const std::string& filePath) const;

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
