/**
 * @file ConfifDefine
 * @brief Header for ConfigDefine class
 *
 */

#pragma once

#include <string>

/**
 * @brief Class storing the information needed for calling define
 * for reco and truth trees
 *
 */
class ConfigDefine {

public:
    ConfigDefine() = delete;

    /**
     * @brief Construct a new Config Define object without tree name (for reco trees)
     *
     * @param columnName
     * @param formula
     */
    ConfigDefine(const std::string& columnName,
                 const std::string& formula);

    /**
     * @brief Construct a new Config Define object including the tree name (for truth trees)
     *
     * @param columnName
     * @param formula
     * @param treeName
     */
    ConfigDefine(const std::string& columnName,
                 const std::string& formula,
                 const std::string& treeName);

    /**
     * @brief Destroy the Config Define object
     *
     */
    ~ConfigDefine() = default;

    /**
     * @brief Get new column name
     *
     * @return const std::string&
     */
    inline const std::string& columnName() const {return m_columnName;}

    /**
     * @brief Get the formula for define
     *
     * @return const std::string&
     */
    inline const std::string& formula() const {return m_formula;}

    /**
     * @brief Get the tree name
     *
     * @return const std::string&
     */
    inline const std::string& treeName() const {return m_treeName;}

private:
    std::string m_columnName;
    std::string m_formula;
    std::string m_treeName;
};
