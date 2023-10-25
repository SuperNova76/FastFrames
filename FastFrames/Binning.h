#pragma once

#include <vector>

class Binning {
public:
  Binning(const int min, const int max, const int nbins) noexcept;

  explicit Binning(const std::vector<double>& binEdges) noexcept;
  
  explicit Binning() noexcept;
  
  ~Binning() = default;

  inline void setBinning(const int min, const int max, const int nbins) {
    m_min = min;
    m_max = max;
    m_nbins = nbins;
  };

  inline void setBinning(const std::vector<double>& binEdges) {m_binEdges = binEdges;}

private:
  int m_min;
  int m_max;
  int m_nbins;
  std::vector<double> m_binEdges;
};
