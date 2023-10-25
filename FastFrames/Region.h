#pragma once

#include <FastFrames/Variable.h>

#include <string>
#include <vector>

class Region {
public:
  explicit Region(const std::string& name) noexcept;

  ~Region() = default;

  inline const std::string& name() {return m_name;}

  inline void setSelection(const std::string& selection) {m_selection = selection;}

  inline const std::string& selection() const {return m_selection;}

private:

  std::string m_name;
  std::string m_selection;
  std::vector<Variable> m_variables;
};
