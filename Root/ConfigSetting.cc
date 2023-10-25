#include "FastFrames/ConfigSetting.h"

ConfigSetting::ConfigSetting() :
m_outputPath(""),
m_regions({}),
m_samples({}),
m_systematics({})
{
    //std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("JET_BJES_Response__1up");
    std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("EG_RESOLUTION_ALL__1down");
    m_systematics.emplace_back(syst);
    std::shared_ptr<Sample> sample = std::make_shared<Sample>("ttbar_FS");
    UniqueSampleID unique(410470, "mc20e", "mc");
    sample->addUniqueSampleID(unique);
    sample->addSystematic(syst);
    std::shared_ptr<Region> reg = std::make_shared<Region>("Electron");
    reg->setSelection("el_pt_NOSYS > 30000 && el_select_tight_NOSYS && el_select_or_NOSYS");
    sample->addRegion(reg);
    m_samples.emplace_back(sample);
}
