#pragma once

#include <string>

class Systematic {
public:

  explicit Systematic(const std::string& name) noexcept;

  ~Systematic() = default;

  inline const std::string& name() const {return m_name;} 

  inline void setSumWeights(const std::string& sumWeights) {m_sumWeights = sumWeights;};

  inline const std::string& sumWeights() const {return m_sumWeights;}

private:

  std::string m_name;
  std::string m_sumWeights;
};
