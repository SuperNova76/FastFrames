/**
 * @file Cutflow.h
 * @brief Cutflow class header
 *
 */

#pragma once

#include <string>
#include <vector>

/**
 * @brief Class holding cutflow configuration
 *
 */
class Cutflow {
public:

  /**
   * @brief Construct a new Cutflow object
   *
   * @param name
   */
  explicit Cutflow(const std::string& name) noexcept;

  /**
   * @brief Construct a new Cutflow object
   *
   */
  Cutflow() = delete;

  /**
   * @brief Destroy the Cutflow object
   *
   */
  ~Cutflow() = default;

  /**
   * @brief Get the name of the cutflow
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Add a selection to the cutflow
   *
   * @param selection
   * @param title
   */
  inline void addSelection(const std::string& selection, const std::string& title) {m_selections.emplace_back(std::make_pair(selection, title));}

  /**
   * @brief Get the selections
   *
   * @return const std::vector<std::pair<std::string, std::string> >&
   */
  inline const std::vector<std::pair<std::string, std::string> >& selections() const {return m_selections;}

private:

  std::string m_name;
  std::vector<std::pair<std::string, std::string> > m_selections;
};
