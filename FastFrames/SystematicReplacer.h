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

  static std::vector<std::string> getBranchesFromFile(const std::unique_ptr<TFile>& file,
                                                      const std::string& treeName);

  void matchSystematicVariables(const std::vector<std::string>& variables,
                                const std::vector<std::shared_ptr<Systematic> >& systematics);

  std::string replaceString(const std::string& original, const std::shared_ptr<Systematic>& systematic) const;

private:
  std::map<std::string, std::vector<std::string> > m_affectedBranches;
};
