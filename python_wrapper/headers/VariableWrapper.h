/**
 * @file VariableWrapper.h
 * @brief Header file for the VariableWrapper class
 *
 */
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

        /**
         * @brief Construct a new VariableWrapper object given the name of the variable
         *
         * @param std::string - name
         */
        explicit VariableWrapper(const std::string& name)  : m_variable(std::make_shared<Variable>(name)) {};

        /**
         * @brief Destroy the VariableWrapper object
         *
         */
        ~VariableWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Variable>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_variable);
        }

        /**
         * @brief Construct wrapper around the Variable object, given the raw pointer to shared_ptr<Variable>
         *
         * @param long long int - variable_shared_ptr_int
         */
        void constructFromSharedPtr(unsigned long long int variable_shared_ptr_int) {
            m_variable = *reinterpret_cast<std::shared_ptr<Variable> *>(variable_shared_ptr_int);
        };

        /**
         * @brief Construct wrapper around the Variable object, given the raw pointer to Variable
         *
         * @param long long int - variable_raw_ptr_int
         */
        void constructFromRawPtr(unsigned long long int variable_raw_ptr_int) {
            Variable *variable = reinterpret_cast<Variable *>(variable_raw_ptr_int);
            m_variable = std::make_shared<Variable>(*variable);
        };

        /**
         * @brief Get name of the variable
         *
         * @return std::string - name
         */
        std::string name() const {
            return m_variable->name();
        };

        /**
         * @brief Set definition of the variable string
         *
         * @param std::string - variable definition (formula)
         */
        void setDefinition(const std::string& definition) {
            m_variable->setDefinition(definition);
        };

        /**
         * @brief Get variable definition (formula)
         *
         * @return std::string - variable definition (formula)
         */
        std::string definition() const {
            return m_variable->definition();
        };

        /**
         * @brief Set the title of the variable
         *
         * @param std::string - title
         */
        void setTitle(const std::string& title) {
            m_variable->setTitle(title);
        };

        /**
         * @brief Get the title of the variable
         *
         * @return std::string - title
         */
        std::string title() const {
            return m_variable->title();
        };

        /**
         * @brief Set the regular binning of the histogram
         *
         * @param double - minimum of the x-axis
         * @param double - maximum of the x-axis
         * @param nbins - number of bins
         */
        void setBinningRegular(const double min, const double max, const int nbins) {
            m_variable->setBinning(min, max, nbins);
        };

        /**
         * @brief Set irregular binning given the bin edges
         *
         * @param std::vector<double> - bin edges
         */
        void setBinningIrregular(const std::vector<double> &bin_edges) {
            m_variable->setBinning(bin_edges);
        };

        /**
         * @brief Has regular binning?
         *
         * @return true
         * @return false
         */
        bool hasRegularBinning() const {
            return m_variable->hasRegularBinning();
        };

        /**
         * @brief Get vector<double> of bin edges
         *
         * @return std::vector<double>
         */
        std::vector<double> binEdges() const {
            return m_variable->binEdges();
        };

        /**
         * @brief Get bin edges as string, separated by comma
         *
         * @return std::string
         */
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

        /**
         * @brief Get minimum of the x-axis
         *
         * @return double
         */
        double axisMin() const {
            return m_variable->axisMin();
        };

        /**
         * @brief Get maximum of the x-axis
         *
         * @return double
         */
        double axisMax() const {
            return m_variable->axisMax();
        };

        /**
         * @brief Get number of bins
         *
         * @return int
         */
        int axisNbins() const {
            if (m_variable->hasRegularBinning())    {
                return m_variable->axisNbins();
            }
            return m_variable->binEdges().size() -1;
        };

    private:
        std::shared_ptr<Variable> m_variable;

};