/**
 * @file MetadataManager.cc
 * @brief Pocessing of sample metadata information
 *
 */

#include "FastFrames/MetadataManager.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/XSectionManager.h"

#include <fstream>
#include <exception>

MetadataManager::MetadataManager() noexcept
{

}

void MetadataManager::readFileList(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open() || !file.good()) {
        LOG(ERROR) << "Cannot open file with file list at: " << path << "\n";
        throw std::invalid_argument("");
    }
    LOG(DEBUG) << "Reading file list from: " << path << "\n";

    int dsid;
    std::string campaign;
    std::string simulation;
    std::string filePath;

    while (file >> dsid >> campaign >> simulation >> filePath) {
        UniqueSampleID id(dsid, campaign, simulation);
        auto itr = m_metadata.find(id);
        if (itr == m_metadata.end()) {
            Metadata metadata;
            metadata.addFilePath(filePath);
            m_metadata.insert({id, metadata});
        } else {
            itr->second.addFilePath(filePath);
        }
    }

    file.close();
}

void MetadataManager::readSumWeights(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open() || !file.good()) {
        LOG(ERROR) << "Cannot open file with sumWeights list at: " << path << "\n";
        throw std::invalid_argument("");
    }

    LOG(DEBUG) << "Reading sumWeights from: " << path << "\n";

    int dsid;
    std::string campaign;
    std::string simulation;
    std::string name;
    double sumOfWeights;

    while (file >> dsid >> campaign >> simulation >> name >> sumOfWeights) {
        UniqueSampleID id(dsid, campaign, simulation);
        auto itr = m_metadata.find(id);
        if (itr == m_metadata.end()) {
            Metadata metadata;
            metadata.addSumWeights(name, sumOfWeights);
            m_metadata.insert({id, metadata});
        } else {
            itr->second.addSumWeights(name, sumOfWeights);
        }
    }

    file.close();

}

void MetadataManager::readXSectionFiles(const std::vector<std::string>& xSectionFiles)  {
    XSectionManager xSectionManger(xSectionFiles);

    for (auto &m_mapPair : m_metadata)    {
        const UniqueSampleID &uniqueSampleId = m_mapPair.first;
        if (!uniqueSampleId.isData()) {
            Metadata &metadata = m_mapPair.second;
            metadata.setCrossSection(xSectionManger.xSection(uniqueSampleId.dsid()));
        }
    }
};

void MetadataManager::addLuminosity(const std::string& campaign, const double lumi) {
    if (lumi <= 0) {
        LOG(WARNING) << "Luminosity for campaign: " << campaign << " is <= 0, ignoring\n";
        return;
    }
    auto itr = m_luminosity.find(campaign);
    if (itr == m_luminosity.end()) {
        LOG(INFO) << "Adding luminosity for campaign: " << campaign << ", value: " << lumi << "\n";
        m_luminosity.insert({campaign, lumi});
    } else {
        LOG(INFO) << "Changing luminosity for campaign: " << campaign << " to value: " << lumi << "\n";
        itr->second = lumi;
    }
}

double MetadataManager::sumWeights(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const {
    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find the correct sample in the map for the sumweights\n";
        throw std::invalid_argument("");
    }

    return itr->second.sumWeight(systematic->sumWeights());
}

bool MetadataManager::sumWeightsExist(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const {
    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find the correct sample in the map for the sumweights\n";
        throw std::invalid_argument("");
    }

    return itr->second.sumWeightExist(systematic->name());
}

double MetadataManager::luminosity(const std::string& campaign) const {
    auto itr = m_luminosity.find(campaign);
    if (itr == m_luminosity.end()) {
        LOG(ERROR) << "Cannot find the campaign in the luminosity map\n";
        throw std::invalid_argument("");
    }

    return itr->second;
}

double MetadataManager::crossSection(const UniqueSampleID& id) const {

    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find the correct sample in the map for the cross section\n";
        throw std::invalid_argument("");
    }

    const double crossSection = itr->second.crossSection();

    if (crossSection <= 0) {
        LOG(WARNING) << "Cross-section for sample: " << id << " is <= 0, this is suspicious\n";
    }

    return crossSection;
}

double MetadataManager::normalisation(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const {
    return this->crossSection(id) * this->luminosity(id.campaign()) / this->sumWeights(id, systematic);
}

const std::vector<std::string>& MetadataManager::filePaths(const UniqueSampleID& id) const {
    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find ID " << id << " in the map! Please, fix\n";
        throw std::invalid_argument("");
    }

    return itr->second.filePaths();
}
