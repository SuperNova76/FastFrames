#pragma once

#include <string>

class Systematic {
public:

  explicit Systematic(const std::string& name) noexcept;

  ~Systematic() = default;

  inline const std::string& name() const {return m_name;} 

private:

  std::string m_name;
};
