#pragma once

#include "FastFrames/Sample.h"

#include <memory>
#include <string>

class SampleWrapper {
    public:
        SampleWrapper(const std::string& name) :
            m_sample(std::make_shared<Sample>(name))  {};

        ~SampleWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_sample);};

        std::string name() const {return m_sample->name();};

        std::string recoTreeName() const {return m_sample->recoTreeName();};

        void addUniqueSampleID(const unsigned int dsid, const std::string& campaign, const std::string& simulation) {
            m_sample->addUniqueSampleID(UniqueSampleID(dsid, campaign, simulation));
        };

        void addSystematic(unsigned long long int syst_shared_ptr_int) {
            const std::shared_ptr<Systematic> *syst = reinterpret_cast<std::shared_ptr<Systematic> *>(syst_shared_ptr_int);
            m_sample->addSystematic(*syst);
        };

        void addRegion(unsigned long long int reg_shared_ptr_int) {
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            m_sample->addRegion(*reg);
        };

        void setEventWeight(const std::string& weight) {m_sample->setEventWeight(weight);};

        std::string weight() const {return m_sample->weight();};

        bool skipSystematicRegionCombination(unsigned long long int syst_shared_ptr_int, unsigned long long int reg_shared_ptr_int) const {
            const std::shared_ptr<Systematic> *syst = reinterpret_cast<std::shared_ptr<Systematic> *>(syst_shared_ptr_int);
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            return m_sample->skipSystematicRegionCombination(*syst, *reg);
        };

    private:
        std::shared_ptr<Sample> m_sample;

};