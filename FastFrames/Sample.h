/**
 * @file Sample.h
 * @brief Sample
 *
 */

#pragma once

#include "FastFrames/Truth.h"
#include "FastFrames/UniqueSampleID.h"

#include <memory>
#include <string>
#include <vector>

class Region;
class Systematic;

/**
 * @brief Class containing all information for a given Sample
 *
 */
class Sample {
public:

  /**
   * @brief Construct a new Sample object
   *
   * @param name Name of the sample
   */
  explicit Sample(const std::string& name) noexcept;

  /**
   * @brief Destroy the Sample object
   *
   */
  ~Sample() = default;

  /**
   * @brief Get the name of the Sample
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set the Reco Tree Name object
   *
   * @param treeName
   */
  inline void setRecoTreeName(const std::string& treeName) {m_recoTreeName = treeName;}

  /**
   * @brief Get the name of the reco tree
   *
   * @return const std::string&
   */
  inline const std::string& recoTreeName() const {return m_recoTreeName;}

  /**
   * @brief Set the selectionSuffix
   *
   * @param selectionSuffix
   */
  inline void setSelectionSuffix(const std::string& selectionSuffix) {m_selectionSuffix = selectionSuffix;};

  /**
   * @brief Get the selectionSuffix
   *
   * @return const std::string&
   */
  inline const std::string& selectionSuffix() const {return m_selectionSuffix;};

  /**
   * @brief Get all UniqueSampleIDs for this Sample
   *
   * @return const std::vector<UniqueSampleID>&
   */
  inline const std::vector<UniqueSampleID>& uniqueSampleIDs() const {return m_uniqueSampleIDs;}

  /**
   * @brief Add UniqueSampleID to this Sample
   *
   * @param id
   */
  inline void addUniqueSampleID(const UniqueSampleID& id) {m_uniqueSampleIDs.emplace_back(id);}

  /**
   * @brief Add Systemtic to this sample
   *
   * @param syst
   */
  inline void addSystematic(const std::shared_ptr<Systematic>& syst) {m_systematics.emplace_back(syst);}

  /**
   * @brief Get list of all Systematics
   *
   * @return const std::vector<std::shared_ptr<Systematic> >&
   */
  inline const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

  /**
   * @brief Add Region to this sample
   *
   * @param region
   */
  inline void addRegion(const std::shared_ptr<Region>& region) {m_regions.emplace_back(region);}

  /**
   * @brief Get all regions for this sample
   *
   * @return const std::vector<std::shared_ptr<Region> >&
   */
  inline const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

  /**
   * @brief Set vector of indices used to pair reco level and truth level trees.
   *
   * @param reco_to_truth_pairing_indices
   */
  void setRecoToTruthPairingIndices(const std::vector<std::string>& reco_to_truth_pairing_indices) {m_reco_to_truth_pairing_indices = reco_to_truth_pairing_indices;};

  /**
   * @brief Get vector of indices used to pair reco level and truth level trees.
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& recoToTruthPairingIndices() const {return m_reco_to_truth_pairing_indices;};

  /**
   * @brief Set the event weight
   *
   * @param weight
   */
  inline void setEventWeight(const std::string& weight) {m_eventWeight = weight;}

  /**
   * @brief Get the event weight
   *
   * @return const std::string&
   */
  inline const std::string& weight() const {return m_eventWeight;}

  /**
   * @brief Skip setup where a given region and a systematic are not set
   *
   * @param syst Systematic
   * @param reg Region
   * @return true
   * @return false
   */
  bool skipSystematicRegionCombination(const std::shared_ptr<Systematic>& syst,
                                       const std::shared_ptr<Region>& reg) const;

  /**
   * @brief Remove all systematics
   *
   */
  inline void clearSystematics() {m_systematics.clear();}

  /**
   * @brief Add Truth object
   *
   * @param truth
   */
  inline void addTruth(const std::shared_ptr<Truth>& truth) {m_truths.emplace_back(truth);}

  /**
   * @brief Get Truth objects
   *
   * @return const std::vector<std::shared_ptr<Truth> >&
   */
  inline const std::vector<std::shared_ptr<Truth> >& truths() const {return m_truths;}

  /**
   * @brief Has truths defined?
   *
   * @return true
   * @return false
   */
  inline bool hasTruth() const {return !m_truths.empty();}

  /**
   * @brief Get the list of unqiue truth tree names for this sample
   *
   * @return std::vector<std::string>
   */
  std::vector<std::string> uniqueTruthTreeNames() const;

  /**
   * @brief Get nominal (i.e. name == "NOSYS") systematic
   *
   * @return const std::shared_ptr<Systematic>&
   */
  const std::shared_ptr<Systematic>& nominalSystematic() const;

  /**
   * @brief Add custom new column from the config
   *
   * @param newName name of the new column
   * @param formula the formula for the new column
   */
  inline void addCustomDefine(const std::string& newName,
                              const std::string& formula) {m_customDefines.emplace_back(std::make_pair(newName, formula));}

  /**
   * @brief Get custom defines
   *
   * @return const std::vector<std::pair<std::string, std::string> >&
   */
  inline const std::vector<std::pair<std::string, std::string> >& customDefines() const {return m_customDefines;}

  /**
   * @brief Add variable to the list of variables
   *
   * @param variable
   */
  inline void addVariable(const std::string& variable) {m_variables.emplace_back(variable);}

  /**
   * @brief Get the list of variables
   *
   * @return const std::vector<std::string>&
   */
  inline const std::vector<std::string>& variables() const {return m_variables;}

  /**
   * @brief Add regex for exclude systematics from automatic systematics
   * 
   * @param name 
   */
  inline void addExcludeAutomaticSystematic(const std::string& name) {m_excludeAutomaticSystematics.emplace_back(name);}

  /**
   * @brief Get the vector of regexes for automatic systematics exclusion 
   * 
   * @return const std::vector<std::string>& 
   */
  inline const std::vector<std::string>& excludeAutomaticSystematics() const {return m_excludeAutomaticSystematics;}

private:
  std::string m_name;

  std::string m_recoTreeName;

  std::string m_selectionSuffix;

  std::vector<std::shared_ptr<Region> > m_regions;

  std::vector<std::shared_ptr<Systematic> > m_systematics;

  std::vector<UniqueSampleID> m_uniqueSampleIDs;

  std::vector<std::shared_ptr<Truth> > m_truths;

  std::vector<std::string> m_reco_to_truth_pairing_indices;

  std::string m_eventWeight;

  std::vector<std::pair<std::string, std::string> > m_customDefines;

  std::vector<std::string> m_variables;

  std::vector<std::string> m_excludeAutomaticSystematics;
};
