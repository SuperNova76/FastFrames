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

    inline bool contains(const std::string &main_string, const std::string &substring)  {
        return (main_string.find(substring)) != std::string::npos;
    };

    bool stringStartsWith(const std::string &main_string, const std::string &prefix);

    bool stringEndsWith(const std::string &main_string, const std::string &suffix);

    std::vector<std::string> splitAndStripString(const std::string &input_string, const std::string &separator);

    std::vector<std::string> splitString(std::string input_string, const std::string &separator);

    std::vector<std::string> splitByWhitespaces(std::string line);

    void stripString(std::string *input_string, const std::string &chars_to_remove = " \n\t\r");

    void         toUpper(std::string *input_string);

    std::string  toUpperCopy(const std::string &input_string);

    // convert both strings to uppercase and compare
    bool compare_case_insensitive(const std::string &x, const std::string &y);

    template <class ResultType>
    ResultType convertStringTo([[__maybe_unused__]]const std::string &input_string) {
        throw std::runtime_error ("Requested type not implemented!");
    };

    template <> inline
    int convertStringTo(const std::string &input_string)    {
        return std::stoi(input_string);
    };

    template <> inline
    unsigned int convertStringTo(const std::string &input_string)    {
        return std::stoul(input_string);
    };

    template <> inline
    unsigned long long int convertStringTo(const std::string &input_string)    {
        return std::stoull(input_string);
    };

    template <> inline
    float convertStringTo(const std::string &input_string)    {
        return std::stod(input_string);
    };

    template <> inline
    double convertStringTo(const std::string &input_string)    {
        return std::stod(input_string);
    };

    template <> inline
    std::string convertStringTo(const std::string &input_string)    {
        return input_string;
    };

    template <> inline
    bool convertStringTo(const std::string &input_string)    {
        const std::string inputUpper = StringOperations::toUpperCopy(input_string);
        if      (inputUpper == "TRUE")  return true;
        else if (inputUpper == "FALSE") return false;
        else {
            throw std::runtime_error("String \"" + input_string + "\" can't be converted to bool value!");
        }
    };

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