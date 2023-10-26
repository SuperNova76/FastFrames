#include "FastFrames/ConfigSetting.h"

#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"

ConfigSetting::ConfigSetting() :
m_outputPath(""),
m_regions({}),
m_samples({}),
m_systematics({})
{
    std::shared_ptr<Systematic> nominal = std::make_shared<Systematic>("NOSYS");
    nominal->setSumWeights("NOSYS");
    m_systematics.emplace_back(nominal);
    std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("EG_RESOLUTION_ALL__1down");
    syst->setSumWeights("NOSYS");
    m_systematics.emplace_back(syst);
    std::shared_ptr<Sample> sample = std::make_shared<Sample>("ttbar_FS");
    UniqueSampleID unique(410470, "mc20e", "mc");
    sample->addUniqueSampleID(unique);
    sample->addSystematic(syst);
    sample->addSystematic(nominal);
    sample->setEventWeight({"weight_mc_NOSYS * weight_beamspot * weight_pileup_NOSYS"});
    std::shared_ptr<Region> reg = std::make_shared<Region>("Electron");
    reg->setSelection("el_pt_NOSYS[0] > 30000");

    Variable var("jet_pt_NOSYS");
    var.setDefinition("jet_pt_NOSYS");
    var.setBinning(0, 300000, 10);
    reg->addVariable(var);

    sample->addRegion(reg);
    m_samples.emplace_back(sample);
}
