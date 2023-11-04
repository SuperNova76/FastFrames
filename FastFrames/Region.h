/**
 * @file Region.h
 * @brief Region
 *
 */

#pragma once

#include <FastFrames/Variable.h>

#include <string>
#include <vector>

/**
 * @brief Class containing information for a given region
 *
 */
class Region {
public:

  /**
   * @brief Construct a new Region object
   *
   * @param name Name of the region
   */
  explicit Region(const std::string& name) noexcept;

  /**
   * @brief Deleted default constructor
   *
   */
  Region()  = delete;

  /**
   * @brief Destroy the Region object
   *
   */
  ~Region() = default;

  /**
   * @brief Get the name of the region
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set selection for this region
   *
   * @param selection
   */
  inline void setSelection(const std::string& selection) {m_selection = selection;}

  /**
   * @brief Get the selection for this region
   *
   * @return const std::string&
   */
  inline const std::string& selection() const {return m_selection;}

  /**
   * @brief Add Variable to this regiom
   *
   * @param variable
   */
  inline void addVariable(const Variable& variable) {m_variables.emplace_back(variable);}

  /**
   * @brief Get all Variables for this region
   *
   * @return const std::vector<Variable>&
   */
  inline const std::vector<Variable>& variables() const {return m_variables;}

  /**
   * @brief Add variable combination for 2D histograms
   *
   * @param v1
   * @param v2
   */
  void addVariableCombination(const std::string& v1, const std::string& v2) {m_variableCombinations.emplace_back(std::make_pair(v1, v2));}

  /**
   * @brief Get variable combinations
   *
   * @return const std::vector<std::pair<std::string, std::string> >&
   */
  inline const std::vector<std::pair<std::string, std::string> >& variableCombinations() const {return m_variableCombinations;}

  /**
   * @brief Retrieve variable by its name
   *
   * @param name
   * @return const Variable&
   */
  const Variable& variableByName(const std::string& name) const;

private:

  std::string m_name;
  std::string m_selection;
  std::vector<Variable> m_variables;
  std::vector<std::pair<std::string, std::string> > m_variableCombinations;
};
