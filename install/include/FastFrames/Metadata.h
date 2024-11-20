/**
 * @file Metadata.h
 * @brief Metadata information for a sample
 *
 */

#pragma once

#include <string>
#include <map>
#include <vector>

/**
 * @brief Class containing sample Metadata information, such as cross-section, sumWeights etc
 *
 */
class Metadata {
public:

  /**
   * @brief Construct a new Metadata object
   *
   */
  explicit Metadata() noexcept;

  /**
   * @brief Destroy the Metadata object
   *
   */
  ~Metadata() = default;

  /**
   * @brief Set the Cross Section object
   *
   * @param xSec
   */
  inline void setCrossSection(const double xSec) {m_crossSection = xSec; m_crossSectionSet = true;}

  /**
   * @brief Get cross-section
   *
   * @return double
   */
  inline double crossSection() const {return m_crossSection;}

  /**
   * @brief Is the cross-section set?
   * 
   * @return double 
   */
  inline double crossSectionSet() const {return m_crossSectionSet;}

  /**
   * @brief Add sumWeights for this sample
   *
   * @param name Name of the sumWeights variation
   * @param value sumWeights
   */
  void addSumWeights(const std::string& name, const double value);

  /**
   * @brief Get sumWeights for a given name
   *
   * @param name Name of sumWeights
   * @return double
   */
  double sumWeight(const std::string& name) const;

  /**
   * @brief Tells yuo if a given sumWeight exists
   *
   * @param name name of the sumWeights
   * @return true
   * @return false
   */
  bool sumWeightExist(const std::string& name) const;

  /**
   * @brief Is sum weights map empty? 
   * 
   * @return true 
   * @return false 
   */
  inline bool sumWeightsIsEmpty() const {return m_sumWeights.empty();}

  /**
   * @brief Add a path to a ROOT file belonging to this sample
   *
   * @param path
   */
  void addFilePath(const std::string& path);

  /**
   * @brief Get all file paths for this sample
   *
   * @return const std::vector<std::string>&
   */
  inline const std::vector<std::string>& filePaths() const {return m_filePaths;}

private:
  double m_crossSection;
  bool m_crossSectionSet;
  std::map<std::string, double> m_sumWeights;
  std::vector<std::string> m_filePaths;
};
