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
        explicit FastFramesExecutorWrapper(long long unsigned int config_shared_ptr)    {
            const std::shared_ptr<ConfigSetting> *config = reinterpret_cast<std::shared_ptr<ConfigSetting>*>(config_shared_ptr);
            m_executor = std::make_shared<FastFramesExecutor>(*config);
        };

        ~FastFramesExecutorWrapper();

        void runFastFrames() const  { m_executor->runFastFrames(); };

    private:
        std::shared_ptr<FastFramesExecutor> m_executor;

};