/**
 * @file MetadataManager.cc
 * @brief Pocessing of sample metadata information
 *
 */

#include "FastFrames/MetadataManager.h"

#include "FastFrames/Logger.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Utils.h"
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
        if (uniqueSampleId.isData()) continue;
        Metadata &metadata = m_mapPair.second;
        const double xSec = xSectionManger.xSection(uniqueSampleId.dsid());
        LOG(INFO) << "Cross-section for UniqueSample: " << uniqueSampleId << " is: " << xSec << "\n";
        if (xSec <= 0) {
            LOG(WARNING) << "Cross-section for UniqueSample: " << uniqueSampleId << " is <= 0. Please check!";
        }
        metadata.setCrossSection(xSec);
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
    if (id.isData()) {
        LOG(DEBUG) << "UniqueSample: " << id << " is data, returning sum weights = 0";
        return 0;
    }
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
    if (id.isData()) {
        LOG(DEBUG) << "UniqueSample: " << id << " is data, returning cross-section 0\n";
        return 0;
    }

    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find the correct sample in the map for the cross section\n";
        throw std::invalid_argument("");
    }

    const double crossSection = itr->second.crossSection();

    return crossSection;
}

double MetadataManager::normalisation(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const {

    if (id.isData()) {
        LOG(DEBUG) << "UniqueSample: " << id << " is data, returning normalisation = 1\n";
        return 1;
    }

    return this->crossSection(id) * this->luminosity(id.campaign()) / this->sumWeights(id, systematic);
}

const std::vector<std::string>& MetadataManager::filePaths(const UniqueSampleID& id) const {
    auto itr = m_metadata.find(id);
    if (itr == m_metadata.end()) {
        LOG(ERROR) << "Cannot find ID " << id << " in the filelist and metadata text file! Please, fix\n";
        throw std::invalid_argument("");
    }

    return itr->second.filePaths();
}

bool MetadataManager::checkSamplesMetadata(const std::vector<std::shared_ptr<Sample> >& samples) const {

    for (const auto& isample : samples) {
        for (const auto& id : isample->uniqueSampleIDs()) {
           if (!this->checkUniqueSampleIDMetadata(id)) return false;
        }
    }

    return true;
}

bool MetadataManager::checkUniqueSampleIDMetadata(const UniqueSampleID& id) const {

    auto itrMeta = m_metadata.find(id);
    if (itrMeta == m_metadata.end()) {
        LOG(ERROR) << "Cannot find metadata for unique sample : " << id << "!\n";
        return false;
    }

    if (id.isData()) return true;

    if (itrMeta->second.sumWeightsIsEmpty()) {
        LOG(ERROR) << "Sum weights for unique sample: " << id << " are empty\n";
        return false;
    }

    if (!itrMeta->second.crossSectionSet()) {
        LOG(ERROR) << "Cross-section for unique sample: " << id << " is not set\n";
        return false;
    }

    return true;
}

ROOT::RDF::Experimental::RDatasetSpec MetadataManager::dataSpec(const std::shared_ptr<Sample>& sample,
                                                                const std::shared_ptr<ConfigSetting>& config) const {

    ROOT::RDF::Experimental::RDatasetSpec spec;
    for (const auto& id : sample->uniqueSampleIDs()) {
        spec.AddSample(this->singleSampleInfo(sample, id, config));
    }

    return spec;
}

ROOT::RDF::Experimental::RSample MetadataManager::singleSampleInfo(const std::shared_ptr<Sample>& sample,
                                                                   const UniqueSampleID& id,
                                                                   const std::shared_ptr<ConfigSetting>& config) const {


    std::vector<std::string> paths = this->filePaths(id);
    if (config->totalJobSplits() > 0) {
        paths = Utils::selectedFileList(paths, config->totalJobSplits(), config->currentJobIndex());
    }
    if (paths.empty()) {
        LOG(WARNING) << "UniqueSample: " << id << " has no files, will not produce output ntuple\n";
    }

    ROOT::RDF::Experimental::RMetaData meta = this->sampleMetadata(sample, id);

    ROOT::RDF::Experimental::RSample result(sample->name(), sample->recoTreeName(), this->filePaths(id), meta);

    return result;
}

ROOT::RDF::Experimental::RMetaData MetadataManager::sampleMetadata(const std::shared_ptr<Sample>& sample,
                                                                   const UniqueSampleID& id) const {
    ROOT::RDF::Experimental::RMetaData meta;

    if (sample->isData()) return meta;

    meta.Add("luminosity", this->luminosity(id.campaign()));
    meta.Add("xSection", this->crossSection(id));

    bool addedNominal(false);

    for (const auto& isyst : sample->systematics()) {
        const std::string systName = isyst->name();
        const std::string weightName = "sumWeights_" + isyst->sumWeights();
        if (systName == "NOSYS") {
            // add only once
            if (addedNominal) continue;

            // has not beed added yet
            addedNominal = true;
        }

        // only add non-nominal sumWeights
        if (systName != "NOSYS" && isyst->sumWeights() == "NOSYS") continue;

        meta.Add(weightName, this->sumWeights(id, isyst));
    }

    return meta;
}
