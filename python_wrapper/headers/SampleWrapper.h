/**
 * @file SampleWrapper.h
 * @brief Header file for the SampleWrapper class
 *
 */
#pragma once

#include "FastFrames/Sample.h"

#include <memory>
#include <string>

/**
 * @brief A wrapper class for the Sample class, providing an interface for Python.
 */
class SampleWrapper {
    public:
        /**
         * @brief Construct a new Sample Wrapper object
         *
         * @param name
         */
        explicit SampleWrapper(const std::string& name) :
            m_sample(std::make_shared<Sample>(name))  {};

        /**
         * @brief Destroy the Sample Wrapper object
         *
         */
        ~SampleWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Sample>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_sample);};

        /**
         * @brief Get name of the sample
         *
         * @return std::string
         */
        std::string name() const {return m_sample->name();};

        /**
         * @brief Set the name of the reco-level tree
         *
         * @param name
         */
        void setRecoTreeName(const std::string& treeName) {m_sample->setRecoTreeName(treeName);};

        /**
         * @brief Get the name of the reco-level tree
         *
         * @return std::string
         */
        std::string recoTreeName() const {return m_sample->recoTreeName();};

        /**
         * @brief Set additional selection for the sample
         *
         * @param selectionSuffix
         */
        void setSelectionSuffix(const std::string& selectionSuffix) {m_sample->setSelectionSuffix(selectionSuffix);};

        /**
         * @brief Get additional selection for the sample
         *
         * @return std::string
         */
        std::string selectionSuffix() const {return m_sample->selectionSuffix();};

        /**
         * @brief Add unique sample
         *
         * @param dsid
         * @param campaign
         * @param simulation
         */
        void addUniqueSampleID(const unsigned int dsid, const std::string& campaign, const std::string& simulation) {
            m_sample->addUniqueSampleID(UniqueSampleID(dsid, campaign, simulation));
        };

        /**
         * @brief Get number of unique samples
         *
         * @return unsigned int
         */
        unsigned int nUniqueSampleIDs() const {return (m_sample->uniqueSampleIDs()).size();};

        /**
         * @brief Get std::string with identifier of unique sample (dsid, campaign, simulation)
         *
         * @param i
         * @return std::string
         */
        std::string uniqueSampleIDstring(unsigned int i) const {
            const std::vector<UniqueSampleID> &uniqueSamples = m_sample->uniqueSampleIDs();
            const UniqueSampleID &id = uniqueSamples.at(i);
            return "(" + std::to_string(id.dsid()) + "," + id.campaign() + "," + id.simulation() + ")";
        };

        /**
         * @brief Add systematic uncertainty, given the raw pointer to the shared_ptr<Systematic>
         *
         * @param syst_shared_ptr_int - raw pointer to the shared_ptr<Systematic>
         */
        void addSystematic(unsigned long long int syst_shared_ptr_int) {
            const std::shared_ptr<Systematic> *syst = reinterpret_cast<std::shared_ptr<Systematic> *>(syst_shared_ptr_int);
            m_sample->addSystematic(*syst);
        };

        /**
         * @brief Get number of systematic variations defined for the sample
         *
         * @return unsigned int
         */
        unsigned int nSystematics() const {return (m_sample->systematics()).size();};

        /**
         * @brief Get raw pointer to the shared_ptr<Systematic> object
         *
         * @param i
         * @return unsigned long long int
         */
        unsigned long long getSystematicPtr(unsigned int i) const {
            const std::vector<std::shared_ptr<Systematic>> &systematics = m_sample->systematics();
            const std::shared_ptr<Systematic> &syst = systematics.at(i);
            return reinterpret_cast<unsigned long long int>(&syst);
        };

        /**
         * @brief Add region, given the raw pointer to the shared_ptr<Region>
         *
         * @param reg_shared_ptr_int
         */
        void addRegion(unsigned long long int reg_shared_ptr_int) {
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            m_sample->addRegion(*reg);
        };


        /**
         * @brief Set vector<string> of variables used for reco-to-truth pairing (e.g. {"mcChannelNumber", "eventNumber"})
         *
         * @param indices
         */
        void setRecoToTruthPairingIndices(const std::vector<std::string>& indices) {
            m_sample->setRecoToTruthPairingIndices(indices);
        };


        /**
         * @brief Get vector<string> of variables used for reco-to-truth pairing (e.g. {"mcChannelNumber", "eventNumber"})
         *
         * @param std::vector<std::string>
         */
        std::vector<std::string> recoToTruthPairingIndices() const {
            return m_sample->recoToTruthPairingIndices();
        };

        /**
         * @brief Set the event_weight for the sample
         *
         * @param string
         */
        void setEventWeight(const std::string& weight) {m_sample->setEventWeight(weight);};

        /**
         * @brief Get the event_weight for the sample
         *
         * @return std::string
         */
        std::string weight() const {return m_sample->weight();};

        /**
         * @brief Given the raw pointers to the shared_ptr<Systematic> and shared_ptr<Region> objects, should the systematic be applied to the region?
         *
         * @param unsigned long long int - syst_shared_ptr_int
         * @param unsigned long long int - reg_shared_ptr_int
         * @return bool
         */
        bool skipSystematicRegionCombination(unsigned long long int syst_shared_ptr_int, unsigned long long int reg_shared_ptr_int) const {
            const std::shared_ptr<Systematic> *syst = reinterpret_cast<std::shared_ptr<Systematic> *>(syst_shared_ptr_int);
            const std::shared_ptr<Region> *reg = reinterpret_cast<std::shared_ptr<Region> *>(reg_shared_ptr_int);
            return m_sample->skipSystematicRegionCombination(*syst, *reg);
        };

        /**
         * @brief Add truth object, given the raw pointers to the shared_ptr<Truth>
         *
         * @param truth_shared_ptr_int
         */
        void addTruth(unsigned long long int truth_shared_ptr_int) {
            const std::shared_ptr<Truth> *truth = reinterpret_cast<std::shared_ptr<Truth> *>(truth_shared_ptr_int);
            m_sample->addTruth(*truth);
        };

        /**
         * @brief Get std::vector<unsigned long long int> of raw pointers to shared_ptr<Truth> objects
         *
         * @return std::vector<unsigned long long int>
         */
        std::vector<unsigned long long int> getTruthPtrs() const {
            const std::vector<std::shared_ptr<Truth>> &truths = m_sample->truths();
            std::vector<unsigned long long int> truth_ptrs;
            for (const std::shared_ptr<Truth> &truth : truths) {
                truth_ptrs.emplace_back(reinterpret_cast<unsigned long long int>(&truth));
            }
            return truth_ptrs;
        };

        /**
         * @brief Add custom column to the sample, given the name and formula
         *
         * @param newName
         * @param formula
         */
        void addCustomDefine(const std::string &newName, const std::string &formula) {
            m_sample->addCustomDefine(newName, formula);
        };

        /**
         * @brief Get std::vector<std::string> of custom column names
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> customDefines() const {
            const std::vector<std::pair<std::string, std::string> > &defines = m_sample->customDefines();
            std::vector<std::string> result(defines.size());
            for (unsigned int i = 0; i < defines.size(); ++i) {
                result.at(i) = "\"" + defines.at(i).first + "\" -> \"" + defines.at(i).second + "\"" ;
            }
            return result;
        };

    private:
        std::shared_ptr<Sample> m_sample;

};