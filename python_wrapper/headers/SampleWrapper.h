#pragma once

#include "FastFrames/Sample.h"

#include <memory>
#include <string>

class SampleWrapper {
    public:
        explicit SampleWrapper(const std::string& name) :
            m_sample(std::make_shared<Sample>(name))  {};

        ~SampleWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_sample);};

        std::string name() const {return m_sample->name();};

        void setRecoTreeName(const std::string& treeName) {m_sample->setRecoTreeName(treeName);};

        std::string recoTreeName() const {return m_sample->recoTreeName();};


        void setSelectionSuffix(const std::string& selectionSuffix) {m_sample->setSelectionSuffix(selectionSuffix);};

        std::string selectionSuffix() const {return m_sample->selectionSuffix();};


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


        unsigned int nUniqueSampleIDs() const {return (m_sample->uniqueSampleIDs()).size();};

        std::string uniqueSampleIDstring(unsigned int i) const {
            const std::vector<UniqueSampleID> &uniqueSamples = m_sample->uniqueSampleIDs();
            const UniqueSampleID &id = uniqueSamples.at(i);
            return "(" + std::to_string(id.dsid()) + "," + id.campaign() + "," + id.simulation() + ")";
        };

    private:
        std::shared_ptr<Sample> m_sample;

};