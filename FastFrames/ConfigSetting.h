/**
 * @file ConfigSetting.h
 * @brief Config class
 *
 */

#pragma once

#include "FastFrames/Logger.h"
#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class sotring all relevant configuration options
 *
 */
class ConfigSetting {
public:

  /**
   * @brief Construct a new Config Setting object
   *
   */
  explicit ConfigSetting();

  /**
   * @brief Destroy the Config Setting object
   *
   */
  ~ConfigSetting() = default;

  /**
   * @brief Path the output folder
   *
   * @return std::string
   */
  std::string outputPath() const {return m_outputPath;}

  /**
   * @brief Set path to the output folder
   *
   * @param outputPath
   */
  void setOutputPath(const std::string& outputPath) {m_outputPath = outputPath;}

  /**
   * @brief Path to the sum weights file
   *
   * @return const std::string&
   */
  const std::string &inputSumWeightsPath() const {return m_inputSumWeightsPath;}

  /**
   * @brief Set path to the sum weights file
   *
   * @param inputSumWeightsPath
   */
  void setInputSumWeightsPath(const std::string &inputSumWeightsPath) {m_inputSumWeightsPath = inputSumWeightsPath;}

  /**
   * @brief Path to the file list file
   *
   * @return const std::string&
   */
  const std::string &inputFilelistPath() const {return m_inputFilelistPath;}

  /**
   * @brief Set path to the file list file
   *
   * @param inputFilelistPath
   */
  void setInputFilelistPath(const std::string &inputFilelistPath) {m_inputFilelistPath = inputFilelistPath;}

  /**
   * @brief Name of the custom derived class
   *
   * @return const std::string&
   */
  const std::string &customFrameName() const {return m_customFrameName;}

  /**
   * @brief Set the name of the custom derived class
   *
   * @param customFrameName
   */
  void setCustomFrameName(const std::string &customFrameName) {m_customFrameName = customFrameName;}

  /**
   * @brief Number of CPUs used for RDataFrame processing
   *
   * @return int
   */
  int numCPU() const {return m_numCPU;}

  /**
   * @brief Set the number of CPUs used for RDataFrame processing
   *
   * @param numCPU
   */
  void setNumCPU(int numCPU) {m_numCPU = numCPU;}

  /**
   * @brief Add luminosity for  given campaign
   *
   * @param campaign
   * @param luminosity
   */
  void addLuminosityInformation(const std::string& campaign, const float luminosity);

  /**
   * @brief Get the from a campaign
   *
   * @param campaign
   * @return float
   */
  float getLuminosity(const std::string& campaign) const;

  /**
   * @brief Get the full luminosity map for each campaign set
   *
   * @return const std::map<std::string, float>&
   */
  const std::map<std::string, float>& luminosityMap() const {return m_luminosity_map;}

  /**
   * @brief Add one Region
   *
   * @param region
   */
  void addRegion(const std::shared_ptr<Region>& region);

  /**
   * @brief Get list of all Regions
   *
   * @return const std::vector<std::shared_ptr<Region> >&
   */
  const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

  /**
   * @brief Get list of all Samples
   *
   * @return const std::vector<std::shared_ptr<Sample> >&
   */
  const std::vector<std::shared_ptr<Sample> >& samples() const {return m_samples;}

  /**
   * @brief Get list of all Systematics
   *
   * @return const std::vector<std::shared_ptr<Systematic> >&
   */
  const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

private:
  std::string m_outputPath;
  std::string m_inputSumWeightsPath;
  std::string m_inputFilelistPath;
  std::string m_customFrameName;

  int         m_numCPU = 1;

  std::map<std::string, float> m_luminosity_map;

  std::vector<std::shared_ptr<Region> > m_regions;
  std::vector<std::shared_ptr<Sample> > m_samples;
  std::vector<std::shared_ptr<Systematic> > m_systematics;

};
