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

class SumWeightsGetter {
    public:
        explicit SumWeightsGetter(const std::vector<std::string> &filelist) {
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


        ~SumWeightsGetter() = default;

        std::vector<std::string> getSumWeightsNames() const {
            return m_sumWeightsNames;
        };

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
                    if (map_uninitialized)  {
                        m_sumWeightsMap[variation_name] = hist->GetBinContent(2);
                    }
                    else    {
                        m_sumWeightsMap[variation_name] += hist->GetBinContent(2);
                    }
                }
            }
        };

        static std::string get_variation_name(const std::string &histogram_name)   {
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

        std::vector<std::string> m_sumWeightsNames;
        std::vector<double>      m_sumWeightsValues;

        std::map<std::string, double> m_sumWeightsMap;

};