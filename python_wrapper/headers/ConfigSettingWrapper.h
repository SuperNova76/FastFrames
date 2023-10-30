#pragma once

#include "FastFrames/ConfigSetting.h"

#include <memory>


/**
 * @brief Wrapper around config setting class, to be able to use it in python
 * Wrapper cannot return references or custom classes.
 *
 */
class ConfigSettingWrapper {
    public:
        ConfigSettingWrapper() :  m_configSetting(std::make_shared<ConfigSetting>())    {};

        ~ConfigSettingWrapper() = default;


        long long int getPtr() {
            return reinterpret_cast<long long int>(&m_configSetting);
        }


        std::string outputPath() const  {
            return m_configSetting->outputPath();
        };

        void setOutputPath(const std::string& outputPath) {
            m_configSetting->setOutputPath(outputPath);
        };


        std::string inputPath() const   {
            return m_configSetting->inputPath();
        };

        void setInputPath(const std::string &inputPath) {
            m_configSetting->setInputPath(inputPath);
        };


        std::string inputFilelistPath() const   {
            return m_configSetting->inputFilelistPath();
        };

        void setInputFilelistPath(const std::string &inputFilelistPath) {
            m_configSetting->setInputFilelistPath(inputFilelistPath);
        };


        std::string inputSumWeightsPath() const {
            return m_configSetting->inputSumWeightsPath();
        };

        void setInputSumWeightsPath(const std::string &inputSumWeightsPath) {
            m_configSetting->setInputSumWeightsPath(inputSumWeightsPath);
        };


        std::string customFrameName() const {
            return m_configSetting->customFrameName();
        };

        void setCustomFrameName(const std::string &customFrameName) {
            m_configSetting->setCustomFrameName(customFrameName);
        };


        int numCPU() const {
            return m_configSetting->numCPU();
        };

        void setNumCPU(int numCPU) {
            m_configSetting->setNumCPU(numCPU);
        };


        void addLuminosityInformation(const std::string& campaign, const float luminosity) {
            m_configSetting->addLuminosityInformation(campaign, luminosity);
        };

        float getLuminosity(const std::string& campaign) const {
            return m_configSetting->getLuminosity(campaign);
        };


        // regions
        void addRegion(long long int region_shared_ptr_int) {
            const std::shared_ptr<Region> *region = reinterpret_cast<std::shared_ptr<Region> *>(region_shared_ptr_int);
            m_configSetting->addRegion(*region);
        };


    private:
        std::shared_ptr<ConfigSetting> m_configSetting;
};