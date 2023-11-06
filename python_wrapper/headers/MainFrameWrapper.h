/**
 * @file MainFrameWrapper.h
 * @brief Header file for the MainFrameWrapper class
 *
 */

#pragma once

#include "FastFrames/MainFrame.h"

#include <memory>

/**
 * @brief Wrapper class for MainFrame.
 *
 * This class provides a C++ interface for the MainFrame class which can be later used from python.
 * It allows setting the configuration and executing histograms.
 */
class MainFrameWrapper {
    public:
        /**
         * @brief Constructor for MainFrameWrapper.
         *
         * Initializes the MainFrame object.
         */
        MainFrameWrapper()  : m_mainFrame(std::make_shared<MainFrame>()) {};

        /**
         * @brief Sets the config file class
         *
         * @param config_ptr Raw pointer to the shared pointer where configSettings object is stored.
         */
        void setConfig(long long unsigned int config_ptr) {
            const std::shared_ptr<ConfigSetting> *configPtr = reinterpret_cast<const std::shared_ptr<ConfigSetting>*>(config_ptr);
            m_mainFrame->setConfig(*configPtr);
        }

        /**
         * @brief Initializes the MainFrame object.
         *
         */
        void init() {
            m_mainFrame->init();
        }

        /**
         * @brief Executes the histogram generation process.
         *
         */
        void executeHistograms() {
            m_mainFrame->executeHistograms();
        }

    private:
        std::shared_ptr<MainFrame> m_mainFrame;
};