#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace StringOperations  {

    inline bool contains(const std::string &main_string, const std::string &substring)  {
        return (main_string.find(substring)) != std::string::npos;
    };

    bool starts_with(const std::string &main_string, const std::string &prefix);

    bool ends_with(const std::string &main_string, const std::string &suffix);

    std::vector<std::string> split_and_strip_string(std::string input_string, const std::string &separator);

    std::vector<std::string> split_string(std::string input_string, const std::string &separator);

    std::vector<std::string> split_by_whitespaces(std::string line);

    void strip_string(std::string *input_string, const std::string &chars_to_remove = " \n\t\r");

    void         to_upper(std::string *input_string);

    std::string  to_upper_copy(const std::string &input_string);

    // convert both strings to uppercase and compare
    bool compare_case_insensitive(const std::string &x, const std::string &y);
    template <class ResultType>
    ResultType convert_string_to([[__maybe_unused__]]const std::string &input_string) {
        throw std::runtime_error ("Requested type not implemented!");
    };

    template <> inline
    int convert_string_to(const std::string &input_string)    {
        return std::stoi(input_string);
    };

    template <> inline
    unsigned int convert_string_to(const std::string &input_string)    {
        return std::stoul(input_string);
    };

    template <> inline
    unsigned long long int convert_string_to(const std::string &input_string)    {
        return std::stoull(input_string);
    };

    template <> inline
    float convert_string_to(const std::string &input_string)    {
        return std::stod(input_string);
    };

    template <> inline
    double convert_string_to(const std::string &input_string)    {
        return std::stod(input_string);
    };

    template <> inline
    std::string convert_string_to(const std::string &input_string)    {
        return input_string;
    };

    template <> inline
    bool convert_string_to(const std::string &input_string)    {
        const std::string input_upper = StringOperations::to_upper_copy(input_string);
        if      (input_upper == "TRUE")  return true;
        else if (input_upper == "FALSE") return false;
        else {
            throw std::runtime_error("String \"" + input_string + "\" can't be converted to bool value!");
        }
    };

    template <> inline
    std::vector<int> convert_string_to(const std::string &input_string)    {
        std::string stripped_string = input_string;
        strip_string(&stripped_string, "([{}})");
        std::vector<std::string> elements = split_and_strip_string(stripped_string, ",");

        std::vector<int> result;
        for (const std::string &element : elements) {
            result.push_back(convert_string_to<int>(element));
        }
        return result;
    };

    template <> inline
    std::vector<unsigned long long int> convert_string_to(const std::string &input_string)    {
        std::string stripped_string = input_string;
        strip_string(&stripped_string, "([{}})");
        std::vector<std::string> elements = split_and_strip_string(stripped_string, ",");

        std::vector<unsigned long long int> result;
        for (const std::string &element : elements) {
            result.push_back(convert_string_to<unsigned long long int>(element));
        }
        return result;
    };


    template <> inline
    std::vector<double> convert_string_to(const std::string &input_string)    {
        std::string stripped_string = input_string;
        strip_string(&stripped_string, "([{}})");
        std::vector<std::string> elements = split_and_strip_string(stripped_string, ",");

        std::vector<double> result;
        for (const std::string &element : elements) {
            result.push_back(convert_string_to<double>(element));
        }
        return result;
    };

}