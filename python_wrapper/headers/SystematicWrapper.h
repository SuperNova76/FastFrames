#pragma once


#include <memory>
#include <string>
#include <vector>

class SystematicWrapper {
    public:
        explicit SystematicWrapper(const std::string& name) :
            m_systematic(std::make_shared<Systematic>(name)) {};

        ~SystematicWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_systematic);};

        std::string name() const {return m_systematic->name();};

        void setSumWeights(const std::string& sumWeights) {m_systematic->setSumWeights(sumWeights);};

        std::string sumWeights() const {return m_systematic->sumWeights();};

        void setWeightSuffix(const std::string& weightSuffix) {m_systematic->setWeightSuffix(weightSuffix);};

        std::string weightSuffix() const {return m_systematic->weightSuffix();};

        void addRegion(unsigned long long int reg_shared_ptr_int) {
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            m_systematic->addRegion(*reg);
        };


    private:
        std::shared_ptr<Systematic> m_systematic;
};