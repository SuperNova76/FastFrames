#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"

#include <memory>
#include <map>
#include <string>


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


        void addLuminosityInformation(const std::string& campaign, const float luminosity, const bool force) {
            m_configSetting->addLuminosityInformation(campaign, luminosity, force);
        };

        float getLuminosity(const std::string& campaign) const {
            return m_configSetting->getLuminosity(campaign);
        };

        bool campaignIsDefined(const std::string& campaign) const {
            const std::map<std::string, float> luminosity_map = m_configSetting->luminosityMap();
            return luminosity_map.find(campaign) != luminosity_map.end();
        };

        void addXsectionFile(const std::string& xsectionFile) {
            m_configSetting->addXsectionFile(xsectionFile);
        };

        unsigned int getNumberOfXsectionFiles() const {
            return (m_configSetting->xSectionFiles()).size();
        };

        std::string getXsectionFile(unsigned int i_file) const  {
            return (m_configSetting->xSectionFiles()).at(i_file);
        }


        // regions
        void addRegion(long long int region_shared_ptr_int) {
            const std::shared_ptr<Region> *region = reinterpret_cast<std::shared_ptr<Region> *>(region_shared_ptr_int);
            m_configSetting->addRegion(*region);
        };

        void addSample(long long int sample_shared_ptr_int) {
            const std::shared_ptr<Sample> *sample = reinterpret_cast<std::shared_ptr<Sample> *>(sample_shared_ptr_int);
            m_configSetting->addSample(*sample);
        };

        void addSystematic(long long int systematic_shared_ptr_int) {
            const std::shared_ptr<Systematic> *systematic = reinterpret_cast<std::shared_ptr<Systematic> *>(systematic_shared_ptr_int);
            m_configSetting->addSystematic(*systematic);
        };


        void setNominalOnly(bool nominalOnly) {
            m_configSetting->setNominalOnly(nominalOnly);
        };

        bool nominalOnly() const {
            return m_configSetting->nominalOnly();
        };


        void setAutomaticSystematics(bool automaticSystematics) {
            m_configSetting->setAutomaticSystematics(automaticSystematics);
        };

        bool automaticSystematics() const {
            return m_configSetting->automaticSystematics();
        };

        void clearSystematics() {
            m_configSetting->clearSystematics();
        };

    private:
        std::shared_ptr<ConfigSetting> m_configSetting;
};
