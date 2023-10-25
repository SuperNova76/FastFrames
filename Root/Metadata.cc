#include "FastFrames/Metadata.h"

#include <algorithm>
#include <iostream>

Metadata::Metadata() :
m_crossSection(-1),
m_sumWeights({})
{
}

void Metadata::addSumWeights(const std::string& name, const double value) {
    auto itr = m_sumWeights.find(name);
    if (itr == m_sumWeights.end()) {
        m_sumWeights.insert({name, value});
    } else {
        std::cerr << "name: " << name << " already found in the list of the sumweights, not adding it again\n";
    }
}

double Metadata::sumWeight(const std::string& name) const {
    auto itr = m_sumWeights.find(name);
    if (itr == m_sumWeights.end()) {
        std::cerr << "Cannot find name: " << name << ", in the list of the sumweights, cannot retrieve it, returning -1\n";
        return -1;
    } else {
        return itr->second;
    }
}

void Metadata::addFilePath(const std::string& path) {
    auto itr = std::find(m_filePaths.begin(), m_filePaths.end(), path);
    if (itr == m_filePaths.end()) {

    } else {
        m_filePaths.emplace_back(path);
    }
}