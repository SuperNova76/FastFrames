#pragma once

#include "FastFrames/Ntuple.h"

#include <memory>
#include <string>

class NtupleWrapper {
    public:
        explicit NtupleWrapper() :
            m_ntuple(std::make_shared<Ntuple>())  {};

        ~NtupleWrapper() = default;

        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_ntuple);};

        void addSample(long long int sample_shared_ptr) {
            const std::shared_ptr<Sample> *sample = reinterpret_cast<std::shared_ptr<Sample> *>(sample_shared_ptr);
            m_ntuple->addSample(*sample);
        }

        unsigned int nSamples() const {return m_ntuple->samples().size();};
        std::string sampleName(unsigned int i) const {return m_ntuple->samples().at(i)->name();};

        void setSelection(const std::string& selection) {m_ntuple->setSelection(selection);}

        const std::string selection() const {return m_ntuple->selection();}


        void addBranch(const std::string& branch) {m_ntuple->addBranch(branch);};

        unsigned int nBranches() const {return m_ntuple->branches().size();};

        std::string branchName(unsigned int i) const {return m_ntuple->branches().at(i);};


        void addExcludedBranch(const std::string& branch) {m_ntuple->addExcludedBranch(branch);};

        unsigned int nExcludedBranches() const {return m_ntuple->excludedBranches().size();};

        std::string excludedBranchName(unsigned int i) const {return m_ntuple->excludedBranches().at(i);};


    private:
        std::shared_ptr<Ntuple> m_ntuple;

};