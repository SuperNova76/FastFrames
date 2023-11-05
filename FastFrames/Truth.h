/**
 * @file Truth.h
 * @brief Truth information processing
 *
 */

#pragma once

#include "FastFrames/Variable.h"

#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class responsible for truth information
 *
 */
class Truth {
public:

  /**
   * @brief Construct a new Truth object
   *
   * @param name
   */
  explicit Truth(const std::string& name) noexcept;

  /**
   * @brief Delete default constructor
   *
   */
  Truth() = delete;

  /**
   * @brief Destroy the Truth object
   *
   */
  ~Truth() = default;

  /**
   * @brief Get name
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set truth tree name
   *
   * @param treeName
   */
  inline void setTruthTreeName(const std::string& treeName) {m_truthTreeName = treeName;}

  /**
   * @brief Get truth tree name
   *
   * @return const std::string&
   */
  inline const std::string& truthTreeName() const {return m_truthTreeName;}

  /**
   * @brief Set selection
   *
   * @param selection
   */
  inline void setSelection(const std::string& selection) {m_selection = selection;}

  /**
   * @brief Get selection
   *
   * @return const std::string&
   */
  inline const std::string& selection() const {return m_selection;}

  /**
   * @brief Set event weight
   *
   * @param weight
   */
  inline void setEventWeight(const std::string& weight) {m_eventWeight = weight;}

  /**
   * @brief Get event weight
   *
   * @return const std::string&
   */
  inline const std::string& eventWeight() const {return m_eventWeight;}

  /**
   * @brief Add pair of variables for reco and truth 2D histograms
   *
   * @param reco Reco variable name
   * @param truth Truth variable name
   */
  inline void addMatchVariables(const std::string& reco, const std::string& truth) {m_matchedVariables.emplace_back(std::make_pair(reco, truth));}

  /**
   * @brief Get matched variables
   *
   * @return const std::vector<std::pair<std::string, std::string> >&
   */
  inline const std::vector<std::pair<std::string, std::string> >& matchedVariables() const {return m_matchedVariables;}

  /**
   * @brief Add truth variable
   *
   * @param variable
   */
  inline void addVariable(const Variable& variable) {m_variables.emplace_back(variable);}

  /**
   * @brief Get truth variables
   *
   * @return const Variable&
   */
  inline const std::vector<Variable>& variables() const {return m_variables;}

  /**
   * @brief Tell the code to produce unfolding histograms
   *
   * @param flag
   */
  inline void setProduceUnfolding(const bool flag) {m_produceUnfolding = flag;}

  /**
   * @brief Produce unfolding histograms?
   *
   * @return true
   * @return false
   */
  inline bool produceUnfolding() const {return m_produceUnfolding;}

private:

  std::string m_name;
  std::string m_truthTreeName;
  std::string m_selection;
  std::string m_eventWeight;
  std::vector<std::pair<std::string, std::string> > m_matchedVariables;
  std::vector<Variable> m_variables;
  bool m_produceUnfolding;
};
