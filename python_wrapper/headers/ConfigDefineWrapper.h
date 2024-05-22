/**
 * @file ConfigDefineWrapper.h
 * @brief Header file for the ConfigDefineWrapper class
 *
 */
#pragma once

#include "FastFrames/ConfigDefine.h"


#include <string>
#include <vector>
#include <memory>
#include <tuple>

/**
 * @brief Wrapper around ConfigDefine class, to be able to use it in python
 */
class ConfigDefineWrapper {
    public:
        /**
         * @brief Construct a new Config Define Wrapper object including the tree name (for truth trees)
         *
         * @param columnName
         * @param formula
         * @param treeName
         */
        ConfigDefineWrapper(const std::string& columnName,
                            const std::string& formula,
                            const std::string& treeName) :  m_configDefine(std::make_shared<ConfigDefine>(columnName, formula, treeName)) {};

        /**
         * @brief Destroy the ConfigDefineWrapper object
         *
         */
        ~ConfigDefineWrapper() = default;


        /**
         * @brief Get raw pointer to the shared pointer of configDefine object
         *
         * @return long long int - pointer to the config setting object
         */
        long long int getPtr() {
            return reinterpret_cast<long long int>(&m_configDefine);
        }

        /**
         * @brief Build ConfigDefineWrapper object from shared_ptr
         *
         * @param unsigned long long int - shared_ptr
         */
        void constructFromSharedPtr(unsigned long long int shared_ptr)  {
            m_configDefine = *reinterpret_cast<std::shared_ptr<ConfigDefine> *>(shared_ptr);
        };

        /**
         * @brief Build ConfigDefineWrapper object from raw pointer
         *
         * @param unsigned long long int - raw pointer to ConfigDefine object
         */
        void constructFromRawPointer(unsigned long long int raw_pointer)  {
            m_configDefine = std::make_shared<ConfigDefine>(*reinterpret_cast<ConfigDefine *>(raw_pointer));
        };

        /**
         * @brief Get new column name
         *
         * @return const std::string
         */
        inline const std::string columnName() const {
            return m_configDefine->columnName();
        };

        /**
         * @brief Get the formula for define
         *
         * @return const std::string
         */
        inline const std::string formula() const {
            return m_configDefine->formula();
        };

        /**
         * @brief Get the tree name
         *
         * @return const std::string
         */
        inline const std::string treeName() const {
            return m_configDefine->treeName();
        };


    private:
        std::shared_ptr<ConfigDefine> m_configDefine;

};