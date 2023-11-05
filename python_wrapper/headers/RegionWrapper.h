#pragma once

#include "FastFrames/Region.h"


#include <string>
#include <vector>
#include <memory>

/**
 * @brief Wrapper around region class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 */

class RegionWrapper {
    public:
        RegionWrapper() = delete;

        explicit RegionWrapper(const std::string& name) : m_region(std::make_shared<Region>(name)) {};

        ~RegionWrapper() = default;

        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_region);
        }


        std::string name() const {
            return m_region->name();
        };


        void setSelection(const std::string& selection) {
            m_region->setSelection(selection);
        };

        std::string selection() const {
            return m_region->selection();
        };


        void addVariable(unsigned long long int variable_ptr) {
            const std::shared_ptr<Variable> *variable = reinterpret_cast<std::shared_ptr<Variable> *>(variable_ptr);
            m_region->addVariable(*(*variable));
        };

        std::vector<unsigned long long int> getVariableRawPtrs()   const {
            const std::vector<Variable> &variables = m_region->variables();
            std::vector<unsigned long long int> variable_ptrs;
            for (const auto &variable : variables) {
                variable_ptrs.push_back(reinterpret_cast<unsigned long long int>(&variable));
            }
            return variable_ptrs;
        };


    private:
        std::shared_ptr<Region> m_region;

};