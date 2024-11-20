/**
 * @file Logger.h
 * @brief Logging class
 *
 */

#pragma once

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(x) Logger::get()(LoggingLevel::x, __FILENAME__, __LINE__)
#define LOG_ENUM(x) Logger::get() (x, __FILE__, __LINE__)

/**
 * @brief Enum storing different logging levels
 *
 */
enum class LoggingLevel {
  ERROR = 0,
  WARNING = 1,
  INFO = 2,
  DEBUG = 3,
  VERBOSE = 4
};

/**
 * @brief Singletop class for logging
 *
 */
class Logger {
public:

  /**
   * @brief Deleted copy constructor
   *
   */
  Logger(const Logger&) = delete;

  /**
   * @brief Deleted assignment operator
   *
   */
  void operator=(const Logger&) = delete;

  /**
   * @brief Returns the class. Created on first call, then persistent
   *
   * @return Logger&
   */
  static Logger& get() {
    static Logger logger;
    return logger;
  }

  /**
   * @brief Set the Log Level object
   *
   * @param level
   */
  void setLogLevel(const LoggingLevel& level) {
    m_logLevel = level;
  }

  /**
   * @brief Get current logging level
   *
   * @return const LoggingLevel&
   */
  const LoggingLevel& currentLevel() const {return m_currentLevel;}

  /**
   * @brief Get global logging level
   *
   * @return const LoggingLevel&
   */
  const LoggingLevel& logLevel() const {return m_logLevel;}

  /**
   * @brief Functor that sets the current logging level
   *
   * @param level Logging level
   * @param file Name of the file where this is called form
   * @param line Line where this is called from
   * @return Logger&
   */
  Logger& operator() (const LoggingLevel& level,
                      const char* file,
                      int line) {
    m_currentLevel = level;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    if (level <= m_logLevel) {
      m_stream << fancyHeader(level) << formatString(file + std::string(":") + std::to_string(line), 26) <<
              " " << formatTime(now->tm_mday) << "-" << formatTime(now->tm_mon+1) << "-" << now->tm_year+1900 << " " << formatTime(now->tm_hour) <<
              ":" << formatTime(now->tm_min) << ":" << formatTime(now->tm_sec)
              << " | ";
    }
    return *this;
  }

  /**
   * @brief << Operator that allows to use the class an standard streams
   *
   * @tparam T
   * @param l Logger
   * @param message Message to be printed
   * @return Logger&
   */
  template<typename T>
  Logger& operator <<(const T& message) {
    if (this->currentLevel() <= this->logLevel()) {
      std::cout << message;
      return *this;
    } else {
      return *this;
    }
  }
   /**
   * @brief This overload allows std::endl use.
   *
   * @tparam os : Function pointer to the std::endl function.
   * @param 
   */
  Logger& operator<< (std::ostream& (*const os)(std::ostream&))
  {
    if (this->currentLevel() <= this->logLevel()) {
      std::cout << os;
      return *this;
    } else {
      return *this;
    }
  }

private:

  /**
   * @brief Construct a new Logger object
   *
   */
  Logger() :
    m_logLevel(LoggingLevel::INFO),
    m_currentLevel(LoggingLevel::INFO) {};

  LoggingLevel m_logLevel;
  LoggingLevel m_currentLevel;
  std::ostream& m_stream = std::cout;

  /**
   * @brief Return nice header based on the current logLevel
   *
   * @param level Log level
   * @return std::string
   */
  static std::string fancyHeader(const LoggingLevel& level) {
    switch (level) {
      case LoggingLevel::ERROR:
        return "\033[0;31m[ ERROR   ]\033[0;0m ";
      case LoggingLevel::WARNING:
        return "\033[0;33m[ WARNING ]\033[0;0m ";
      case LoggingLevel::INFO:
        return "\033[1;32m[ INFO    ]\033[0;0m ";
      case LoggingLevel::DEBUG:
        return "\033[1;36m[ DEBUG   ]\033[0;0m ";
      case LoggingLevel::VERBOSE:
        return "[ VERBOSE ] ";
      default:
        return "";
    }
  }

  /**
   * @brief Format string to always fit in the size
   *
   * @param input input string
   * @param max maximum column width
   * @return std::string
   */
  static std::string formatString(std::string input, std::size_t max) {
    std::size_t size = input.size();
    if (size >= max-2) {
      input.resize(max-3);
      input += "...";
    } else {
      input += std::string(max-size, ' ');
    }
    return input;
  }

  /**
   * @brief Turn time to always have 2 digits
   *
   * @param time
   * @return std::string
   */
  static std::string formatTime(int time) {
    if (time > 9) return std::to_string(time);
    return "0" + std::to_string(time);
  }

};

