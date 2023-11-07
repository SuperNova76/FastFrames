/**
 * @file Ntuple.h
 * @brief Class storing all option for ntupling
 *
 */

#pragma once

#include "FastFrames/Sample.h"

#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class storing all option for ntupling
 *
 */
class Ntuple {
public:

  /**
   * @brief Construct a new Ntuple object
   *
   */
  explicit Ntuple() noexcept;

  /**
   * @brief Destroy the Ntuple object
   *
   */
  ~Ntuple() = default;

  /**
   * @brief Add one sample
   *
   * @param sample
   */
  void addSample(const std::shared_ptr<Sample>& sample) {m_samples.emplace_back(sample);}

  /**
   * @brief Return samples
   *
   * @return const std::vector<std::shared_ptr<Sample> >&
   */
  const std::vector<std::shared_ptr<Sample> >& samples() const {return m_samples;}

  /**
   * @brief Return samples (non const)
   *
   * @return std::vector<std::shared_ptr<Sample> >&
   */
  std::vector<std::shared_ptr<Sample> >& samples() {return m_samples;}

  /**
   * @brief Set selection
   *
   * @param selection
   */
  void setSelection(const std::string& selection) {m_selection = selection;}

  /**
   * @brief Get selection
   *
   * @return const std::string&
   */
  const std::string& selection() const {return m_selection;}

  /**
   * @brief Add a branch
   *
   * @param branch
   */
  void addBranch(const std::string& branch) {m_branches.emplace_back(branch);}

  /**
   * @brief Get branches
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& branches() const {return m_branches;}

  /**
   * @brief Add an excluded branch
   *
   * @param branch
   */
  void addExcludedBranch(const std::string& branch) {m_excludedBrances.emplace_back(branch);}

  /**
   * @brief Get excluded branches
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& excludedBranches() const {return m_excludedBrances;}

  /**
   * @brief Get the list of selected branches based on regex from branches and excluded branches
   *
   * @param allBranches all branches
   * @return std::vector<std::string>
   */
  std::vector<std::string> listOfSelectedBranches(const std::vector<std::string>& allBranches) const;

  /**
   * @brief Add name of the tree to be copied
   *
   * @param treeName
   */
  inline void addCopyTree(const std::string& treeName) {m_copyTrees.emplace_back(treeName);}

  /**
   * @brief Get the list of trees to copy
   *
   * @return const std::vector<std::string>&
   */
  inline const std::vector<std::string>& copyTrees() const {return m_copyTrees;}

private:

  std::vector<std::shared_ptr<Sample> > m_samples;

  std::string m_selection;

  std::vector<std::string> m_branches;

  std::vector<std::string> m_excludedBrances;

  std::vector<std::string> m_copyTrees;
};
