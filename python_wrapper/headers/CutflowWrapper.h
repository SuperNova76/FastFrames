/**
 * @file CutflowWrapper.h
 * @brief Header file for the CutflowWrapper class
 *
 */
#pragma once

#include "FastFrames/Cutflow.h"


#include <string>
#include <vector>
#include <memory>
#include <tuple>

/**
 * @brief Wrapper around Cutflow class, to be able to use it in python
 */
class CutflowWrapper {
    public:
        CutflowWrapper() = delete;

        /**
         * @brief Construct a new Cutflow Wrapper object with given name
         *
         * @param name
         */
        explicit CutflowWrapper(const std::string& name) : m_cutflow(std::make_shared<Cutflow>(name)) {};

        /**
         * @brief Destroy the Cutflow Wrapper object
         *
         */
        ~CutflowWrapper() = default;

        /**
         * @brief Get raw pointer to the shared_ptr<Cutflow>
         *
         * @return unsigned long long int
         */
        unsigned long long int getPtr() {
            return reinterpret_cast<unsigned long long int>(&m_cutflow);
        }

        /**
         * @brief Build Cutflow object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_cutflow = *reinterpret_cast<std::shared_ptr<Cutflow> *>(shared_ptr);
        };

        /**
         * @brief Get name of the cutflow
         *
         * @return std::string
         */
        std::string name() const {
            return m_cutflow->name();
        }

        /**
         * @brief Add a selection to the cutflow
         *
         * @param selection
         * @param title
         */
        void addSelection(const std::string& selection, const std::string& title) {
            m_cutflow->addSelection(selection, title);
        }

        /**
         * @brief Get the selections defintions
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> selectionsDefinition() const {
            std::vector<std::string> selections;
            for (auto& selection : m_cutflow->selections()) {
                selections.push_back(selection.first);
            }
            return selections;
        }

        /**
         * @brief Get the selections titles
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> selectionsTitles() const {
            std::vector<std::string> selections;
            for (auto& selection : m_cutflow->selections()) {
                selections.push_back(selection.second);
            }
            return selections;
        }


    private:
        std::shared_ptr<Cutflow> m_cutflow;

};