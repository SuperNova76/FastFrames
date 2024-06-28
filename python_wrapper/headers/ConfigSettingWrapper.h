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

        bool useRVec() const {
            return m_configSetting->useRVec();
        }

        void setUseRVec(bool useRVec) {
            m_configSetting->setUseRVec(useRVec);
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
         * @brief Get vector of raw pointer to shared pointers to region objects
         *
         * @return std::vector<unsigned long long int>
         */
        std::vector<unsigned long long int> getRegionsSharedPtr() const {
            const std::vector<std::shared_ptr<Region>> &regions = m_configSetting->regions();
            std::vector<unsigned long long int> regions_ptr(regions.size());
            for (unsigned int i = 0; i < regions.size(); ++i) {
                regions_ptr.at(i) = reinterpret_cast<unsigned long long int>(&regions.at(i));
            }
            return regions_ptr;
        }

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
         * @brief Get vector of raw pointer to shared pointers to sample objects
         *
         * @return std::vector<unsigned long long int>
         */
        std::vector<unsigned long long int> getSamplesSharedPtr() const {
            const std::vector<std::shared_ptr<Sample>> &samples = m_configSetting->samples();
            std::vector<unsigned long long int> samples_ptr(samples.size());
            for (unsigned int i = 0; i < samples.size(); ++i) {
                samples_ptr.at(i) = reinterpret_cast<unsigned long long int>(&samples.at(i));
            }
            return samples_ptr;
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
         * @brief Get vector of raw pointer to shared pointers to systematic objects
         *
         * @return std::vector<unsigned long long int>
         */
        std::vector<unsigned long long int> getSystematicsSharedPtr() const {
            const std::vector<std::shared_ptr<Systematic>> &systematics = m_configSetting->systematics();
            std::vector<unsigned long long int> systematics_ptr(systematics.size());
            for (unsigned int i = 0; i < systematics.size(); ++i) {
                systematics_ptr.at(i) = reinterpret_cast<unsigned long long int>(&systematics.at(i));
            }
            return systematics_ptr;
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

        /**
         * @brief Get the Ntuple shared pointer
         *
         * @return unsigned long long int
         */
        unsigned long long int getNtupleSharedPtr() const {
            const std::shared_ptr<Ntuple> &ntuple = m_configSetting->ntuple();
            return reinterpret_cast<unsigned long long int>(&ntuple);
        }

        /**
         * @brief Set the minimum event count to be processed
         *
         * @param i
         */
        void setMinEvent(const long long int i) {m_configSetting->setMinEvent(i);}
        /**
         * @brief Get the min event index
         *
         * @return long long int
         */
        long long int minEvent() const {return m_configSetting->minEvent();}

        /**
         * @brief Get the max event index
         *
         * @return long long int
         */
        long long int maxEvent() const {return m_configSetting->maxEvent();}

        /**
         * @brief Set the maximum event count to be processed
         *
         * @param i
         */
        void setMaxEvent(const long long int i) {m_configSetting->setMaxEvent(i);}

        /**
         * @brief Set the total number of job splittings for submitting
         *
         * @param number
         */
        void setTotalJobSplits(const int number) {m_configSetting->setTotalJobSplits(number);}

        /**
         * @brief Get the total number of job splittings for submitting
         *
         * @return int
         */
        int totalJobSplits() const {return m_configSetting->totalJobSplits();}

        /**
         * @brief Set the current job index
         *
         * @param number
         */
        void setCurrentJobIndex(const int number) { m_configSetting->setCurrentJobIndex(number);}

        /**
         * @brief Get the current job index
         *
         * @return int
         */
        int currentJobIndex() const {return m_configSetting->currentJobIndex();}

        /**
         * @brief Set the flag to force acceptance and selection to be within 0 and 1
         *
         * @param flag
         */
        inline void setCapAcceptanceSelection(const bool flag) {m_configSetting->setCapAcceptanceSelection(flag);}

        /**
         * @brief Force acceptance and selection to be within 0 and 1?
         *
         * @return true
         * @return false
         */
        inline bool capAcceptanceSelection() const {return m_configSetting->capAcceptanceSelection();}

        /**
         * @brief Add custom setting option
         *
         * @param std::string key, std::string value
         */
        void addOption(const std::string& key, const std::string& value) {
            m_configSetting->customOptions().addOption(key, value);
        };

        /**
         * @brief Get custom setting option
         *
         * @param std::string key
         * @return std::string
         */
        std::string getOption(const std::string& key) const {
            return m_configSetting->customOptions().getOption(key);
        };

        /**
         * @brief Get custom setting option
         *
         * @param std::string key, std::string default_value
         * @return std::string
         */
        std::string getOptionWithDefault(const std::string& key, const std::string& default_value) const {
            return m_configSetting->customOptions().getOption(key, default_value);
        };

        /**
         * @brief Check if custom setting option is defined
         *
         * @param std::string key
         * @return bool
         */
        bool hasOption(const std::string& key) const {
            return m_configSetting->customOptions().hasOption(key);
        };

        /**
         * @brief Get vector of all custom keys
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> getKeys() const {
            return m_configSetting->customOptions().getKeys();
        };

        /**
         * @brief Set the configDefineAfterCustomClass flag
         *
         * @param flag
         */
        void setConfigDefineAfterCustomClass(const bool flag) { m_configSetting->setConfigDefineAfterCustomClass(flag);}

        /**
         * @brief configDefineAfterCustomClass flag
         *
         * @return true
         * @return false
         */
        bool configDefineAfterCustomClass() const {return m_configSetting->configDefineAfterCustomClass();}

        /**
         * @brief Set the Use Region Subfolders object
         *
         * @param flag
         */
        inline void setUseRegionSubfolders(const bool flag) {m_configSetting->setUseRegionSubfolders(flag);}

        /**
         * @brief Use region subfolders?
         *
         * @return true
         * @return false
         */
        inline bool useRegionSubfolders() const {return m_configSetting->useRegionSubfolders();}

    private:
        std::shared_ptr<ConfigSetting> m_configSetting;
};
