/**
 * @file XSectionManager.cc
 * @brief Code for extracting cross-sections
 *
 */

#include "FastFrames/XSectionManager.h"
#include "FastFrames/Logger.h"
#include "FastFrames/StringOperations.h"

#include <fstream>
#include <stdexcept>

using std::string, std::map, std::vector, std::ifstream;


XSectionManager::XSectionManager(const std::vector<std::string> &xSectionFiles)  {
    for (const string &xSectionFile : xSectionFiles) {
        readTopDataPreparationXSectionFile(xSectionFile);
    }
};

double XSectionManager::xSection(const int sampleDSID) const    {
    if (m_xSectionMap.find(sampleDSID) == m_xSectionMap.end()) {
        LOG(ERROR) << "Cannot find cross section for DSID: " << sampleDSID << ", please update your x-section file\n";
        throw std::runtime_error("");
    }
    return std::get<0>(m_xSectionMap.at(sampleDSID));
};

void XSectionManager::readTopDataPreparationXSectionFile(const std::string &xSectionFile) {
    string line;
    ifstream input_file (xSectionFile);
    if (input_file.is_open())    {
        while ( getline (input_file,line) )        {
            processTopDataPreparationLine(line);
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
    if (!validTopDataPreparationLine(line))   {
        return;
    }

    const vector<string> elements = StringOperations::splitByWhitespaces(line);
    if (elements.size() < 3)    {
        LOG(ERROR) << ("Invalid line in x-section text file: \"" + line + "\"\n");
        throw std::runtime_error("");
    }

    // Check whether the 1st element is float (it's supposed to be x-section)
    if (!StringOperations::stringIsFloat(elements.at(1)))    {
        LOG(ERROR) << ("I couldn't read the x-section for sample with dsid " + elements.at(0) + ". The x-section value in text file: \"" + elements.at(1) + "\"\n");
        throw std::runtime_error("");
    }

    // Check whether the 2nd element is float (it's supposed to be k-factor)
    if (!StringOperations::stringIsFloat(elements.at(2)))    {
        LOG(ERROR) << ("I couldn't read the k-factor for sample with dsid " + elements.at(0) + ". The k-factor value in text file: \"" + elements.at(2) + "\"\n");
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
    m_xSectionMap[dsid] = std::make_tuple<double,int>(xsec * kfac,0);
};

bool XSectionManager::validTopDataPreparationLine(const std::string &line) const  {
    string strippedString = line;
    StringOperations::stripString(&strippedString);

    if (strippedString.size() == 0)    {
        return false;
    }

    return strippedString.at(0) != '#';
};