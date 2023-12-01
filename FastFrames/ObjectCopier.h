/**
 * @file ObjectCopier.h
 * @brief Class responsible for copying metedata from input ntuple to output ntuple
 *
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

class TH1;
class TFile;

/**
 * @brief Class responsible for copying metedata from input ntuple to output ntuple
 *
 */
class ObjectCopier {
public:

  enum class ObjectType {
    Histogram = 0,
    Tree = 1
  };

  /**
   * @brief Construct a new Object Copier object
   *
   * @param fileList
   */
  explicit ObjectCopier(const std::vector<std::string>& fileList) noexcept;

  /**
   * @brief Destroy the Object Copier object
   *
   */
  ~ObjectCopier() = default;

  /**
   * @brief Read information about the objects to be copied
   *
   * @param input
   */
  void readObjectInfo();

  /**
   * @brief Do the actual copying of importanat information
   *
   * @param outputPath
   */
  void copyObjectsTo(const std::string& outputPath) const;

  /**
   * @brief Copy trees if requested
   *
   * @param outputPath Output ROOT file path
   * @param trees List of the tree names
   */
  void copyTreesTo(const std::string& outputPath,
                   const std::vector<std::string>& trees) const;

private:

  /**
   * @brief Merge same histograms from multiple files
   *
   * @param name name of the histogram
   * @param files list of files
   * @return std::unique_ptr<TH1>
   */
  std::unique_ptr<TH1> mergeHistos(const std::string& name,
                                   const std::vector<std::unique_ptr<TFile> >& files) const;

  std::vector<std::string> m_fileList;

  std::vector<std::pair<std::string, ObjectType> > m_objectList;
};
