#pragma once

#include "FastFrames/StringOperations.h"
#include "FastFrames/Logger.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <map>

class CustomOptions {
    public:
        /**
         * @brief Construct a new Custom Options object
         *
         */
        CustomOptions() = default;

        /**
         * add a new custom option, value must be string
         *
         * @param option name of the option
         * @param value value of the option
        */
        void addOption(const std::string& option, const std::string& value) {
            m_options[option] = value;
        }

        /**
         * get value for the option and convert it to the requested type T. Throw exception if not defined
         *
         * @param option name of the option
         *
         * @return value of the option
        */
        template<typename T = std::string>
        T getOption(const std::string& option)  const {
            if (m_options.find(option) == m_options.end()) {
                const std::string error_message = "Option \"" + option + "\" not provided in the custom_variables of general block.";
                LOG(ERROR) << error_message << "\n";
                throw std::runtime_error(error_message);
            }
            try {
                return StringOperations::convertStringTo<T>(m_options.at(option));
            }
            catch (const std::invalid_argument& e) {
                const std::string error_message = "Value for the \"" + option + "\" could not be converted to the requested type.";
                LOG(ERROR) << error_message << "\n";
                throw std::runtime_error(error_message);
            }
        }

        /**
         * get value for the option and convert it to the requested type T. Return default if not defined
         *
         * @param option name of the option
         * @param default_value default value if option is not defined
         *
         * @return value of the option
        */
        template<typename T = std::string>
        T getOption(const std::string& option, const T& default_value) const {
            if (m_options.find(option) == m_options.end()) {
                return default_value;
            }
            return getOption<T>(option);
        }

        /**
         * check if option is defined
         *
         * @param option name of the option
         *
         * @return true if option is defined
         * @return false if option is not defined
        */
        bool hasOption(const std::string& option) {
            return m_options.find(option) != m_options.end();
        }

        /**
         * get vector of all keys
        */
        const std::vector<std::string> getKeys() const {
            std::vector<std::string> keys;
            if (m_options.empty()) {
                return keys;
            }

            for (const auto& [key, value] : m_options) {
                keys.push_back(key);
            }
            return keys;
        }

    private:
        std::map<std::string, std::string> m_options;
};