/**
 * @file TruthWrapper.h
 * @brief Header file for the TruthWrapper class
 *
 */
#pragma once

#include "FastFrames/Truth.h"
#include "FastFrames/Variable.h"

#include <memory>
#include <tuple>

/**
 * @brief Wrapper around the Truth level object to be able to interact with the class from python
 *
 */
class TruthWrapper  {
    public:
        TruthWrapper() = delete;

        /**
         * @brief Construct a new TruthWrapper object with a given name
         *
         * @param std::string - name
         */
        explicit TruthWrapper(const std::string& name) :
            m_truth(std::make_shared<Truth>(name))  {};

        /**
         * @brief Destroy the Truth Wrapper object
         *
         */
        ~TruthWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Truth>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_truth);};

        /**
         * @brief Construct wrapper around the Truth level object, given the raw pointer to shared_ptr<Truth>
         *
         * @param log long int - truth_shared_ptr_int
         */
        void constructFromSharedPtr(unsigned long long int truth_shared_ptr_int) {
            m_truth = *reinterpret_cast<std::shared_ptr<Truth> *>(truth_shared_ptr_int);
        };

        /**
         * @brief Get name of the truth level object
         *
         * @return std::string
         */
        std::string name() const {return m_truth->name();};


        /**
         * @brief Set the Truth Tree Name
         *
         * @param std::string treeName
         */
        void setTruthTreeName(const std::string& treeName) {m_truth->setTruthTreeName(treeName);};

        /**
         * @brief Get truth tree name
         *
         * @return std::string
         */
        std::string truthTreeName() const {return m_truth->truthTreeName();};

        /**
         * @brief Set the Selection string
         *
         * @param std::string selection
         */
        void setSelection(const std::string& selection) {m_truth->setSelection(selection);};

        /**
         * @brief Get selection string
         *
         * @return std::string
         */
        std::string selection() const {return m_truth->selection();};

        /**
         * @brief Set the Event Weight string
         *
         * @param std::string - eventWeight
         */
        void setEventWeight(const std::string& eventWeight) {m_truth->setEventWeight(eventWeight);};

        /**
         * @brief Get event weight string
         *
         * @return std::string
         */
        std::string eventWeight() const {return m_truth->eventWeight();};

        /**
         * @brief Add pair of variables for unfolding - reco level and truth level
         *
         * @param std::string - reco
         * @param std::string - truth
         */
        void addMatchVariables(const std::string &reco, const std::string &truth)   {
            m_truth->addMatchVariables(reco, truth);
        };

        /**
         * @brief Get number of matched variables
         *
         * @return unsigned int
         */
        unsigned int nMatchedVariables() const {return m_truth->matchedVariables().size();};

        /**
         * @brief Get pair of the matched variables. First -> reco, second -> truth
         *
         * @param unsigned int - index of the matched variables
         * @return boost::python::tuple
         */
        boost::python::tuple matchedVariables(unsigned int i) const {
            const std::pair<std::string, std::string> &matchedPair = m_truth->matchedVariables().at(i);
            return boost::python::make_tuple(matchedPair.first, matchedPair.second);
        };

        /**
         * @brief Add variable to the truth block, given the raw pointer to the shared_ptr<Variable>
         *
         * @param long long int - variable_shared_ptr
         */
        void addVariable(unsigned long long variable_shared_ptr) {
            const std::shared_ptr<Variable> *variable = reinterpret_cast<std::shared_ptr<Variable> *>(variable_shared_ptr);
            m_truth->addVariable(*(*variable));
        };

        /**
         * @brief Get std::vector<unsigned long long int> of raw pointers to the shared_ptr<Variable>
         *
         * @return unsigned int
         */
        std::vector<unsigned long long int> getVariableRawPtrs()   const {
            const std::vector<Variable> &variables = m_truth->variables();
            std::vector<unsigned long long int> variable_ptrs;
            for (const auto &variable : variables) {
                variable_ptrs.push_back(reinterpret_cast<unsigned long long int>(&variable));
            }
            return variable_ptrs;
        };

        /**
         * @brief Set if the unfolding inputs (MMs and corrections) should be produced
         *
         * @param flag
         */
        void setProduceUnfolding(const bool flag)   {m_truth->setProduceUnfolding(flag);};

        /**
         * @brief Should inputs for the unfolding (MMs and corrections) be produced?
         *
         * @return true
         * @return false
         */
        bool produceUnfolding() const {return m_truth->produceUnfolding();};


        /**
         * @brief Add custom column to the truth object, given the name and formula
         *
         * @param newName
         * @param formula
         */
        void addCustomDefine(const std::string &newName, const std::string &formula) {
            m_truth->addCustomDefine(newName, formula);
        };

        /**
         * @brief Get std::vector<std::string> of custom column names
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> customDefines() const {
            const std::vector<std::pair<std::string, std::string> > &defines = m_truth->customDefines();
            std::vector<std::string> result(defines.size());
            for (unsigned int i = 0; i < defines.size(); ++i) {
                result.at(i) = "\"" + defines.at(i).first + "\" -> \"" + defines.at(i).second + "\"" ;
            }
            return result;
        };

        /**
         * @brief Set the Match Reco Truth flag
         *
         * @param flag
         */
        void setMatchRecoTruth(const bool flag) {m_truth->setMatchRecoTruth(flag);}

        /**
         * @brief match reco and truth?
         *
         * @return true
         * @return false
         */
        bool matchRecoTruth() const {return m_truth->matchRecoTruth();}

    private:
        std::shared_ptr<Truth> m_truth;

};