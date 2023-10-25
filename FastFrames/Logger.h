#pragma once

#define LOG(x) Logger::get()(LoggingLevel::x, __PRETTY_FUNCTION__, __LINE__)
#define LOG_ENUM(x) Logger::get() (x, __PRETTY_FUNCTION__, __LINE__)

#include <iostream>

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
                      const char* function,
                      int line) {
    m_currentLevel = level;
    if (level <= m_logLevel) {
      m_stream << fancyHeader(level) << function << " line: " << line << ": ";
    }
    return *this;
  }

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

private:
  Logger() :
    m_logLevel(LoggingLevel::INFO),
    m_currentLevel(LoggingLevel::INFO) {};
  LoggingLevel m_logLevel;
  LoggingLevel m_currentLevel;
  std::ostream& m_stream = std::cout;
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