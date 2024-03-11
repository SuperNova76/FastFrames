/**
 * @file MetadataManager.h
 * @brief Processing of metadata for a sample
 *
 */

#pragma once

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Metadata.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include "ROOT/RDFHelpers.hxx"

#include <map>
#include <memory>
#include <string>

class Sample;
class Systematic;

/**
 * @brief Class responsible for managing metadata
 *
 */
class MetadataManager {
public:

  /**
   * @brief Construct a new Metadata Manager object
   *
   */
  explicit MetadataManager() noexcept;

  /**
   * @brief Destroy the Metadata Manager object
   *
   */
  ~MetadataManager() = default;

  /**
   * @brief Reads file lists txt file
   *
   * @param path Path to the txt file
   */
  void readFileList(const std::string& path);

  /**
   * @brief Reads file with the sumweight information
   *
   * @param path Oath to the txt file
   */
  void readSumWeights(const std::string& path);

  /**
   * @brief Reads x-section files
   *
   * @param xSectionFiles
   */
  void readXSectionFiles(const std::vector<std::string>& xSectionFiles);

  /**
   * @brief Adds luminosity value for a given campaign
   *
   * @param campaign
   * @param lumi
   */
  void addLuminosity(const std::string& campaign, const double lumi);

  /**
   * @brief Get sumweights for a given Unique sample and systematic
   *
   * @param id
   * @param systematic
   * @return double
   */
  double sumWeights(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief Tells you if a systematic weight exists
   *
   * @param id
   * @param systematic
   * @return true
   * @return false
   */
  bool sumWeightsExist(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief Get luminosity for a given campaign
   *
   * @param campaign
   * @return double
   */
  double luminosity(const std::string& campaign) const;

  /**
   * @brief Get cross-section for a given sample
   *
   * @param id
   * @return double
   */
  double crossSection(const UniqueSampleID& id) const;

  /**
   * @brief Get normalisation (luminosity * cross-section/sumweights) for a given sample and systematic
   *
   * @param id
   * @param systematic
   * @return double
   */
  double normalisation(const UniqueSampleID& id, const std::shared_ptr<Systematic>& systematic) const;

  /**
   * @brief Get file paths for a given unique sample
   *
   * @param id
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string>& filePaths(const UniqueSampleID& id) const;

  /**
   * @brief Check if all sample metadata is available
   *
   * @param samples
   * @return true
   * @return false
   */
  bool checkSamplesMetadata(const std::vector<std::shared_ptr<Sample> >& samples) const;

  /**
   * @brief Get DataSpec class that contains all information about metadata
   * for a given sample (including all unique samples) that can be used to construct
   * the main RDF node for processing
   *
   * @param sample
   * @param config
   * @return ROOT::RDF::Experimental::RDatasetSpec
   */
  ROOT::RDF::Experimental::RDatasetSpec dataSpec(const std::shared_ptr<Sample>& sample,
                                                 const std::shared_ptr<ConfigSetting>& config) const;

private:

  /**
   * @brief Check metadata for a single UniqueSampleID
   *
   * @param id
   * @return true
   * @return false
   */
  bool checkUniqueSampleIDMetadata(const UniqueSampleID& id) const;

  /**
   * @brief Get per-uniqueSampleID information
   *
   * @param sample
   * @param id
   * @param config
   * @return ROOT::RDF::Experimental::RSample
   */
  ROOT::RDF::Experimental::RSample singleSampleInfo(const std::shared_ptr<Sample>& sample,
                                                    const UniqueSampleID& id,
                                                    const std::shared_ptr<ConfigSetting>& config) const;

  /**
   * @brief set sample metadata
   *
   * @param sample
   * @param id
   * @return ROOT::RDF::Experimental::RMetaData
   */
  ROOT::RDF::Experimental::RMetaData sampleMetadata(const std::shared_ptr<Sample>& sample,
                                                    const UniqueSampleID& id) const;

  std::map<UniqueSampleID, Metadata> m_metadata;
  std::map<std::string, double> m_luminosity;
};
