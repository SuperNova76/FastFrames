#pragma once

#include "FastFrames/Truth.h"
#include "FastFrames/Variable.h"

#include <memory>
#include <tuple>

class TruthWrapper  {
    public:
        TruthWrapper() = delete;

        explicit TruthWrapper(const std::string& name) :
            m_truth(std::make_shared<Truth>(name))  {};

        ~TruthWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_truth);};

        void constructFromSharedPtr(unsigned long long int truth_shared_ptr_int) {
            m_truth = *reinterpret_cast<std::shared_ptr<Truth> *>(truth_shared_ptr_int);
        };


        std::string name() const {return m_truth->name();};


        void setTruthTreeName(const std::string& treeName) {m_truth->setTruthTreeName(treeName);};

        std::string truthTreeName() const {return m_truth->truthTreeName();};


        void setSelection(const std::string& selection) {m_truth->setSelection(selection);};

        std::string selection() const {return m_truth->selection();};


        void setEventWeight(const std::string& eventWeight) {m_truth->setEventWeight(eventWeight);};

        std::string eventWeight() const {return m_truth->eventWeight();};


        void addMatchVariables(const std::string &reco, const std::string &truth)   {
            m_truth->addMatchVariables(reco, truth);
        };

        unsigned int nMatchedVariables() const {return m_truth->matchedVariables().size();};

        boost::python::tuple matchedVariables(unsigned int i) const {
            const std::pair<std::string, std::string> &matchedPair = m_truth->matchedVariables().at(i);
            return boost::python::make_tuple(matchedPair.first, matchedPair.second);
        };


        void addVariable(unsigned long long variable_shared_ptr) {
            const std::shared_ptr<Variable> *variable = reinterpret_cast<std::shared_ptr<Variable> *>(variable_shared_ptr);
            m_truth->addVariable(*(*variable));
        };

        std::vector<unsigned long long int> getVariableRawPtrs()   const {
            const std::vector<Variable> &variables = m_truth->variables();
            std::vector<unsigned long long int> variable_ptrs;
            for (const auto &variable : variables) {
                variable_ptrs.push_back(reinterpret_cast<unsigned long long int>(&variable));
            }
            return variable_ptrs;
        };

        void setProduceUnfolding(const bool flag)   {m_truth->setProduceUnfolding(flag);};

        bool produceUnfolding() const {return m_truth->produceUnfolding();};

    private:
        std::shared_ptr<Truth> m_truth;

};