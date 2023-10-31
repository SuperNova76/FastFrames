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

private:

  std::string m_name;
  std::string m_selection;
  std::vector<Variable> m_variables;
};
