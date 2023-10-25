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
    std::unique_ptr<TFile> file(TFile::Open(path.c_str(), "read"));
    if (!file) {
        LOG(ERROR) << "Cannot ROOT file at: " << path << "\n";
        throw std::invalid_argument("");
    }

    const std::vector<std::string> branches = this->getBranchesFromFile(file, treeName);
    this->matchSystematicVariables(branches, systematics);

    file->Close();
} 

std::vector<std::string> SystematicReplacer::getBranchesFromFile(const std::unique_ptr<TFile>& file,
                                                                 const std::string& treeName) {

    TTree* tree = file->Get<TTree>(treeName.c_str());
    if (!tree) {
        LOG(ERROR) << "Cannot read TTree: " << treeName << "\n";
        throw std::invalid_argument("");
    }

    std::vector<std::string> result;

    const auto branchList = tree->GetListOfBranches();
    std::size_t branchSize = tree->GetNbranches();
    for (std::size_t ibranch = 0; ibranch < branchSize; ++ibranch) {
        const std::string name = branchList->At(ibranch)->GetName();
        result.emplace_back(std::move(name));
    }

    return result;
}

void SystematicReplacer::matchSystematicVariables(const std::vector<std::string>& variables,
                                                  const std::vector<std::shared_ptr<Systematic> >& systematics) {

    for (const auto& isyst : systematics) {
        const std::string& systName = isyst->name();
        std::vector<std::string> affectedBranches;
        for (const auto& ivariable : variables) {

            // the systematic substring is not found in the branch name
            if (ivariable.find(systName) == std::string::npos) continue;
            
            // if it is found, we need to replace the systematic suffix with "NOSYS:
            const std::string nominalBranch = Utils::replaceString(ivariable, systName, "NOSYS");
            affectedBranches.emplace_back(std::move(nominalBranch));
        }
        m_affectedBranches.insert({systName, affectedBranches});
    }
}