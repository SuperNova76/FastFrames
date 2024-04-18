/**
 * @file Utils.h
 * @brief Helper functions
 *
 */

#pragma once

#include "FastFrames/HistoContainer.h"
#include "FastFrames/Region.h"
#include "FastFrames/Variable.h"

#include "ROOT/RDFHelpers.hxx"
#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include <memory>
#include <string>
#include <vector>

class Sample;

/**
 * @brief Helper functions
 *
 */
namespace Utils {

  /**
   * @brief Get TChain from input file paths
   *
   * @param treeName
   * @param files
   * @return TChain
   */
  std::unique_ptr<TChain> chainFromFiles(const std::string& treeName,
                                         const std::vector<std::string>& files);

  /**
   * @brief Get 2D histo model (TH2D) from variables
   *
   * @param v1
   * @param v2
   * @return ROOT::RDF::TH2DModel
   */
  ROOT::RDF::TH2DModel histoModel2D(const Variable& v1, const Variable& v2);

  /**
   * @brief Get 3D histo model (TH3D) from variables
   *
   * @param v1
   * @param v2
   * @param v3
   * @return ROOT::RDF::TH3DModel
   */
  ROOT::RDF::TH3DModel histoModel3D(const Variable& v1, const Variable& v2, const Variable& v3);

  /**
   * @brief Copy a histogram from VariableHistos based on a name of the histogram
   * This triggers event loop!
   *
   * @param histos
   * @param name
   * @return std::unique_ptr<TH1D>
   */
  std::unique_ptr<TH1D> copyHistoFromVariableHistos(const std::vector<VariableHisto>& histos,
                                                    const std::string& name);

  /**
   * @brief
   * @brief Copy a histogram from VariableHistos2d based on a name of the histogram
   * This triggers event loop!
   *
   * @param histos
   * @param name
   * @return std::unique_ptr<TH2D>
   */
  std::unique_ptr<TH2D> copyHistoFromVariableHistos2D(const std::vector<VariableHisto2D>& histos,
                                                      const std::string& name);

  /**
   * @brief Take only N files from the input file list based on an approximate split and the current index
   *
   * @param fileList Input file list
   * @param split Approximate splitting
   * @param index Current index of a job
   * @return std::vector<std::string>
   */
  std::vector<std::string> selectedFileList(const std::vector<std::string>& fileList,
                                            const int split,
                                            const int index);

  /**
   * @brief Set histogram to be between 0 and 1 in each bin
   *
   * @param h
   */
  void capHisto0And1(TH1D* h, const std::string& name);

  /**
   * @brief Get variable using region and variable name
   * 
   * @param regions list of regions 
   * @param regionName region name for the variable
   * @param variableName variable name
   * @return const Variable&
   */
  const Variable& getVariableByName(const std::vector<std::shared_ptr<Region> >& regions,
                                    const std::string& regionName,
                                    const std::string& variableName);
}
