/**
 * @file SystematicReplacer.h
 * @brief Management of the string replacements for systematics
 *
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Systematic;
class TFile;

/**
 * @brief Class responsible for all the string operations for systematic replacement
 *
 */
class SystematicReplacer {
public:

  /**
   * @brief Construct a new Systematic Replacer object
   *
   */
  explicit SystematicReplacer() noexcept;

  /**
   * @brief Destroy the Systematic Replacer object
   *
   */
  ~SystematicReplacer() = default;

  /**
   * @brief Reads ROOT file and creates two maps:
   * systematic | list of affected branches
   * branch | list of systematics affecting it
   *
   * @param path Path to the ROOT file
   * @param treeName Name of the TTree to read
   * @param systematics List of all systematics provided by the user
   */
  void readSystematicMapFromFile(const std::string& path,
                                 const std::string& treeName,
                                 const std::vector<std::shared_ptr<Systematic> >& systematics);

  /**
   * @brief Get all the branch names from a TTree in a ROOT file
   *
   * @param file ROOT file
   * @param treeName Name of TTree
   */
  void getBranchesFromFile(const std::unique_ptr<TFile>& file,
                           const std::string& treeName);

  /**
   * @brief Helper function that fills the maps
   *
   * @param variables List of all branches
   * @param systematics List of all systematics
   */
  void matchSystematicVariables(const std::vector<std::string>& variables,
                                const std::vector<std::shared_ptr<Systematic> >& systematics);

  /**
   * @brief Replace all occurances in a string for given systematic object
   *
   * @param original Original (nominal) string
   * @param systematic
   * @return std::string
   */
  std::string replaceString(const std::string& original, const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief Replace all occurances in a string for given systematic name
   *
   * @param original Original (nominal) string
   * @param systematicName
   * @return std::string
   */
  std::string replaceString(const std::string& original, const std::string& systematicName) const;

  /**
   * @brief Class replaceString on all elements of a vector
   *
   * @param originalVector
   * @param systematicName
   * @return std::vector<std::string>
   */
  std::vector<std::string> replaceVector(const std::vector<std::string>& originalVector, const std::string& systematicName) const;

  /**
   * @brief Add a branch to the list of available branches
   *
   * @param name Name of the branch
   */
  void addBranch(const std::string& name) {m_allBranches.emplace_back(name);}

  /**
   * @brief Get list of all branches
   *
   * @return const std::vector<std::string>&
   */
  inline const std::vector<std::string>& allBranches() const {return m_allBranches;}

  /**
   * @brief Get lit of all branches that do not have systematic variations
   *
   * @return std::vector<std::string>
   */
  std::vector<std::string> nominalBranches() const;

  /**
   * @brief Check if a branch exists
   *
   * @param branch
   * @return true
   * @return false
   */
  bool branchExists(const std::string& branch) const;

  /**
   * @brief Get the list of all systematic names that have effect on the list of provide columns
   * The provided systematics must impact at least one of the columns
   *
   * @param columns
   * @return std::vector<std::string>
   */
  std::vector<std::string> getListOfEffectiveSystematics(const std::vector<std::string>& columns) const;

  /**
   * @brief Add a new variable (column) to the map with the systematics that impact it
   *
   * @param variable
   * @param systematics
   */
  void addVariableAndEffectiveSystematics(const std::string& variable, const std::vector<std::string>& systematics);

  /**
   * @brief Update the systematics that affect a variable
   * NOTE: adds the variable to the map if it does not already exist
   *
   * @param variable
   * @param systematics
   */
  void updateVariableAndEffectiveSystematics(const std::string& variable, const std::vector<std::string>& systematics);


  /**
   * @brief Print the contents of the maps for systematic matching
   *
   */
  void printMaps() const;

  /**
   * @brief From an input string, find which (nominal) variables are present that are affected by systematics
   *
   * @param input
   * @return std::vector<std::string>
   */
  std::vector<std::string> listOfVariablesAffected(const std::string& input) const;

  /**
   * @brief is the given branch a systematic or affected by systematics?
   *
   * @param branch
   * @return true
   * @return false
   */
  bool isNominalBranch(const std::string& branch) const;

  /**
   * @brief Check if a given branch is directly a systematic variation
   *
   * @param branch
   * @return true
   * @return false
   */
  bool isSystematicVariation(const std::string& branch) const;

  /**
   * @brief Set the Systematic Names object
   *
   * @param systematics
   */
  void setSystematicNames(const std::vector<std::string>& systematics) {m_systematics = systematics;}

private:
  /**
   * @brief map where the key is the name of the systematic
   * and the value is the list of branches affected by the systematic
   *
   */
  std::map<std::string, std::vector<std::string> > m_systImpactsBranches;

  /**
   * @brief map where the key is the branch and the value
   * is the list of systematics that affect it
   *
   */
  std::map<std::string, std::vector<std::string> > m_branchesAffectedBySyst;

  /**
   * @brief list of all branches
   *
   */
  std::vector<std::string> m_allBranches;

  /**
   * @brief names of all systematics
   *
   */
  std::vector<std::string> m_systematics;
};
