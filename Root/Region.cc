/**
 * @file Region.cc
 * @brief Region
 *
 */

#include "FastFrames/Region.h"

#include "FastFrames/Logger.h"

#include <algorithm>
#include <exception>

Region::Region(const std::string& name) noexcept :
m_name(name)
{
}

const Variable& Region::variableByName(const std::string& name) const {
    auto itr = std::find_if(m_variables.begin(), m_variables.end(), [&name](const auto& element){return element.name() == name;});

    if (itr == m_variables.end()) {
        LOG(ERROR) << "Cannot find variable: " << name << " for region: " << m_name << "\n";
        throw std::invalid_argument("");
    }

    return *itr;
}