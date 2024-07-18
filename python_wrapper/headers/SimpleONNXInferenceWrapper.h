/**
 * @file SimpleONNXInference.h
 * @brief Header file for the SimpleONNXInference class
 *
 */
#pragma once

#include "FastFrames/SimpleONNXInference.h"


#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include <boost/python.hpp>

template <typename T>
std::vector<T> pyListToStdVector(const boost::python::object& iterable) {
  return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
                        boost::python::stl_input_iterator<T>());
}

/**
 * @brief Wrapper around SimpleONNXInferenceWrapper class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 */
class SimpleONNXInferenceWrapper {
    public:
        SimpleONNXInferenceWrapper() = delete;

        /**
         * @brief Construct a new SimpleONNXInferenceWrapper Wrapper object with given name
         *
         * @param name
         */
        explicit SimpleONNXInferenceWrapper(const std::string& name) : m_inference(std::make_shared<SimpleONNXInference>(name)) {};

        /**
         * @brief Destroy the SimpleONNXInferenceWrapper Wrapper object
         *
         */
        ~SimpleONNXInferenceWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<SimpleONNXInferenceWrapper>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_inference);
        }

        /**
         * @brief Build SimpleONNXInferenceWrapper object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_inference = *reinterpret_cast<std::shared_ptr<SimpleONNXInference> *>(shared_ptr);
        };

        /**
         * @brief Get name of the SimpleONNXInferenceWrapper
         *
         * @return std::string
         */
        std::string name() const {
            return m_inference->name();
        };

        void setModelPaths(const boost::python::object& model_paths) {
            m_inference->setModelPaths(pyListToStdVector<std::string>(model_paths));
        }

        inline std::vector<std::string> modelPaths() const {
            return m_inference->modelPaths();
        }

        void addInput(const std::string& layer_name, const boost::python::object& branch_names) {
            m_inference->addInput(layer_name, pyListToStdVector<std::string>(branch_names));
        }

        std::vector<std::string> getInputLayerNames() {
            const std::map<std::string, std::vector<std::string>> &inputLayersMap = m_inference->getInputMap();
            std::vector<std::string> result;
            for (auto const& [layer_name, branches] : inputLayersMap) {
                result.push_back(layer_name);
            }
            return result;
        }

        std::vector<std::string> getInputLayerBranches(const std::string& layer_name) {
            const std::map<std::string, std::vector<std::string>> &inputLayersMap = m_inference->getInputMap();
            if (inputLayersMap.find(layer_name) == inputLayersMap.end()) {
                return {};
            }
            return inputLayersMap.at(layer_name);
        }

        void addOutput(const std::string& layer_name, const boost::python::object& branch_names) {
            m_inference->addOutput(layer_name, pyListToStdVector<std::string>(branch_names));
        }

        std::vector<std::string> getOutputLayerNames() {
            const std::map<std::string, std::vector<std::string>> &outputLayersMap = m_inference->getOutputMap();
            std::vector<std::string> result;
            for (auto const& [layer_name, branches] : outputLayersMap) {
                result.push_back(layer_name);
            }
            return result;
        }

        std::vector<std::string> getOutputLayerBranches(const std::string& layer_name) {
            const std::map<std::string, std::vector<std::string>> &outputLayersMap = m_inference->getOutputMap();
            if (outputLayersMap.find(layer_name) == outputLayersMap.end()) {
                return {};
            }
            return outputLayersMap.at(layer_name);
        }

        void setFoldFormula(const std::string& foldFormula) {
            m_inference->setFoldFormula(foldFormula);
        }

    private:
        std::shared_ptr<SimpleONNXInference> m_inference;

};