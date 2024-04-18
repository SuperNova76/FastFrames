/**
 * @file RegionWrapper.h
 * @brief Header file for the RegionWrapper class
 *
 */
#pragma once

#include "FastFrames/Region.h"


#include <string>
#include <vector>
#include <tuple>
#include <memory>

/**
 * @brief Wrapper around region class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 */
class RegionWrapper {
    public:
        RegionWrapper() = delete;

        /**
         * @brief Construct a new Region Wrapper object with given name
         *
         * @param name
         */
        explicit RegionWrapper(const std::string& name) : m_region(std::make_shared<Region>(name)) {};

        /**
         * @brief Destroy the Region Wrapper object
         *
         */
        ~RegionWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Region>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_region);
        }

        /**
         * @brief Build region object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_region = *reinterpret_cast<std::shared_ptr<Region> *>(shared_ptr);
        };

        /**
         * @brief Get name of the region
         *
         * @return std::string
         */
        std::string name() const {
            return m_region->name();
        };


        /**
         * Sets the selection for the region.
         *
         * @param selection The selection to set.
         */
        void setSelection(const std::string& selection) {
            m_region->setSelection(selection);
        };

        /**
         * @brief Returns the selection string.
         *
         * @return std::string The selection string.
         */
        std::string selection() const {
            return m_region->selection();
        };

        /**
         * @brief Add variable to the region, given the raw pointer to the shared_ptr<Variable>
         *
         * @param variable_ptr - raw pointer to the shared_ptr<Variable>
         */
        void addVariable(unsigned long long int variable_ptr) {
            const std::shared_ptr<Variable> *variable = reinterpret_cast<std::shared_ptr<Variable> *>(variable_ptr);
            m_region->addVariable(*(*variable));
        };

        /**
         * @brief Get the vector of raw pointers to the shared_ptr<Variable> objects
         *
         * @return std::vector<unsigned long long int>
         */
        std::vector<unsigned long long int> getVariableRawPtrs()   const {
            const std::vector<Variable> &variables = m_region->variables();
            std::vector<unsigned long long int> variable_ptrs;
            for (const auto &variable : variables) {
                variable_ptrs.push_back(reinterpret_cast<unsigned long long int>(&variable));
            }
            return variable_ptrs;
        };


        /**
         * @brief Get names of the variables defined in this region
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> getVariableNames()   const {
            const std::vector<Variable> &variables = m_region->variables();
            std::vector<std::string> variable_names;
            for (const auto &variable : variables) {
                variable_names.push_back(variable.name());
            }
            return variable_names;
        };

        /**
         * @brief Add combination of 2 variables for 2D histograms
         *
         * @param v1 variable #1 name
         * @param v2 variable #2 name
         */
        void addVariableCombination(const std::string& v1, const std::string& v2)   {
            m_region->addVariableCombination(v1, v2);
        };

        /**
         * @brief Get vector of comma separated pairs of variable names for 2D histograms
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> variableCombinations() const {
            const std::vector<std::pair<std::string,std::string>> &combinations = m_region->variableCombinations();
            std::vector<std::string> combinations_python;
            for (const auto &combination : combinations) {
                combinations_python.push_back(combination.first + ", "+ combination.second);
            }
            return combinations_python;
        };

        /**
         * @brief Add combination of 3 variables for 3D histograms
         *
         * @param v1 variable #1 name
         * @param v2 variable #2 name
         * @param v3 variable #3 name
         */
        void addVariableCombination3D(const std::string& v1, const std::string& v2, const std::string& v3) {
            m_region->addVariableCombination3D(v1, v2, v3);
        };

        /**
         * @brief Get vector of comma separated pairs of variable names for 3D histograms
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> variableCombinations3D() const {
            const std::vector<std::tuple<std::string,std::string,std::string>> &combinations = m_region->variableCombinations3D();
            std::vector<std::string> combinations_python;
            for (const auto &combination : combinations) {
                combinations_python.push_back(std::get<0>(combination) + ", " + std::get<1>(combination) + ", " + std::get<2>(combination));
            }
            return combinations_python;
        };

    private:
        std::shared_ptr<Region> m_region;

};