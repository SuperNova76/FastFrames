/**
 * @file XSectionManager.h
 * @brief Handling cross sections of the samples
 *
 */

#pragma once

#include <vector>
#include <string>
#include <map>

/**
 * @brief Class responsible for handling cross sections of the samples
 *
 */
class XSectionManager {
    public:
        /**
         * @brief Deleted default constructor of XSectionManager object
         *
         */
        XSectionManager() = delete;

        /**
         * @brief Construct a new XSectionManager object
         *
         * @param xSectionFiles
         */
        XSectionManager(const std::vector<std::string> &xSectionFiles);

        /**
         * @brief Destroy the XSectionManager object
         *
         */
        ~XSectionManager() = default;

        /**
         * @brief Get the total x-section of a given sample (i.e. x-section times k-factor), throw exception if not found
         *
         * @param sampleDSID
         * @return double
         */
        double xSection(const int sampleDSID) const;

    private:
        void readXSectionFile(const std::string &xSectionFile);

        void processLine(const std::string &line);

        bool validLine(const std::string &line) const;

        std::map<int, double> m_xSectionMap;
};