#include "FastFrames/ConfigSetting.h"

#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"

ConfigSetting::ConfigSetting() :
m_outputPath(""),
m_inputSumWeightsPath("test/input/sum_of_weights.txt"),
m_inputFilelistPath("test/input/filelist.txt"),
m_regions({}),
m_samples({}),
m_systematics({})
{
    std::shared_ptr<Region> reg = std::make_shared<Region>("Electron");
    reg->setSelection("el_pt_NOSYS[0] > 30000");
    std::shared_ptr<Systematic> nominal = std::make_shared<Systematic>("NOSYS");
    nominal->setSumWeights("NOSYS");
    nominal->addRegion(reg);
    m_systematics.emplace_back(nominal);
    std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("EG_RESOLUTION_ALL__1down");
    syst->setSumWeights("NOSYS");
    syst->addRegion(reg);
    m_systematics.emplace_back(syst);
    std::shared_ptr<Sample> sample = std::make_shared<Sample>("ttbar_FS");
    UniqueSampleID unique(410470, "mc20e", "mc");
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
}


void ConfigSetting::addLuminosityInformation(const std::string& campaign, const float luminosity)   {
    if (m_luminosity_map.find(campaign) != m_luminosity_map.end()) {
        throw std::runtime_error("Campaign " + campaign + " already exists in the luminosity map");
    }
    m_luminosity_map[campaign] = luminosity;
};


float ConfigSetting::getLuminosity(const std::string& campaign) const  {
    if (m_luminosity_map.find(campaign) == m_luminosity_map.end()) {
        throw std::runtime_error("Campaign " + campaign + " does not exist in the luminosity map");
    }
    return m_luminosity_map.at(campaign);
};


void ConfigSetting::addRegion(const std::shared_ptr<Region>& region) {
    LOG(INFO) << "Adding region " << region->name() << "\n";
    m_regions.emplace_back(region);
}
