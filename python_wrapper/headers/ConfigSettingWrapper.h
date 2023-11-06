/**
 * @file ConfigSettingWrapper.h
 * @brief Header file for the ConfigSettingWrapper class
 *
 */

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
        /**
         * @brief Construct a new Config Setting Wrapper object
         *
         */
        ConfigSettingWrapper() :  m_configSetting(std::make_shared<ConfigSetting>())    {};

        /**
         * @brief Destroy the Config Setting Wrapper object
         *
         */
        ~ConfigSettingWrapper() = default;

        /**
         * @brief Get raw pointer to the shared pointer of ConfigSetting object
         *
         * @return long long int - pointer to the config setting object
         */
        long long int getPtr() {
            return reinterpret_cast<long long int>(&m_configSetting);
        }

        /**
         * @brief Get output path for histograms
         *
         * @return std::string - output path for histograms
         */
        std::string outputPathHistograms() const  {
            return m_configSetting->outputPathHistograms();
        };

        /**
         * @brief Set output path for histograms
         *
         * @param outputPathHistograms - output path for histograms
         */
        void setOutputPathHistograms(const std::string& outputPathHistograms) {
            m_configSetting->setOutputPathHistograms(outputPathHistograms);
        };

        /**
         * @brief Get output path for ntuples
         *
         * @return std::string - output path for ntuples
         */
        std::string outputPathNtuples() const  {
            return m_configSetting->outputPathNtuples();
        };

        /**
         * @brief Set output path for ntuples
         *
         * @param outputPathNtuples - output path for ntuples
         */
        void setOutputPathNtuples(const std::string& outputPathNtuples) {
            m_configSetting->setOutputPathNtuples(outputPathNtuples);
        };

        /**
         * @brief Get path to the filelist
         *
         * @return std::string - path to the filelist
         */
        std::string inputFilelistPath() const   {
            return m_configSetting->inputFilelistPath();
        };

        /**
         * @brief Set path to the filelist
         *
         * @param inputFilelistPath - path to the filelist
         */
        void setInputFilelistPath(const std::string &inputFilelistPath) {
            m_configSetting->setInputFilelistPath(inputFilelistPath);
        };

        /**
         * @brief Get path to the sum weights file
         *
         * @return std::string
         */
        std::string inputSumWeightsPath() const {
            return m_configSetting->inputSumWeightsPath();
        };

        /**
         * @brief Set the Input Sum Weights Path object
         *
         * @param inputSumWeightsPath
         */
        void setInputSumWeightsPath(const std::string &inputSumWeightsPath) {
            m_configSetting->setInputSumWeightsPath(inputSumWeightsPath);
        };

        /**
         * @brief Name of the custom frame shared object to be used
         *
         * @return std::string
         */
        std::string customFrameName() const {
            return m_configSetting->customFrameName();
        };

        /**
         * @brief Set the Custom Frame Name object
         *
         * @param customFrameName
         */
        void setCustomFrameName(const std::string &customFrameName) {
            m_configSetting->setCustomFrameName(customFrameName);
        };

        /**
         * @brief Get number of CPUs to run on
         *
         * @return int
         */
        int numCPU() const {
            return m_configSetting->numCPU();
        };

        /**
         * @brief Set number of CPUs to run on
         *
         * @param numCPU
         */
        void setNumCPU(int numCPU) {
            m_configSetting->setNumCPU(numCPU);
        };

        /**
         * @brief Add luminosity value for the campaign. If force is true, overwrite existing value, otherwise throw error if value is already defined
         *
         * @param campaign
         * @param luminosity
         * @param force
         */
        void addLuminosityInformation(const std::string& campaign, const float luminosity, const bool force) {
            m_configSetting->addLuminosityInformation(campaign, luminosity, force);
        };

        /**
         * @brief Get luminosity for the campaign
         *
         * @param campaign
         * @return float
         */
        float getLuminosity(const std::string& campaign) const {
            return m_configSetting->getLuminosity(campaign);
        };

        /**
         * @brief Is the luminosity for the campaign defined?
         *
         * @return bool
         */
        bool campaignIsDefined(const std::string& campaign) const {
            const std::map<std::string, float> luminosity_map = m_configSetting->luminosityMap();
            return luminosity_map.find(campaign) != luminosity_map.end();
        };

        /**
         * @brief Add text file with the cross-sections
         *
         * @param std::string - xsectionFile
         */
        void addXsectionFile(const std::string& xsectionFile) {
            m_configSetting->addXsectionFile(xsectionFile);
        };

        /**
         * @brief Get vector of xsection files
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> xSectionFiles() const {
            return m_configSetting->xSectionFiles();
        };


        /**
         * @brief Add TLorentzVector for the object with specified name (i.e. jet, el, mu, etc.)
         *
         * @param name
         */
        void addTLorentzVector(const std::string& name) {
            m_configSetting->addTLorentzVector(name);
        };

        /**
         * @brief Get vector of TLorentzVector names
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> tLorentzVectors() const  {
            return m_configSetting->tLorentzVectors();
        }

        /**
         * @brief Add region to the config setting
         *
         * @param region_shared_ptr_int - raw pointer to shared pointer with the region
         */
        void addRegion(long long int region_shared_ptr_int) {
            const std::shared_ptr<Region> *region = reinterpret_cast<std::shared_ptr<Region> *>(region_shared_ptr_int);
            m_configSetting->addRegion(*region);
        };

        /**
         * @brief Get the names of the variables defined in any of the region
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> getVariableNames() const {
            std::vector<std::string> variable_names;
            const std::vector<std::shared_ptr<Region>> regions = m_configSetting->regions();
            for (const auto& region : regions) {
                const std::vector<Variable> variables = region->variables();
                for (const auto& variable : variables) {
                    if (std::find(variable_names.begin(), variable_names.end(), variable.name()) == variable_names.end())  {
                        variable_names.push_back(variable.name());
                    }
                }
            }
            return variable_names;
        };

        /**
         * @brief Add sample to the config setting
         *
         * @param sample_shared_ptr_int - raw pointer to the shared pointer with the sample
         */
        void addSample(long long int sample_shared_ptr_int) {
            const std::shared_ptr<Sample> *sample = reinterpret_cast<std::shared_ptr<Sample> *>(sample_shared_ptr_int);
            m_configSetting->addSample(*sample);
        };

        /**
         * @brief Add systematic to the config setting
         *
         * @param systematic_shared_ptr_int - raw pointer to the shared pointer with the systematic
         */
        void addSystematic(long long int systematic_shared_ptr_int) {
            const std::shared_ptr<Systematic> *systematic = reinterpret_cast<std::shared_ptr<Systematic> *>(systematic_shared_ptr_int);
            m_configSetting->addSystematic(*systematic);
        };

        /**
         * @brief Set the Nominal Only object
         *
         * @param bool nominalOnly
         */
        void setNominalOnly(bool nominalOnly) {
            m_configSetting->setNominalOnly(nominalOnly);
        };

        /**
         * @brief Run nominal only?
         *
         * @return bool
         */
        bool nominalOnly() const {
            return m_configSetting->nominalOnly();
        };

        /**
         * @brief Set the Automatic Systematics flag
         *
         * @param automaticSystematics
         */
        void setAutomaticSystematics(bool automaticSystematics) {
            m_configSetting->setAutomaticSystematics(automaticSystematics);
        };

        /**
         * @brief Should the systematics be read automatically from the input ROOT file?
         *
         * @return true
         * @return false
         */
        bool automaticSystematics() const {
            return m_configSetting->automaticSystematics();
        };

        /**
         * @brief Clear list of the added systematics
         *
         */
        void clearSystematics() {
            m_configSetting->clearSystematics();
        };

        /**
         * @brief Set the Ntuple object
         *
         * @param ntuple_shared_ptr_int - raw pointer to the shared pointer with the ntuple
         */
        void setNtuple(long long int ntuple_shared_ptr_int) {
            const std::shared_ptr<Ntuple> *ntuple = reinterpret_cast<std::shared_ptr<Ntuple> *>(ntuple_shared_ptr_int);
            m_configSetting->setNtuple(*ntuple);
        };

    private:
        std::shared_ptr<ConfigSetting> m_configSetting;
};
