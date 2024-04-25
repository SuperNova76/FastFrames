/**
 * @file SystematicWrapper.h
 * @brief Header file for the SystematicWrapper class
 *
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

/**
 * @brief A wrapper class for the Systematic class, providing an interface for Python.
 */
class SystematicWrapper {
    public:
        /**
         * @brief Construct a new Systematic Wrapper object
         *
         * @param name
         */
        explicit SystematicWrapper(const std::string& name) :
            m_systematic(std::make_shared<Systematic>(name)) {};

        /**
         * @brief Destroy the Systematic Wrapper object
         *
         */
        ~SystematicWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Systematic>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_systematic);};


        /**
         * @brief Get raw pointer to the Systematic object
         *
         * @return unsigned long long int
         */
        unsigned long long int getRawPtr() const   {return reinterpret_cast<unsigned long long int>(m_systematic.get());};

        /**
         * @brief Construct SystematicWrapper object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_systematic = *reinterpret_cast<std::shared_ptr<Systematic> *>(shared_ptr);
        };

        /**
         * @brief Get name of the systematic
         *
         * @return std::string
         */
        std::string name() const {return m_systematic->name();};

        /**
         * @brief Set the Sum Weights string
         *
         * @param sumWeights
         */
        void setSumWeights(const std::string& sumWeights) {m_systematic->setSumWeights(sumWeights);};

        /**
         * @brief Get the Sum Weights string
         *
         * @return std::string
         */
        std::string sumWeights() const {return m_systematic->sumWeights();};

        /**
         * @brief Set the weight suffix - additional (multiplicative) weight for the sample
         *
         * @param weightSuffix
         */
        void setWeightSuffix(const std::string& weightSuffix) {m_systematic->setWeightSuffix(weightSuffix);};

        /**
         * @brief Get weight suffix - additional (multiplicative) weight for the sample
         *
         * @return std::string
         */
        std::string weightSuffix() const {return m_systematic->weightSuffix();};

        /**
         * @brief Add region to the systematic, given the raw pointer of the shared_ptr<Region>
         *
         * @param unsigned long long int reg_shared_ptr_int -> raw pointer of the shared_ptr<Region>
         */
        void addRegion(unsigned long long int reg_shared_ptr_int) {
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            m_systematic->addRegion(*reg);
        };

        /**
         * @brief Get std::vector<std::string> names of regions for which the systematics is defined
         *
         */
        std::vector<std::string> regionsNames()     const {
            const std::vector<std::shared_ptr<Region>> &regions = m_systematic->regions();
            std::vector<std::string> region_names(regions.size());
            for (unsigned int i = 0; i < regions.size(); ++i) {
                region_names.at(i) = regions.at(i)->name();
            }
            return region_names;
        };

        /**
         * @brief Is the systematic nominal?
         *
         * @return true
         * @return false
         */
        bool isNominal() const {return m_systematic->isNominal();};


    private:
        std::shared_ptr<Systematic> m_systematic;
};