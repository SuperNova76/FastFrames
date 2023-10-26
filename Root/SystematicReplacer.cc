#include "FastFrames/SystematicReplacer.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Utils.h"

#include "TFile.h"
#include "TTree.h"

#include <exception>

SystematicReplacer::SystematicReplacer() noexcept
{
}

void SystematicReplacer::readSystematicMapFromFile(const std::string& path,
                                                   const std::string& treeName,
                                                   const std::vector<std::shared_ptr<Systematic> >& systematics) {
    m_affectedBranches.clear();
    m_allBranches.clear();
    std::unique_ptr<TFile> file(TFile::Open(path.c_str(), "read"));
    if (!file) {
        LOG(ERROR) << "Cannot open ROOT file at: " << path << "\n";
        throw std::invalid_argument("");
    }

    this->getBranchesFromFile(file, treeName);
    this->matchSystematicVariables(m_allBranches, systematics);

    file->Close();
} 

void SystematicReplacer::getBranchesFromFile(const std::unique_ptr<TFile>& file,
                                             const std::string& treeName) {

    TTree* tree = file->Get<TTree>(treeName.c_str());
    if (!tree) {
        LOG(ERROR) << "Cannot read TTree: " << treeName << "\n";
        throw std::invalid_argument("");
    }

    const auto branchList = tree->GetListOfBranches();
    std::size_t branchSize = tree->GetNbranches();
    for (std::size_t ibranch = 0; ibranch < branchSize; ++ibranch) {
        const std::string name = branchList->At(ibranch)->GetName();
        m_allBranches.emplace_back(std::move(name));
    }
}

void SystematicReplacer::matchSystematicVariables(const std::vector<std::string>& variables,
                                                  const std::vector<std::shared_ptr<Systematic> >& systematics) {

    for (const auto& isyst : systematics) {
        const std::string& systName = isyst->name();
        std::vector<std::string> affectedBranches;
        for (const auto& ivariable : variables) {

            // the systematic substring should be the suffix
            if (!Utils::stringEndsWithString(ivariable, systName)) continue;
            
            // if it is found, we need to replace the systematic suffix with "NOSYS:
            const std::string nominalBranch = Utils::replaceString(ivariable, systName, "NOSYS");
            affectedBranches.emplace_back(std::move(nominalBranch));
        }
        m_affectedBranches.insert({systName, affectedBranches});
    }
}
  
std::string SystematicReplacer::replaceString(const std::string& original, const std::shared_ptr<Systematic>& systematic) const {
    auto itr = m_affectedBranches.find(systematic->name());
    if (itr == m_affectedBranches.end()) {
        LOG(ERROR) << "Cannot find systematic: " << systematic->name() << " in the systematic map. Please, fix!\n";
        throw std::invalid_argument("");
    }

    std::string result(original);

    // loop over all affected branches and replace all of them
    for (const std::string& ibranch : itr->second) {
        const std::string replacer = Utils::replaceString(ibranch, "NOSYS", systematic->name());
        result = Utils::replaceString(result, ibranch, replacer);
    }

    return result;
}