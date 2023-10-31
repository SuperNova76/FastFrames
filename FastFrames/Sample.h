#pragma once

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

private:
  std::string m_name;

  std::string m_recoTreeName;

  std::vector<std::shared_ptr<Region> > m_regions;

  std::vector<std::shared_ptr<Systematic> > m_systematics;

  std::vector<UniqueSampleID> m_uniqueSampleIDs;

  std::string m_eventWeight;
};
