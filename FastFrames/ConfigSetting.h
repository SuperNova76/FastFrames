#pragma once

#include "FastFrames/Region.h"
#include "FastFrames/Sample.h"
#include "FastFrames/Systematic.h"
#include "FastFrames/Logger.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

class ConfigSetting {
public:
  explicit ConfigSetting();

  ~ConfigSetting() = default;


  std::string outputPath() const {return m_outputPath;}

  void setOutputPath(const std::string& outputPath) {m_outputPath = outputPath;}


  const std::string &inputPath() const {return m_inputPath;}

  void setInputPath(const std::string &inputPath) {m_inputPath = inputPath;}


  const std::string &inputSumWeightsPath() const {return m_inputSumWeightsPath;}

  void setInputSumWeightsPath(const std::string &inputSumWeightsPath) {m_inputSumWeightsPath = inputSumWeightsPath;}


  const std::string &inputFilelistPath() const {return m_inputFilelistPath;}

  void setInputFilelistPath(const std::string &inputFilelistPath) {m_inputFilelistPath = inputFilelistPath;}


  const std::string &customFrameName() const {return m_customFrameName;}

  void setCustomFrameName(const std::string &customFrameName) {m_customFrameName = customFrameName;}


  int numCPU() const {return m_numCPU;}

  void setNumCPU(int numCPU) {m_numCPU = numCPU;}


  void addLuminosityInformation(const std::string& campaign, const float luminosity);

  float getLuminosity(const std::string& campaign) const;


  const std::vector<std::shared_ptr<Region> >& regions() const {return m_regions;}

  const std::vector<std::shared_ptr<Sample> >& samples() const {return m_samples;}

  const std::vector<std::shared_ptr<Systematic> >& systematics() const {return m_systematics;}

private:
  std::string m_outputPath;
  std::string m_inputPath;
  std::string m_inputSumWeightsPath;
  std::string m_inputFilelistPath;
  std::string m_customFrameName;

  int         m_numCPU = 1;

  std::map<std::string, float> m_luminosity_map;

  std::vector<std::shared_ptr<Region> > m_regions;
  std::vector<std::shared_ptr<Sample> > m_samples;
  std::vector<std::shared_ptr<Systematic> > m_systematics;

};
