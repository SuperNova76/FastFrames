/**
 * @file Binning.h
 * @brief Header file for the Binning class
 *
 */

#pragma once

#include <vector>

/**
 * @brief Class storing the binnning information for an distribution
 *
 */
class Binning {

public:

  /**
   * @brief Construct a new Binning object (constant bin width)
   *
   * @param min axis min
   * @param max axis max
   * @param nbins number of bins
   */
  Binning(const double min, const double max, const int nbins) noexcept;

  /**
   * @brief Construct a new Binning object (custom binning)
   *
   * @param binEdges edges of the bins
   */
  explicit Binning(const std::vector<double>& binEdges) noexcept;

  /**
   * @brief Construct a new Binning object (default constructor)
   *
   */
  explicit Binning() noexcept;

  /**
   * @brief Destroy the Binning object
   *
   */
  ~Binning() = default;

  /**
   * @brief Set binning (constant bins)
   *
   * @param min axis min
   * @param max axis max
   * @param nbins number of bins
   */
  inline void setBinning(const double min, const double max, const int nbins) {
    m_min = min;
    m_max = max;
    m_nbins = nbins;
    m_hasRegularBinning = true;
  };

  /**
   * @brief Set binning (custom bins)
   *
   * @param binEdges bin edges
   */
  inline void setBinning(const std::vector<double>& binEdges) {m_binEdges = binEdges; m_hasRegularBinning = false;}

  /**
   * @brief Return true of the binning is constant
   *
   * @return true
   * @return false
   */
  inline bool hasRegularBinning() const {return m_hasRegularBinning;}

  /**
   * @brief Get bin edges
   *
   * @return const std::vector<double>&
   */
  inline const std::vector<double>& binEdges() const {return m_binEdges;}

  /**
   * @brief Get axis min
   *
   * @return double
   */
  inline double min() const {return m_min;}

  /**
   * @brief Get axis max
   *
   * @return double
   */
  inline double max() const {return m_max;}

  /**
   * @brief Get number of bins
   *
   * @return int
   */
  inline int nbins() const {return m_nbins;}

private:
  double m_min;
  double m_max;
  int m_nbins;
  std::vector<double> m_binEdges;
  bool m_hasRegularBinning;
};
