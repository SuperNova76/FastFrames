/**
 * @file ConfigDefine.cc
 * @brief Implementation of the ConfigDefine class
 *
 */

#include "FastFrames/ConfigDefine.h"

ConfigDefine::ConfigDefine(const std::string& columnName,
                           const std::string& formula,
                           const std::string& treeName) :
    m_columnName(columnName),
    m_formula(formula),
    m_treeName(treeName)
{}

ConfigDefine::ConfigDefine(const std::string& columnName,
                           const std::string& formula) :
    m_columnName(columnName),
    m_formula(formula),
    m_treeName("reco")
{}
