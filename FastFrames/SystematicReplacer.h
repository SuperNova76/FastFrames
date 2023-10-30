#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Systematic;
class TFile;

class SystematicReplacer {
public:
  explicit SystematicReplacer() noexcept;

  ~SystematicReplacer() = default;

  void readSystematicMapFromFile(const std::string& path,
                                 const std::string& treeName,
                                 const std::vector<std::shared_ptr<Systematic> >& systematics);

  void getBranchesFromFile(const std::unique_ptr<TFile>& file,
                           const std::string& treeName);

  void matchSystematicVariables(const std::vector<std::string>& variables,
                                const std::vector<std::shared_ptr<Systematic> >& systematics);

  std::string replaceString(const std::string& original, const std::shared_ptr<Systematic>& systematic) const;

  std::string replaceString(const std::string& original, const std::string& systematicname) const;

  std::vector<std::string> replaceVector(const std::vector<std::string>& originalVector, const std::string& systematicname) const;

  inline const std::vector<std::string>& allBranches() const {return m_allBranches;}

  bool branchExists(const std::string& branch) const;

  std::vector<std::string> getListOfEffectiveSystematics(const std::vector<std::string>& variable) const;

  void addVariableAndEffectiveSystematics(const std::string& variable, const std::vector<std::string>& systematics);

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
};
