/**
 * @file Utils.h
 * @brief Helper functions
 *
 */

#pragma once

#include "FastFrames/HistoContainer.h"
#include "FastFrames/Variable.h"

#include "ROOT/RDFHelpers.hxx"
#include "TChain.h"
#include "TH1D.h"

#include <memory>
#include <string>
#include <vector>

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
   * @brief Copy a histogram from VariableHistos based on a name of the histogram
   * This triggers event loop!
   *
   * @param histos
   * @param name
   * @return std::unique_ptr<TH1D>
   */
  std::unique_ptr<TH1D> copyHistoFromVariableHistos(const std::vector<VariableHisto>& histos,
                                                    const std::string& name);
}
