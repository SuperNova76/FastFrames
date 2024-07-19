/**
 * @file Truth.cc
 * @brief Truth information processing
 *
 */

#include "FastFrames/Truth.h"

#include "FastFrames/Logger.h"

#include <algorithm>
#include <exception>

Truth::Truth(const std::string& name) noexcept :
m_name(name),
m_truthTreeName(""),
m_selection(""),
m_eventWeight(""),
m_produceUnfolding(false),
m_matchRecoTruth(false)
{
}

const Variable& Truth::variableByName(const std::string& name) const {
    auto itr = std::find_if(m_variables.begin(), m_variables.end(), [&name](const auto& element){return element.name() == name;});
    if (itr == m_variables.end()) {
        LOG(ERROR) << "Unknown variable: " << name << "\n";
        throw std::invalid_argument("");
    }

    return *itr;
}

Variable& Truth::variableByName(const std::string& name) {
    auto itr = std::find_if(m_variables.begin(), m_variables.end(), [&name](const auto& element){return element.name() == name;});
    if (itr == m_variables.end()) {
        LOG(ERROR) << "Unknown variable: " << name << "\n";
        throw std::invalid_argument("");
    }

    return *itr;
}
