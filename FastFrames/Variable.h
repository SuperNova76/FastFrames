#pragma once

#include "FastFrames/Binning.h"

#include <string>

class Variable {
public:
  explicit Variable(const std::string& name);

  ~Variable() = default;

  inline const std::string& name() const {return m_name;}

  inline void setDefinition(const std::string& definition) {m_definition = definition;}

  inline const std::string& definition() const {return m_definition;}

private:
  std::string m_name;
  std::string m_definition;
  Binning m_binning;
};
