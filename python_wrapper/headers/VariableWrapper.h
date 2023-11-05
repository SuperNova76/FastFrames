#pragma once

#include "FastFrames/Variable.h"
#include "FastFrames/StringOperations.h"


#include <string>
#include <vector>
#include <memory>

/**
 * @brief Wrapper around variable class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 */
class VariableWrapper   {
    public:
        VariableWrapper() = delete;

        explicit VariableWrapper(const std::string& name)  : m_variable(std::make_shared<Variable>(name)) {};


        ~VariableWrapper() = default;

        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_variable);
        }

        void constructFromSharedPtr(unsigned long long int variable_shared_ptr_int) {
            m_variable = *reinterpret_cast<std::shared_ptr<Variable> *>(variable_shared_ptr_int);
        };

        void constructFromRawPtr(unsigned long long int variable_raw_ptr_int) {
            Variable *variable = reinterpret_cast<Variable *>(variable_raw_ptr_int);
            m_variable = std::make_shared<Variable>(*variable);
        };

        std::string name() const {
            return m_variable->name();
        };


        void setDefinition(const std::string& definition) {
            m_variable->setDefinition(definition);
        };

        std::string definition() const {
            return m_variable->definition();
        };


        void setTitle(const std::string& title) {
            m_variable->setTitle(title);
        };

        std::string title() const {
            return m_variable->title();
        };


        void setBinningRegular(const double min, const double max, const int nbins) {
            m_variable->setBinning(min, max, nbins);
        };

        void setBinningIrregular(const std::string& binning_string) {
            const std::vector<double> bin_edges = StringOperations::convertStringTo<std::vector<double>>(binning_string);
            m_variable->setBinning(bin_edges);
        };


        bool hasRegularBinning() const {
            return m_variable->hasRegularBinning();
        };


        std::vector<double> binEdges() const {
            return m_variable->binEdges();
        };

        std::string binEdgesString() const {
            const std::vector edges = m_variable->binEdges();
            std::string result = "";
            for (const auto& edge : edges) {
                result += std::to_string(edge) + ",";
            }
            if (result.size() > 0) {
                result.pop_back();
            }
            return result;
        };


        double axisMin() const {
            return m_variable->axisMin();
        };

        double axisMax() const {
            return m_variable->axisMax();
        };

        int axisNbins() const {
            return m_variable->axisNbins();
        };

    private:
        std::shared_ptr<Variable> m_variable;

};