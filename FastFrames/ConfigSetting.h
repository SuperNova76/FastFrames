/**
 * @file ConfigSetting.h
 * @brief Config class
 *
 */

#pragma once

#include "FastFrames/Logger.h"
#include "FastFrames/Ntuple.h"
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
   * @brief Path the output folder for histogram option
   *
   * @return std::string
   */
  std::string outputPathHistograms() const {return m_outputPathHistograms;}

  /**
   * @brief Set path to the output folder for histogram option
   *
   * @param outputPathHistograms
   */
  void setOutputPathHistograms(const std::string& outputPathHistograms) {m_outputPathHistograms = outputPathHistograms;}

  /**
   * @brief Path the output folder for Ntuple option
   *
   * @return std::string
   */
  std::string outputPathNtuples() const {return m_outputPathNtuples;}

  /**
   * @brief Set path to the output folder for Ntuple option
   *
   * @param outputPathNtuples
   */
  void setOutputPathNtuples(const std::string& outputPathNtuples) {m_outputPathNtuples = outputPathNtuples;}


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
   * @param force if true, overwrite existing value, if false, throw error if already defined
   */
  void addLuminosityInformation(const std::string& campaign, const float luminosity, const bool force = false);

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
   * @brief Add x-section file
   *
   */
  void addXsectionFile(const std::string& xSectionFile)  { m_xSectionFiles.push_back(xSectionFile); };

  /**
   * @brief Get x-section files
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& xSectionFiles() const {return m_xSectionFiles;};

  /**
   * @brief Add TlorentzVector to create
   *
   * @param tlorentz_vector_to_create
   */
  void addTLorentzVector(const std::string& tlorentz_vector_to_create)  { m_tLorentzVectors.push_back(tlorentz_vector_to_create); };

  /**
   * @brief Get TlorentzVector to create
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& tLorentzVectors() const {return m_tLorentzVectors;};

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

  /**
   * @brief Set ntuple
   *
   * @param ntuple
   */
  void setNtuple(const std::shared_ptr<Ntuple>& ntuple) {m_ntuple = ntuple;}

  /**
   * @brief Get ntuple
   *
   * @return const std::shared_ptr<Ntuple>&
   */
  const std::shared_ptr<Ntuple>& ntuple() const {return m_ntuple;}

  /**
   * @brief Set the minimum event count to be processed
   *
   * @param i
   */
  void setMinEvent(const long long int i) {m_minEvent = i;}

  /**
   * @brief Get the min event index
   *
   * @return long long int
   */
  long long int minEvent() const {return m_minEvent;}

  /**
   * @brief Get the max event index
   *
   * @return long long int
   */
  long long int maxEvent() const {return m_maxEvent;}

  /**
   * @brief Set the maximum event count to be processed
   *
   * @param i
   */
  void setMaxEvent(const long long int i) {m_maxEvent = i;}

private:
  std::string m_outputPathHistograms;
  std::string m_outputPathNtuples;
  std::string m_inputSumWeightsPath;
  std::string m_inputFilelistPath;
  std::string m_customFrameName;

  int           m_numCPU = 1;
  bool          m_nominalOnly = false;
  bool          m_automaticSystematics = false;
  long long int m_minEvent = -1;
  long long int m_maxEvent = -1;

  std::map<std::string, float> m_luminosity_map;
  std::vector<std::string>     m_xSectionFiles ;
  std::vector<std::string>     m_tLorentzVectors;

  std::vector<std::shared_ptr<Region> > m_regions;
  std::vector<std::shared_ptr<Sample> > m_samples;
  std::vector<std::shared_ptr<Systematic> > m_systematics;
  std::shared_ptr<Ntuple> m_ntuple = nullptr;

};
