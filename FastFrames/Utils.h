/**
 * @file Utils.h
 * @brief Helper functions
 *
 */

#pragma once

#include "TChain.h"

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

}
