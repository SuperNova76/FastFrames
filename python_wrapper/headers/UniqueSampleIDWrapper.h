#pragma once

#include "FastFrames/UniqueSampleID.h"

#include <memory>
#include <string>

class UniqueSampleIDWrapper {
    public:
        UniqueSampleIDWrapper(const int dsid, const std::string& campaign, const std::string& simulation) :
            m_uniqueSampleID(std::make_shared<UniqueSampleID>(dsid, campaign, simulation))  {};

        ~UniqueSampleIDWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_uniqueSampleID);};

        int dsid() const {return m_uniqueSampleID->dsid();};

        std::string campaign()   const {return m_uniqueSampleID->campaign();};

        std::string simulation() const {return m_uniqueSampleID->simulation();};

    private:
        std::shared_ptr<UniqueSampleID> m_uniqueSampleID;
};