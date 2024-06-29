/**
 * @file Ntuple.cc
 * @brief Class storing all option for ntupling
 *
 */

#include "FastFrames/Ntuple.h"
#include "FastFrames/Utils.h"

#include <regex>

Ntuple::Ntuple() noexcept {
}

std::vector<std::string> Ntuple::listOfSelectedBranches(const std::vector<std::string>& allBranches) const {

    return Utils::selectedNotExcludedElements(allBranches, m_branches, m_excludedBrances);
}