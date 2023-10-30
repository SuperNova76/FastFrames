#pragma once

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(x) Logger::get()(LoggingLevel::x, __FILENAME__,  __FUNCTION__, __LINE__)
#define LOG_ENUM(x) Logger::get() (x, __FILE__, __FUNCTION__, __LINE__)

enum class LoggingLevel {
  ERROR = 0,
  WARNING = 1,
  INFO = 2,
  DEBUG = 3
};

class Logger {
public:
  Logger(const Logger&) = delete;
  void operator=(const Logger&) = delete;

  static Logger& get() {
    static Logger logger;
    return logger;
  }

  void setLogLevel(const LoggingLevel& level) {
    m_logLevel = level;
  }

  const LoggingLevel& currentLevel() const {return m_currentLevel;}

  const LoggingLevel& logLevel() const {return m_logLevel;}

  Logger& operator() (const LoggingLevel& level,
                      const char* file,
                      const char* function,
                      int line) {
    m_currentLevel = level;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t); 
    if (level <= m_logLevel) {
      m_stream << fancyHeader(level) << formatString(file + std::string(":") + std::to_string(line), 25) << " " << formatString(function, 20) <<
              " " << formatTime(now->tm_mday) << "-" << formatTime(now->tm_mon+1) << "-" << now->tm_year+1900 << " " << formatTime(now->tm_hour) <<
              ":" << formatTime(now->tm_min) << ":" << formatTime(now->tm_sec) 
              << " | ";
    }
    return *this;
  }

private:
  Logger() :
    m_logLevel(LoggingLevel::INFO),
    m_currentLevel(LoggingLevel::INFO) {};
  LoggingLevel m_logLevel;
  LoggingLevel m_currentLevel;
  std::ostream& m_stream = std::cout;

  static std::string fancyHeader(const LoggingLevel& level) {
    switch (level) {
      case LoggingLevel::ERROR:
        return "\033[0;31m[ ERROR   ]\033[0;0m ";
      case LoggingLevel::WARNING:
        return "\033[0;33m[ WARNING ]\033[0;0m ";
      case LoggingLevel::INFO:
        return "\033[1;32m[ INFO    ]\033[0;0m ";
      case LoggingLevel::DEBUG:
        return "[ DEBUG   ] ";
      default:
        return "";
    }
  }

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

  static std::string formatTime(int time) {
    if (time > 9) return std::to_string(time);
    return "0" + std::to_string(time); 
  } 

};

template<typename T>
Logger& operator <<(Logger& l, const T& message) {
  if (l.currentLevel() <= l.logLevel()) {
    std::cout << message;
    return l;
  } else {
    return l;
  }
}