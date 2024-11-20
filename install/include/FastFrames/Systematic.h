/**
 * @file Systematic.h
 * @brief Systematic
 *
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

class Region;

/**
 * @brief Class containing all information for Systematic
 *
 */
class Systematic {
public:

  /**
   * @brief Construct a new Systematic object
   *
   * @param name Name of Systematic
   */
  explicit Systematic(const std::string& name) noexcept;

  /**
   * @brief Destroy the Systematic object
   *
   */
  ~Systematic() = default;

  /**
   * @brief Get the name of the Systematic
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set the nominal sum weights for this systematic
   *
   * @param sumWeights
   */
  inline void setSumWeights(const std::string& sumWeights) {m_sumWeights = sumWeights;};

  /**
   * @brief Get the nominal sum weights for this systematic
   *
   * @return const std::string&
   */
  inline const std::string& sumWeights() const {return m_sumWeights;}

  /**
   * @brief Set the weight suffix for this systematic
   *
   * @param weightSuffix
   */
  inline void setWeightSuffix(const std::string& weightSuffix) {m_weightSuffix = weightSuffix;};

  /**
   * @brief Get the weight suffix for this systematic
   *
   * @return const std::string&
   */
  inline const std::string& weightSuffix() const {return m_weightSuffix;}

  /**
   * @brief Add Region for this systematic
   *
   * @param reg
   */
  inline void addRegion(const std::shared_ptr<Region>& reg) {m_regions.emplace_back(reg);}

  /**
   * @brief Get all Regions for this systematic
   *
   * @return const std::vector<std::shared_ptr<Region> >&
   */
  inline const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

  /**
   * @brief Returns true if systematic is nominal, i.e. its name is "NOSYS"
   *
   * @return true
   * @return false
   */
  inline bool isNominal() const {return m_isNominal;}

private:

  std::string m_name;
  std::string m_sumWeights;
  std::string m_weightSuffix;
  std::vector<std::shared_ptr<Region> > m_regions;
  bool m_isNominal;
};
