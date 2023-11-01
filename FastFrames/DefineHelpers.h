/**
 * @file DefineHelpers.h
 * @brief Helper functions to allow easier processing of the ntuple format
 *
 */

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

  /**
   * @brief Return a vector of indices the the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection);

  /**
   * @brief Return a vector of indices the the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

  /**
   * @brief Get vector based on given set of indices
   *
   * @tparam T
   * @param vector original vector
   * @param indices insides
   * @return std::vector<T>
   */
  template<typename T>
  std::vector<T> vectorFromIndices(const std::vector<T>& vector,
                                   const std::vector<std::size_t>& indices) {

    std::vector<T> result;
    for (const std::size_t index : indices) {
      result.emplace_back(vector.at(index));
    }

    return result;
  }
}
