/**
 * @file DefineHelpers.h
 * @brief Helper functions for Define
 *
 */
#pragma once

#include "Math/Vector4D.h"

#include <vector>

using TLV = ROOT::Math::PtEtaPhiEVector;

/**
 * @brief Helper functions for Define
 *
 */
namespace DefineHelpers {

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
   * @brief Return a vector of indices the elements that passed a selection sorted by pT
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
   * @brief Number of objects passing a selection and minimum pT requirement 
   * 
   * @param tlv LorentzVectors
   * @param minPt minimum pT
   * @param selection1 first selection
   * @param selection2 second selection
   * @return std::size_t 
   */
  std::size_t numberOfObjects(const std::vector<TLV>& tlv,
                              const float minPt,
                              const std::vector<char>& selection1,
                              const std::vector<char>& selection2);

  /**
   * @brief Number of objects passing a selection and minimum pT requirement 
   * 
   * @param tlv LorentzVectors
   * @param minPt minimum pT
   * @param selection selection
   * @return std::size_t 
   */
  std::size_t numberOfObjects(const std::vector<TLV>& tlv,
                              const float minPt,
                              const std::vector<char>& selection);

  /**
   * @brief Number of objects passing a selection and minimum pT requirement 
   * 
   * @param pts Pt of the objects
   * @param minPt minimum pT
   * @param selection selection
   * @return std::size_t 
   */
  std::size_t numberOfObjects(const std::vector<float>& pts,
                              const float minPt,
                              const std::vector<char>& selection);
  
  /**
   * @brief Number of objects passing a selection and minimum pT requirement 
   * 
   * @param pts Pt of the objects
   * @param minPt minimum pT
   * @param selection1 selection1
   * @param selection2 selection2
   * @return std::size_t 
   */
  std::size_t numberOfObjects(const std::vector<float>& pts,
                              const float minPt,
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
};
