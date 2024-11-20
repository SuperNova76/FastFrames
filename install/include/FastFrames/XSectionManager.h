/**
 * @file XSectionManager.h
 * @brief Handling cross sections of the samples
 *
 */

#pragma once

#include <vector>
#include <string>
#include <map>


enum class XSectionFileType {
    TopDataPreparation,
    PMG,
    Unknown
};

struct XSectionData {
    double xSection;
    int eTag;
};

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
         * @param usedDSIDs
         */
        explicit XSectionManager(const std::vector<std::string> &xSectionFiles, const std::vector<int> &usedDSIDs);

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

        std::map<int, XSectionData> m_xSectionMap; // DSID -> (x-section, e-tag)

        std::vector<int> m_usedDSIDs;

        XSectionFileType getFileType(const std::string &xSectionFile) const;

        bool validLine(const std::string &line) const;

        void readXSectionFile(const std::string &xSectionFile);

        void processTopDataPreparationLine(const std::string &line);

        void processPMGLine(const std::string &line);

        static bool validTopDataPreparationFileColumns(const std::vector<std::string> &columns);

        static bool validPMGFileColumns(const std::vector<std::string> &columns);

        static constexpr double m_xsection_difference_tolerance = 1e-3;

};