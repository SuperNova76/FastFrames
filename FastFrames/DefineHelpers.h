#pragma once

#include "Math/Vector4D.h"

#include <vector>

using TLV = ROOT::Math::PtEtaPhiEVector;

namespace DefineHelpers {

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that paased the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @param passedOR
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection,
                                      const std::vector<char>& passedOR);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that paased the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection);
}
