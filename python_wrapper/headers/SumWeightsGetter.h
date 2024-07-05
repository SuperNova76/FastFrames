#pragma once

#include "FastFrames/Logger.h"
#include "FastFrames/StringOperations.h"

#include <TFile.h>
#include <TKey.h>
#include <TH1F.h>

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

/**
 * @brief Class for calculating sum of weights from all files in the given vector of files
 *
 */
class SumWeightsGetter {
    public:
        /**
         * @brief Construct a new SumWeightsGetter object - it will calculate sum of weights from all files in the given vector of files
         *
         * @param filelist Vector of files to read
         */
        explicit SumWeightsGetter(const std::vector<std::string> &filelist, const std::string &histo_name) :
            m_histo_name(histo_name) {
            for (const auto &filename : filelist) {
                LOG(INFO) << "SumWeightsGetter: Reading file " + filename + "\n";
                readFile(filename);
            }

            for (const auto &entry : m_sumWeightsMap) {
                const std::string name = entry.first;
                double value = entry.second;

                if (value != value || value <= 0 ) {
                    LOG(WARNING) << "Suspicious sum of weights value for variation" + name + ", setting it to -1!\n";
                    value = -1;
                }

                m_sumWeightsNames.push_back(name);
                m_sumWeightsValues.push_back(value);
            }
        };

        SumWeightsGetter() = delete;

        ~SumWeightsGetter() = default;

        /**
         * @brief Get names of sum weights elements
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> getSumWeightsNames() const {
            return m_sumWeightsNames;
        };

        /**
         * @brief Get the Sum Weights values
         *
         * @return std::vector<double>
         */
        std::vector<double>      getSumWeightsValues() const    {
            return m_sumWeightsValues;
        };

    private:
        void readFile(const std::string &filename)  {
            const bool map_uninitialized = m_sumWeightsMap.empty();
            std::unique_ptr<TFile> file(TFile::Open(filename.c_str(), "READ"));
            if (!file) {
                LOG(ERROR) << "SumWeightsGetter::readFile: Could not open file " + filename + "\n";
                throw std::runtime_error("Could not open file " + filename);
            }

            TIter next(file->GetListOfKeys());
            TKey *key;
            std::size_t number_of_histograms = 0;
            while ((key = static_cast<TKey*>(next()))) {
                TObject *obj = key->ReadObj();
                const std::string name = obj->GetName();
                if (obj->ClassName() == std::string("TH1F")) {
                    TH1F *hist = static_cast<TH1F*>(file->Get(name.c_str()));
                    const std::string variation_name = get_variation_name(name);
                    if (variation_name == "")   {
                        continue;
                    }
                    const float sum_weights_value = hist->GetBinContent(2);
                    if (sum_weights_value != sum_weights_value || sum_weights_value <= 0 ) {
                        LOG(WARNING) << "Suspicious sum of weights value for variation" + variation_name + " in file " + filename + "\n";
                    }
                    number_of_histograms++;
                    if (map_uninitialized)  {
                        m_sumWeightsMap[variation_name] = hist->GetBinContent(2);
                    }
                    else    {
                        m_sumWeightsMap[variation_name] += hist->GetBinContent(2);
                    }
                }
            }

            if (number_of_histograms != m_sumWeightsMap.size()) {
                const std::string message = "Set of cutflow histograms in " + filename + " file differs from other those in other ROOT files of the same sample!";
                LOG(ERROR) << message << "\n";
                throw std::runtime_error(message);
            }
        };

        std::string get_variation_name(const std::string &histogram_name)   {
            if (m_histo_name == "") {
                return get_variation_name_default(histogram_name);
            }
            return get_variation_name_custom(histogram_name);
        }

        std::string get_variation_name_default(const std::string &histogram_name)   {
            if (!StringOperations::stringStartsWith(histogram_name, "CutBookkeeper_"))   {
                return "";
            }
            std::vector<std::string> elements = StringOperations::splitString(histogram_name, "_");
            if (elements.size() < 4)    {
                return "";
            }
            if (!StringOperations::stringIsInt(elements[1]) || !StringOperations::stringIsInt(elements[2]))    {
                return "";
            }

            elements.erase(elements.begin(), elements.begin()+3);
            return StringOperations::joinStrings("_", elements);
        }

        std::string get_variation_name_custom(const std::string &histogram_name)   {
            const std::string prefix = m_histo_name + "_";
            const int prefix_length = prefix.length();
            if (!StringOperations::stringStartsWith(histogram_name, prefix))   {
                return "";
            }
            return histogram_name.substr(prefix_length);
        }

        std::vector<std::string> m_sumWeightsNames;
        std::vector<double>      m_sumWeightsValues;
        std::string m_histo_name = "";

        std::map<std::string, double> m_sumWeightsMap;

};
