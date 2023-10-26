#pragma once

#include <FastFrames/Variable.h>

#include <string>
#include <vector>

class Region {
public:
  explicit Region(const std::string& name) noexcept;

  ~Region() = default;

  inline const std::string& name() const {return m_name;}

  inline void setSelection(const std::string& selection) {m_selection = selection;}

  inline const std::string& selection() const {return m_selection;}

  inline void addVariable(const Variable& variable) {m_variables.emplace_back(variable);}

  inline const std::vector<Variable>& variables() const {return m_variables;}

private:

  std::string m_name;
  std::string m_selection;
  std::vector<Variable> m_variables;
};
