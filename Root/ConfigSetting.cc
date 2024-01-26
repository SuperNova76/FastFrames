/**
 * @file ConfigSetting.cc
 * @brief Config class
 *
 */

#include "FastFrames/ConfigSetting.h"

#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Logger.h"

#include <algorithm>

ConfigSetting::ConfigSetting() :
m_outputPathHistograms(""),
m_outputPathNtuples(""),
m_inputSumWeightsPath("test/input/sum_of_weights.txt"),
m_inputFilelistPath("test/input/filelist.txt"),
m_regions({}),
m_samples({}),
m_systematics({})
{
    m_luminosity_map.insert({"mc20a", 3244.54+33402.2});
    m_luminosity_map.insert({"mc20d", 44630.6});
    m_luminosity_map.insert({"mc20e", 58791.6});
    m_luminosity_map.insert({"mc23a", 29049.3});
    m_luminosity_map.insert({"mc23c", 1});
};

void ConfigSetting::setTestingValues()  {
    std::shared_ptr<Region> reg = std::make_shared<Region>("Electron");
    reg->setSelection("el_pt_NOSYS[0] > 30000");
    //reg->setSelection("passed_4j50GeV_1btag_NOSYS");
    m_regions.emplace_back(reg);
    std::shared_ptr<Systematic> nominal = std::make_shared<Systematic>("NOSYS");
    nominal->setSumWeights("NOSYS");
    nominal->addRegion(reg);
    m_systematics.emplace_back(nominal);
    std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("EG_RESOLUTION_ALL__1down");
    syst->setSumWeights("NOSYS");
    syst->addRegion(reg);
    m_systematics.emplace_back(syst);
    std::shared_ptr<Sample> sample = std::make_shared<Sample>("ttbar_FS");
    UniqueSampleID unique(410470, "mc20e", "fullsim");
    sample->addUniqueSampleID(unique);
    sample->addSystematic(syst);
    sample->addSystematic(nominal);
    sample->setEventWeight({"weight_mc_NOSYS * weight_beamspot * weight_pileup_NOSYS"});

    Variable var("jet_pt_NOSYS");
    var.setDefinition("jet_pt_NOSYS");
    var.setBinning(0, 300000, 10);
    reg->addVariable(var);

    sample->addRegion(reg);
    m_samples.emplace_back(sample);

    addXsectionFile("data/XSection-MC16-13TeV.data");
};

void ConfigSetting::addLuminosityInformation(const std::string& campaign, const float luminosity, const bool force)   {
    if (m_luminosity_map.find(campaign) != m_luminosity_map.end() && !force) {
        LOG(ERROR) << "Campaign " + campaign + " already exists in the luminosity map\n";
        throw std::runtime_error("");
    }
    m_luminosity_map[campaign] = luminosity;
};


float ConfigSetting::getLuminosity(const std::string& campaign) const  {
    if (m_luminosity_map.find(campaign) == m_luminosity_map.end()) {
        LOG(ERROR) << "Campaign " << campaign << " does not exist in the luminosity map\n";
        throw std::runtime_error("");
    }
    return m_luminosity_map.at(campaign);
};


void ConfigSetting::addRegion(const std::shared_ptr<Region>& region) {
    LOG(INFO) << "Adding region " << region->name() << "\n";
    m_regions.emplace_back(region);
}

void ConfigSetting::addUniqueSystematic(const std::shared_ptr<Systematic>& syst) {
    auto itr = std::find_if(m_systematics.begin(), m_systematics.end(), [&syst](const auto& element){return element->name() == syst->name();});

    if (itr == m_systematics.end()) {
        m_systematics.emplace_back(syst);
    }
}

bool ConfigSetting::hasAutomaticSystematics() const {
    for (const auto& isample : m_samples) {
        if (isample->automaticSystematics()) return true;
        if (isample->nominalOnly()) return true;
    }

    return false;
}
