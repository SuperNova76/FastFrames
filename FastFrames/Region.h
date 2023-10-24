#pragma once

#include <FastFrames/Variable.h>

#include <string>
#include <vector>

class Region {
public:
  explicit Region(const std::string& name);

  ~Region() = default;

  inline const std::string& name() {return m_name;}

private:

  std::string m_name;
  std::vector<Variable> m_variables;
};
