/**
 * @file StringOperations.cc
 * @brief Helper functions for string operations
 *
 */

#include "FastFrames/StringOperations.h"

#include <algorithm>

using namespace std;
using namespace StringOperations;

std::string StringOperations::replaceString(const std::string& original,
                                            const std::string& from,
                                            const std::string& to) {

    std::string result(original);
    std::string::size_type n = 0;
    while ((n = result.find(from, n)) != std::string::npos) {
        result.replace(n, from.size(), to);
        n += to.size();
    }

    return result;
}

std::vector<std::string> StringOperations::replaceVector(const std::vector<std::string>& input,
                                                         const std::string& from,
                                                         const std::string& to) {

    std::vector<std::string> result;

    for (const auto& ielement : input) {
        result.emplace_back(StringOperations::replaceString(ielement, from, to));
    }

    return result;
}

bool StringOperations::stringStartsWith(const string &main_string, const string &prefix)    {
    const unsigned int prefix_lenght = prefix.length();
    const unsigned int string_lenght = main_string.length();
    if (prefix_lenght > string_lenght) return false;
    return prefix == main_string.substr(0, prefix_lenght);
};

bool StringOperations::stringEndsWith(const string &main_string, const string &suffix)    {
    const unsigned int suffix_lenght = suffix.length();
    const unsigned int string_lenght = main_string.length();
    if (suffix_lenght > string_lenght) return false;
    return suffix == main_string.substr(string_lenght-suffix_lenght, string_lenght);
};


bool StringOperations::stringIsInt(const std::string &input_string) {
    return !input_string.empty() && input_string.find_first_not_of("0123456789") == std::string::npos;
};

bool StringOperations::stringIsFloat(const std::string &input_string)   {
    try {
        [[__maybe_unused__]]float a = std::stof(input_string);
    }
    catch (const std::invalid_argument& ia) {
        return false;
    }
    return true;
};

vector<string> StringOperations::splitAndStripString(const string &input_string, const string &separator) {
    vector<string> result = splitString(input_string, separator);
    for (string &x : result)    {
        stripString(&x, " \n\t\r");
    }
    return result;
}

vector<string> StringOperations::splitString(string input_string, const string &separator)    {
    vector<string> result;
    size_t pos = 0;
    while ((pos = input_string.find(separator)) != std::string::npos) {
        result.push_back(input_string.substr(0, pos));
        input_string.erase(0, pos + separator.length());
    }
    if (input_string.length() > 0) result.push_back(input_string);
    return result;
};

std::vector<std::string> StringOperations::splitByWhitespaces(std::string line)   {
    const string white_space_chars = " \t\r\n";
    stripString(&line, white_space_chars);
    std::vector<std::string> result;
    while (line.size() > 0) {
        const std::size_t first_white_space = line.find_first_of(white_space_chars);
        if (first_white_space == std::string::npos) {
            result.push_back(line);
            break;
        }
        else    {
            result.push_back(line.substr(0, first_white_space));
            line.erase(0, first_white_space);
            stripString(&line, white_space_chars);
        }
    }
    return result;
};

void StringOperations::stripString(string *input_string, const string &chars_to_remove)    {
    input_string->erase(0,input_string->find_first_not_of(chars_to_remove));
    input_string->erase(input_string->find_last_not_of(chars_to_remove)+1);
}

void StringOperations::toUpper(std::string *input_string) {
    std::transform(input_string->begin(), input_string->end(),input_string->begin(), ::toupper);
};

std::string  StringOperations::toUpperCopy(const std::string &input_string)  {
    string result = input_string;
    StringOperations::toUpper(&result);
    return result;
};

bool StringOperations::compare_case_insensitive(const std::string &x, const std::string &y)    {
    const string x_upper = StringOperations::toUpperCopy(x);
    const string y_upper = StringOperations::toUpperCopy(y);
    return x_upper == y_upper;
};
