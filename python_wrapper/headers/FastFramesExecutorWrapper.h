/**
 * @file FastFramesExecutorWrapper.h
 * @brief Header file for the FastFramesExecutorWrapper class
 *
 */

#pragma once

#include "FastFrames/FastFramesExecutor.h"

#include <string>
#include <vector>
#include <memory>

/**
 * @brief Wrapper around FastFramesExecutor class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 */
class FastFramesExecutorWrapper {
    public:
        /**
         * @brief Construct a new FastFramesExecutorWrapper object
         *
         * @param config_shared_ptr - raw pointer to the shared pointer of the ConfigSetting object
         */
        explicit FastFramesExecutorWrapper(long long unsigned int config_shared_ptr) :
            m_executor(std::make_shared<FastFramesExecutor>(*(reinterpret_cast<std::shared_ptr<ConfigSetting>*>(config_shared_ptr))))   {};

        /**
         * @brief Destroy the Fast Frames Executor Wrapper object
         *
         */
        ~FastFramesExecutorWrapper() = default;

        /**
         * @brief Main function running the whole C++ code
         *
         */
        void runFastFrames() const  { m_executor->runFastFrames(); };

        /**
         * @brief Set if the output should be ntuple
         *
         * @param flag
         */
        void setRunNtuples(const bool flag) {m_executor->setRunNtuples(flag);};

        /**
         * @brief Should the output be ntuple?
         *
         * @return true
         * @return false
         */
        bool runNtuples() const {return m_executor->runNtuples();};

    private:
        std::shared_ptr<FastFramesExecutor> m_executor;

};