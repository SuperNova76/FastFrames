/**
 * @file NtupleWrapper.h
 * @brief Header file for the NtupleWrapper class
 *
 */
#pragma once

#include "FastFrames/Ntuple.h"

#include <memory>
#include <string>

/**
 * @brief A wrapper class for Ntuple objects.
 *
 * This class provides a simplified interface to interact with Ntuple objects from python.
 * It allows adding samples, setting selection, adding branches, and adding excluded branches.
 */
class NtupleWrapper {
    public:

        /**
         * @brief Construct a new NtupleWrapper object
         *
         */
        explicit NtupleWrapper() :
            m_ntuple(std::make_shared<Ntuple>())  {};

        /**
         * @brief Destroy the Ntuple Wrapper object
         *
         */
        ~NtupleWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Ntuple>
         *
         * @return unsigned long long int - pointer casted to unsigned long long int
         */
        unsigned long long int getPtr() const   {return reinterpret_cast<unsigned long long int>(&m_ntuple);};

        /**
         * @brief Construct NtupleWrapper object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_ntuple = *reinterpret_cast<std::shared_ptr<Ntuple> *>(shared_ptr);
        }

        /**
         * @brief Add sample to the Ntuple, given the raw pointer to the shared_ptr<Sample>
         *
         * @param sample_shared_ptr
         */
        void addSample(long long int sample_shared_ptr) {
            const std::shared_ptr<Sample> *sample = reinterpret_cast<std::shared_ptr<Sample> *>(sample_shared_ptr);
            m_ntuple->addSample(*sample);
        }

        /**
         * @brief Get number of added samples
         *
         * @return unsigned int
         */
        unsigned int nSamples() const {return m_ntuple->samples().size();};

        /**
         * @brief Get sample name
         *
         * @param i - index of the sample
         * @return std::string
         */
        std::string sampleName(unsigned int i) const {return m_ntuple->samples().at(i)->name();};

        /**
         * @brief Sets the selection string for the Ntuple.
         *
         * @param selection The selection string to be set.
         */
        void setSelection(const std::string& selection) {m_ntuple->setSelection(selection);}

        /**
         * @brief Get selection string
         *
         * @return const std::string
         */
        const std::string selection() const {return m_ntuple->selection();}

        /**
         * @brief Add branch to the output Ntuple
         *
         * @param branch
         */
        void addBranch(const std::string& branch) {m_ntuple->addBranch(branch);};

        /**
         * @brief Get number of added branches
         *
         * @return unsigned int
         */
        unsigned int nBranches() const {return m_ntuple->branches().size();};

        /**
         * @brief Get branch name
         *
         * @param i - index of the branch
         * @return std::string
         */
        std::string branchName(unsigned int i) const {return m_ntuple->branches().at(i);};

        /**
         * @brief Add excluded branch
         *
         * @param branch
         */
        void addExcludedBranch(const std::string& branch) {m_ntuple->addExcludedBranch(branch);};

        /**
         * @brief Get number of excluded branches
         *
         * @return unsigned int
         */
        unsigned int nExcludedBranches() const {return m_ntuple->excludedBranches().size();};

        /**
         * @brief Get excluded branch name
         *
         * @param i - index of the excluded branch
         * @return std::string
         */
        std::string excludedBranchName(unsigned int i) const {return m_ntuple->excludedBranches().at(i);};

        /**
         * @brief Add a tree to copy
         *
         * @param tree
         */
        void addCopyTree(const std::string& tree) { m_ntuple->addCopyTree(tree);}

        /**
         * @brief Get trees to copy
         *
         * @return const std::vector<std::string>&
         */
        std::vector<std::string> copyTrees() const {return m_ntuple->copyTrees();}


    private:
        std::shared_ptr<Ntuple> m_ntuple;

};