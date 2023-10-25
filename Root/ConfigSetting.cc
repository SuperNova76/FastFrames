#include "FastFrames/ConfigSetting.h"

ConfigSetting::ConfigSetting() :
m_outputPath(""),
m_regions({}),
m_samples({}),
m_systematics({})
{
    std::shared_ptr<Systematic> syst = std::make_shared<Systematic>("JET_BJES_Response__1up");
    m_systematics.emplace_back(syst);
    std::shared_ptr<Sample> sample = std::make_shared<Sample>("ttbar_FS");
    UniqueSampleID unique(410470, "mc20e", "mc");
    sample->addUniqueSampleID(unique);
    m_samples.emplace_back(sample);
}
