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
   * @brief Set some regions, systematics and samples for testing
   */
  void setTestingValues();


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
   * @brief Get list of all Samples (non const)
   *
   * @return const std::vector<std::shared_ptr<Sample> >&
   */
  std::vector<std::shared_ptr<Sample> >& samples() {return m_samples;}

  /**
   * @brief Add sample
   *
   * @param sample
   */
  void addSample(const std::shared_ptr<Sample> &samples)  {
    m_samples.push_back(samples);
  };


  /**
   * @brief Get list of all Systematics
   *
   * @return const std::vector<std::shared_ptr<Systematic> >&
   */
  const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

  /**
   * @brief Add systematic uncertainty
   *
   * @param systematic
   */
  void addSystematic(const std::shared_ptr<Systematic> &systematic) {
    m_systematics.push_back(systematic);
  };

  /**
   * @brief Set to true to only use nominal systematics
   *
   * @param flag
   */
  void setNominalOnly(const bool flag) {m_nominalOnly = flag;}

  /**
   * @brief Get is nominal only
   *
   * @return true
   * @return false
   */
  bool nominalOnly() const {return m_nominalOnly;}

  /**
   * @brief Set to true if all systematic uncertainties should be read from the file
   *
   * @param flag
   */
  void setAutomaticSystematics(const bool flag) {m_automaticSystematics = flag;}

  /**
   * @brief Get automaticSystematics
   *
   * @return true
   * @return false
   */
  bool automaticSystematics() const {return m_automaticSystematics;}

  /**
   * @brief Remove all systematics
   *
   */
  void clearSystematics() {m_systematics.clear();}

  /**
   * @brief Add systematic that is not present
   *
   * @param syst
   */
  void addUniqueSystematic(const std::shared_ptr<Systematic>& syst);

private:
  std::string m_outputPath;
  std::string m_inputSumWeightsPath;
  std::string m_inputFilelistPath;
  std::string m_customFrameName;

  int         m_numCPU = 1;
  bool        m_nominalOnly = false;
  bool        m_automaticSystematics = false;

  std::map<std::string, float> m_luminosity_map;

  std::vector<std::shared_ptr<Region> > m_regions;
  std::vector<std::shared_ptr<Sample> > m_samples;
  std::vector<std::shared_ptr<Systematic> > m_systematics;

};
