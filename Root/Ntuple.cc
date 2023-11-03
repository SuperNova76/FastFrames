/**
 * @file Ntuple.cc
 * @brief Class storing all option for ntupling
 *
 */

#include "FastFrames/Ntuple.h"

#include <regex>

Ntuple::Ntuple() noexcept {
}

std::vector<std::string> Ntuple::listOfSelectedBranches(const std::vector<std::string>& allBranches) const {
    std::vector<std::string> result;

    if (m_branches.empty() && m_excludedBrances.empty()) {
        return allBranches;
    }

    for (const auto& ibranch : allBranches) {
        bool selected(false);
        for (const auto& imatch : m_branches) {
            std::regex match(imatch);
            if (std::regex_match(ibranch, match)) {
                selected = true;
                break;
            }
        }

        // if not selected, skip
        if (!selected) continue;

        // if selected check if it is not excluded
        for (const auto& imatch : m_excludedBrances) {
            std::regex match(imatch);
            if (std::regex_match(ibranch, match)) {
                selected = false;
                break;
            }
        }

        if (selected) {
            result.emplace_back(ibranch);
        }
    }

    return result;
}