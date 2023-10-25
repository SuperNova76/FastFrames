#include "FastFrames/Metadata.h"

#include "FastFrames/Logger.h"

#include <algorithm>
#include <exception>

Metadata::Metadata() noexcept :
m_crossSection(-1),
m_sumWeights({})
{
}

void Metadata::addSumWeights(const std::string& name, const double value) {
    auto itr = m_sumWeights.find(name);
    if (itr == m_sumWeights.end()) {
        m_sumWeights.insert({name, value});
    } else {
        LOG(WARNING) << "name: " << name << " already found in the list of the sumweights, not adding it again\n";
    }
}

double Metadata::sumWeight(const std::string& name) const {
    auto itr = m_sumWeights.find(name);
    if (itr == m_sumWeights.end()) {
        LOG(ERROR) << "Cannot find name: " << name << ", in the list of the sumweights, cannot retrieve it. Please fix the code\n";
        throw std::invalid_argument("");
    } else {
        return itr->second;
    }
}

void Metadata::addFilePath(const std::string& path) {
    auto itr = std::find(m_filePaths.begin(), m_filePaths.end(), path);
    if (itr == m_filePaths.end()) {
        m_filePaths.emplace_back(path);
    } else {
        LOG(WARNING) << "File path: " << path << " already exist in the paths, ignoring it\n";
    }
}