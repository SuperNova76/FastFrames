/**
 * @file XSectionManager.cc
 * @brief Code for extracting cross-sections
 *
 */

#include "FastFrames/XSectionManager.h"
#include "FastFrames/Logger.h"
#include "FastFrames/Utils.h"
#include "FastFrames/StringOperations.h"

#include <fstream>
#include <stdexcept>

using std::string, std::map, std::vector, std::ifstream;


XSectionManager::XSectionManager(const std::vector<std::string> &xSectionFiles)  {
    for (const string &xSectionFile : xSectionFiles) {
        readXSectionFile(xSectionFile);
    }
};

double XSectionManager::xSection(const int sampleDSID) const    {
    if (m_xSectionMap.find(sampleDSID) == m_xSectionMap.end()) {
        LOG(ERROR) << "Cannot find cross section for DSID: " << sampleDSID << ", please update your x-section file\n";
        throw std::runtime_error("");
    }
    return std::get<0>(m_xSectionMap.at(sampleDSID));
};

void XSectionManager::readXSectionFile(const std::string &xSectionFile) {
    const XSectionFileType file_type = getFileType(xSectionFile);

    void(XSectionManager::*read_function)(const std::string &) = nullptr;
    if (file_type == XSectionFileType::TopDataPreparation) {
        read_function = &XSectionManager::processTopDataPreparationLine;
    }
    else if (file_type == XSectionFileType::PMG) {
        read_function = &XSectionManager::processPMGLine;
    }
    else {
        LOG(ERROR) << "Unsupported x-section file type\n";
        throw std::runtime_error("");
    }

    string line;
    ifstream input_file (xSectionFile);
    if (input_file.is_open())    {
        while ( getline (input_file,line) )        {
            (this->*read_function)(line);
        }
        input_file.close();
    }
    else    {
        LOG(ERROR) << "Unable to open x-section file \"" << xSectionFile << "\"\n";
        throw std::runtime_error("");
    }
};

void XSectionManager::processTopDataPreparationLine(const std::string &line)  {
    // For now just Top Data Preparation style x-section file is supported
    if (!validLine(line))   {
        return;
    }

    const vector<string> elements = StringOperations::splitByWhitespaces(line);
    const bool valid_line = validTopDataPreparationFileColumns(elements);

    if (!valid_line) {
        LOG(ERROR) << ("Invalid line in x-section text file: \"" + line + "\"\n");
        throw std::runtime_error("");
    }

    const int       dsid = std::stoi(elements.at(0));
    const double    xsec = std::stod(elements.at(1));
    const double    kfac = std::stod(elements.at(2));

    // Check if the sample is not defined multiple times in the x-section text file
    if (m_xSectionMap.find(dsid) != m_xSectionMap.end())    {
        LOG(ERROR) << ("The following dsid was found multiple times in the x-section text files: \"" + elements.at(0) + "\"\n");
        throw std::runtime_error("");
    }
    m_xSectionMap[dsid] = std::tuple<double,int>(xsec * kfac,0);
};

XSectionFileType XSectionManager::getFileType(const std::string &xSectionFile) const {
    string line;
    ifstream input_file (xSectionFile);
    if (input_file.is_open())    {
        while ( getline (input_file,line) )        {
            if (!validLine(line))   {
                continue;
            }
            const std::vector<std::string> elements = StringOperations::splitByWhitespaces(line);
            if (validTopDataPreparationFileColumns(elements))   {
                return XSectionFileType::TopDataPreparation;
            }
            else if (validPMGFileColumns(elements)) {
                return XSectionFileType::PMG;
            }
            else if (StringOperations::stringStartsWith(line, "dataset_number/I:physics_short/C:crossSection_pb/D:genFiltEff/D:kFactor/D:relUncertUP/D:relUncertDOWN/D:generator_name/C:etag/C"))   {
                return XSectionFileType::PMG;
            }
            else {
                return XSectionFileType::Unknown;
            }
        }
        input_file.close();
        return XSectionFileType::Unknown;
    }
    else    {
        LOG(ERROR) << "Unable to open x-section file \"" << xSectionFile << "\"\n";
        throw std::runtime_error("");
    }
};

bool XSectionManager::validLine(const std::string &line) const  {
    string strippedString = line;
    StringOperations::stripString(&strippedString);

    if (strippedString.size() == 0)    {
        return false;
    }

    return strippedString.at(0) != '#';
};

void XSectionManager::processPMGLine(const std::string &line)    {
    // For now just Top Data Preparation style x-section file is supported
    if (!validLine(line))   {
        return;
    }

    // skip header
    if (StringOperations::stringStartsWith(line, "dataset_number/I"))    {
        return;
    }

    const vector<string> elements = StringOperations::splitByWhitespaces(line);

    const bool line_is_valid =  validPMGFileColumns(elements);

    if (!line_is_valid) {
        LOG(ERROR) << ("Invalid line in x-section text file: \"" + line + "\"\n");
        throw std::runtime_error("");
    }

    const int       dsid = std::stoi(elements.at(0));
    const double    xsec = std::stod(elements.at(2));
    const double    filt = std::stod(elements.at(3));
    const double    kfac = std::stod(elements.at(4));
    const int       etag = std::stoi(elements.at(8).substr(1));

    const double total_xsec = xsec * kfac * filt;

    // Check if the sample is not defined multiple times in the x-section text file
    if (m_xSectionMap.find(dsid) != m_xSectionMap.end())    {
        const int old_etag = std::get<1>(m_xSectionMap.at(dsid));
        // already defined in TopDataPreparation file or with the same e-tag in PMG file
        if (old_etag <= 0 || old_etag == etag) {
            const double old_xsec = std::get<0>(m_xSectionMap.at(dsid));

            if (!Utils::compareDoubles(old_xsec, total_xsec, 1e-6))   {
                LOG(ERROR) << ("The cross section for this DSID is defined multiple times with the same etag, or in both PMG and TopDataPreparation files. DSID: \"" + elements.at(0) + "\". Please fix it.\n");
                throw std::runtime_error("");
            }
        }
        else if (old_etag < etag) {
            m_xSectionMap[dsid] = std::tuple<double,int>(total_xsec, etag);
        }
    }
    else {
        m_xSectionMap[dsid] = std::tuple<double,int>(total_xsec, etag);
    }
};

bool XSectionManager::validTopDataPreparationFileColumns(const std::vector<std::string> &columns)   {
    const bool line_is_valid =  columns.size() < 3 ? false :
                                StringOperations::stringIsInt(columns.at(0)) &&    // DSID
                                StringOperations::stringIsFloat(columns.at(1)) &&  // x-section
                                StringOperations::stringIsFloat(columns.at(2));    // k-factor

    return line_is_valid;
};

bool XSectionManager::validPMGFileColumns(const std::vector<std::string> &columns) {
    const bool line_is_valid =  columns.size() < 9 ? false :
                                StringOperations::stringIsInt(columns.at(0)) &&            // DSID
                                StringOperations::stringIsFloat(columns.at(2)) &&          // x-section
                                StringOperations::stringIsFloat(columns.at(3)) &&          // filter efficiency
                                StringOperations::stringIsFloat(columns.at(4)) &&          // k-factor
                                StringOperations::stringStartsWith(columns.at(8), "e") &&  // e-tag
                                StringOperations::stringIsInt(columns.at(8).substr(1));    // e-tag
    return line_is_valid;
};