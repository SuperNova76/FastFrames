#pragma once

#include <vector>

class Binning {
public:
  Binning(const double min, const double max, const int nbins) noexcept;

  explicit Binning(const std::vector<double>& binEdges) noexcept;
  
  explicit Binning() noexcept;
  
  ~Binning() = default;

  inline void setBinning(const double min, const double max, const int nbins) {
    m_min = min;
    m_max = max;
    m_nbins = nbins;
    m_hasRegularBinning = true;
  };

  inline void setBinning(const std::vector<double>& binEdges) {m_binEdges = binEdges; m_hasRegularBinning = false;}

  inline bool hasRegularBinning() const {return m_hasRegularBinning;}

  inline const std::vector<double>& binEdges() const {return m_binEdges;}

  inline double min() const {return m_min;}
  inline double max() const {return m_max;}
  inline int nbins() const {return m_nbins;}

private:
  double m_min;
  double m_max;
  int m_nbins;
  std::vector<double> m_binEdges;
  bool m_hasRegularBinning;
};
