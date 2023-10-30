#include "FastFrames/SystematicReplacer.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Utils.h"

#include "TFile.h"
#include "TTree.h"

#include <algorithm>
#include <exception>

SystematicReplacer::SystematicReplacer() noexcept
{
}

void SystematicReplacer::readSystematicMapFromFile(const std::string& path,
                                                   const std::string& treeName,
                                                   const std::vector<std::shared_ptr<Systematic> >& systematics) {
    m_systImpactsBranches.clear();
    m_branchesAffectedBySyst.clear();
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
            affectedBranches.emplace_back(nominalBranch);

            // also do it the other way around
            auto itr = m_branchesAffectedBySyst.find(nominalBranch);
            if (itr == m_branchesAffectedBySyst.end()) {
                m_branchesAffectedBySyst.insert({nominalBranch, {systName}});
            } else {
                itr->second.emplace_back(systName);
            }
        }
        m_systImpactsBranches.insert({systName, affectedBranches});
    }
}
  
std::string SystematicReplacer::replaceString(const std::string& original, const std::shared_ptr<Systematic>& systematic) const {
    auto itr = m_systImpactsBranches.find(systematic->name());
    if (itr == m_systImpactsBranches.end()) {
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

bool SystematicReplacer::branchExists(const std::string& name) const {
    auto itr = std::find(m_allBranches.begin(), m_allBranches.end(), name);

    return itr != m_allBranches.end();
}

std::vector<std::string> SystematicReplacer::getListOfEffectiveSystematics(const std::vector<std::string>& variables) const {

    std::vector<std::string> result;

    for (const auto& ivariable : variables) {
        if (ivariable.find("NOSYS") == std::string::npos) {
            LOG(ERROR) << "Wrong name of a variable, it does not contain \"NOSYS\"\n";
            throw std::invalid_argument("");
        }

        // get the variables from the map and then take the unique ones
        auto itr = m_branchesAffectedBySyst.find(ivariable);
        if (itr == m_branchesAffectedBySyst.end()) {
            LOG(ERROR) << "Cannot find branch: " << ivariable << ", in the branch map\n";
            throw std::invalid_argument("");
        }

        // loop over all systematics and add the unique ones
        for (const auto& isyst : itr->second) {
            auto itrResult = std::find(result.begin(), result.end(), isyst);
            if (itrResult == result.end()) {
                result.emplace_back(isyst);
            }
        }
    }

    return result;
}
  
void SystematicReplacer::addVariableAndEffectiveSystematics(const std::string& variable, const std::vector<std::string>& systematics) {
    if (this->branchExists(variable)) {
        LOG(DEBUG) << "Variable " << variable << " already exists, not adding it\n";
        return;
    }

    if (variable.find("NOSYS") == std::string::npos) {
        LOG(ERROR) << "Variable " << variable << " does not contain \"NOSYS\"\n";
        throw std::invalid_argument("");
    }

    // add to the list of branches
    m_allBranches.emplace_back(variable);

    // add to the maps of systematics
    m_branchesAffectedBySyst.insert({variable, systematics});

    for (const auto& isystematic : systematics) {
        auto itr = m_systImpactsBranches.find(isystematic);
        if (itr == m_systImpactsBranches.end()) {
            LOG(ERROR) << "Unknown systematic: " << isystematic << "\n";
            throw std::invalid_argument("");
        }
        itr->second.emplace_back(variable);
    }
}

std::vector<std::string> SystematicReplacer::replaceVector(const std::vector<std::string>& input,
                                                           const std::string& systematic) const {

    std::vector<std::string> result;

    for (const auto& ivariable : input) {
        result.emplace_back(Utils::replaceString(ivariable, "NOSYS", systematic));
    }

    return result;
}