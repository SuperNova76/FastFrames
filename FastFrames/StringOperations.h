/**
 * @file StringOperations.h
 * @brief Helper functions for string operations
 *
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace StringOperations  {

    /**
     * @brief Replaces all occurances of substring with another string
     *
     * @param original input string
     * @param from string to be replaced
     * @param to replace with
     * @return std::string
     */
    std::string replaceString(const std::string& original,
                              const std::string& from,
                              const std::string& to);

    /**
     * @brief calls replaceString on each element of a string vector
     *
     * @param input input string vector
     * @param from string to be replaced in each element
     * @param to replace with
     * @return std::vector<std::string>
     */
    std::vector<std::string> replaceVector(const std::vector<std::string>& input,
                                           const std::string& from,
                                           const std::string& to);

    /**
     * @brief String contains a substring
     *
     * @param main_string
     * @param substring
     * @return true
     * @return false
     */
    inline bool contains(const std::string &main_string, const std::string &substring)  {
        return (main_string.find(substring)) != std::string::npos;
    };

    /**
     * @brief String starts with a substring
     *
     * @param main_string
     * @param prefix
     * @return true
     * @return false
     */
    bool stringStartsWith(const std::string &main_string, const std::string &prefix);

    /**
     * @brief String ends with a substring
     *
     * @param main_string
     * @param suffix
     * @return true
     * @return false
     */
    bool stringEndsWith(const std::string &main_string, const std::string &suffix);

    /**
     * @brief String is an iteger
     *
     * @param input_string
     * @return true
     * @return false
     */
    bool stringIsInt(const std::string &input_string);

    /**
     * @brief String is a float
     *
     * @param input_string
     * @return true
     * @return false
     */
    bool stringIsFloat(const std::string &input_string);

    /**
     * @brief Split string into substrings and remove whitespace
     *
     * @param input_string
     * @param separator
     * @return std::vector<std::string>
     */
    std::vector<std::string> splitAndStripString(const std::string &input_string, const std::string &separator);

    /**
     * @brief Split string into substring but keep whitespaces
     *
     * @param input_string
     * @param separator
     * @return std::vector<std::string>
     */
    std::vector<std::string> splitString(std::string input_string, const std::string &separator);

    /**
     * @brief Split string by whitesaces
     *
     * @param line
     * @return std::vector<std::string>
     */
    std::vector<std::string> splitByWhitespaces(std::string line);

    /**
     * @brief Remove characters from a string
     *
     * @param input_string
     * @param chars_to_remove
     */
    void stripString(std::string *input_string, const std::string &chars_to_remove = " \n\t\r");

    /**
     * @brief Switch all characters to upper case
     *
     * @param input_string
     */
    void         toUpper(std::string *input_string);

    /**
     * @brief Make a copy of a string with upper cases
     *
     * @param input_string
     * @return std::string
     */
    std::string  toUpperCopy(const std::string &input_string);

    /**
     * @brief Compares two string to upper case and compare them
     *
     * @param x
     * @param y
     * @return true
     * @return false
     */
    bool compare_case_insensitive(const std::string &x, const std::string &y);

    /**
     * @brief Convert string to a given format
     *
     * @tparam ResultType
     * @param input_string
     * @return ResultType
     */
    template <class ResultType>
    ResultType convertStringTo([[__maybe_unused__]]const std::string &input_string) {
        throw std::runtime_error ("Requested type not implemented!");
    };

    /**
     * @brief Convert string to an int
     *
     * @tparam
     * @param input_string
     * @return int
     */
    template <> inline
    int convertStringTo(const std::string &input_string)    {
        return std::stoi(input_string);
    };

    /**
     * @brief Convert string to an unsigned int
     *
     * @tparam
     * @param input_string
     * @return unsigned int
     */
    template <> inline
    unsigned int convertStringTo(const std::string &input_string)    {
        return std::stoul(input_string);
    };

    /**
     * @brief Convert string into unsigned long long int
     *
     * @tparam
     * @param input_string
     * @return unsigned long long int
     */
    template <> inline
    unsigned long long int convertStringTo(const std::string &input_string)    {
        return std::stoull(input_string);
    };

    /**
     * @brief Convert string to float
     *
     * @tparam
     * @param input_string
     * @return float
     */
    template <> inline
    float convertStringTo(const std::string &input_string)    {
        return std::stod(input_string);
    };

    /**
     * @brief Convert string to double
     *
     * @tparam
     * @param input_string
     * @return double
     */
    template <> inline
    double convertStringTo(const std::string &input_string)    {
        return std::stod(input_string);
    };

    /**
     * @brief Convert string to string - needed for other template functions
     *
     * @tparam
     * @param input_string
     * @return std::string
     */
    template <> inline
    std::string convertStringTo(const std::string &input_string)    {
        return input_string;
    };

    /**
     * @brief Convert \"true\" and \"false\" to boolean (case insensitive)
     *
     * @tparam
     * @param input_string
     * @return true
     * @return false
     */
    template <> inline
    bool convertStringTo(const std::string &input_string)    {
        const std::string inputUpper = StringOperations::toUpperCopy(input_string);
        if      (inputUpper == "TRUE")  return true;
        else if (inputUpper == "FALSE") return false;
        else {
            throw std::runtime_error("String \"" + input_string + "\" can't be converted to bool value!");
        }
    };

    /**
     * @brief Convert to a vector of integers needed for brackets treatment
     *
     * @tparam
     * @param input_string
     * @return std::vector<int>
     */
    template <> inline
    std::vector<int> convertStringTo(const std::string &input_string)    {
        std::string stripped_string = input_string;
        stripString(&stripped_string, "([{}})");
        std::vector<std::string> elements = splitAndStripString(stripped_string, ",");

        std::vector<int> result;
        for (const std::string &element : elements) {
            result.push_back(convertStringTo<int>(element));
        }
        return result;
    };

    /**
     * @brief Convert string to unsigned long long int needed for brackets treatment
     *
     * @tparam
     * @param input_string
     * @return std::vector<unsigned long long int>
     */
    template <> inline
    std::vector<unsigned long long int> convertStringTo(const std::string &input_string)    {
        std::string stripped_string = input_string;
        stripString(&stripped_string, "([{}})");
        std::vector<std::string> elements = splitAndStripString(stripped_string, ",");

        std::vector<unsigned long long int> result;
        for (const std::string &element : elements) {
            result.push_back(convertStringTo<unsigned long long int>(element));
        }
        return result;
    };


    /**
     * @brief Convert string to double needed for brackets treatment
     *
     * @tparam
     * @param input_string
     * @return std::vector<double>
     */
    template <> inline
    std::vector<double> convertStringTo(const std::string &input_string)    {
        std::string stripped_string = input_string;
        stripString(&stripped_string, "([{}})");
        std::vector<std::string> elements = splitAndStripString(stripped_string, ",");

        std::vector<double> result;
        for (const std::string &element : elements) {
            result.push_back(convertStringTo<double>(element));
        }
        return result;
    };

}