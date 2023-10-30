#pragma once

#include "FastFrames/MainFrame.h"

#include <memory>

class MainFrameWrapper {
    public:
        MainFrameWrapper()  : m_mainFrame(std::make_shared<MainFrame>()) {};

        void setConfig(long long unsigned int config_ptr) {
            const std::shared_ptr<ConfigSetting> *configPtr = reinterpret_cast<const std::shared_ptr<ConfigSetting>*>(config_ptr);
            m_mainFrame->setConfig(*configPtr);
        }

        void init() {
            m_mainFrame->init();
        }

        void executeHistograms() {
            m_mainFrame->executeHistograms();
        }

    private:
        std::shared_ptr<MainFrame> m_mainFrame;
};