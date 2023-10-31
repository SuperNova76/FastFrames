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
   * @brief Set the nominal sum weights for this sample
   *
   * @param sumWeights
   */
  inline void setSumWeights(const std::string& sumWeights) {m_sumWeights = sumWeights;};

  /**
   * @brief Get the nominal sum weights for this sample
   *
   * @return const std::string&
   */
  inline const std::string& sumWeights() const {return m_sumWeights;}

  /**
   * @brief Add Region for this sample
   *
   * @param reg
   */
  inline void addRegion(const std::shared_ptr<Region>& reg) {m_regions.emplace_back(reg);}

  /**
   * @brief Get all Regions for this sample
   *
   * @return const std::vector<std::shared_ptr<Region> >&
   */
  inline const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

private:

  std::string m_name;
  std::string m_sumWeights;
  std::vector<std::shared_ptr<Region> > m_regions;
};
