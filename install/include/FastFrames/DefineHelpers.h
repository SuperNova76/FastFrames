/**
 * @file DefineHelpers.h
 * @brief Helper functions for Define
 *
 */
#pragma once

#include "Math/Vector4D.h"

#include <vector>

#include "ROOT/RVec.hxx"

using TLV = ROOT::Math::PtEtaPhiEVector;

/**
 * @brief Helper functions for Define
 *
 */
namespace DefineHelpers {

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @return ROOT::VecOps::RVec
   */
  ROOT::VecOps::RVec<TLV> sortedPassedVector(const ROOT::VecOps::RVec<TLV>& tlv,
                                             const ROOT::VecOps::RVec<char>& passedSelection);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed the selection and sort the resulting vector based on pT
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
   * that passed the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @param passedOR
   * @return ROOT::VecOps::RVec
   */
  ROOT::VecOps::RVec<TLV> sortedPassedVector(const ROOT::VecOps::RVec<TLV>& tlv,
                                             const ROOT::VecOps::RVec<char>& passedSelection,
                                             const ROOT::VecOps::RVec<char>& passedOR);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection1
   * @param passedSelection2
   * @param passedOR
   * @return std::vector<TLV>
   */

  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection1,
                                      const std::vector<char>& passedSelection2,
                                      const std::vector<char>& passedOR);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection1
   * @param passedSelection2
   * @param passedOR
   * @return ROOT::VecOps::RVec<TLV>
   */

  ROOT::VecOps::RVec<TLV> sortedPassedVector(const ROOT::VecOps::RVec<TLV>& tlv,
                                             const ROOT::VecOps::RVec<char>& passedSelection1,
                                             const ROOT::VecOps::RVec<char>& passedSelection2,
                                             const ROOT::VecOps::RVec<char>& passedOR);

  /**
   * @brief Return a vector of indices of the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection);

  /**
   * @brief Return a vector of indices of the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection vector
   * @return ROOT::VecOps::RVec<std::size_t>
   */
  ROOT::VecOps::RVec<std::size_t> sortedPassedIndices(const ROOT::VecOps::RVec<TLV>& tlv,
                                                      const ROOT::VecOps::RVec<char>& selection);

  /**
   * @brief Return a vector of indices the elements that passed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<float>& pt,
                                               const std::vector<char>& selection);

  /**
   * @brief Return a vector of indices the elements that passed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection vector
   * @return ROOT::VecOps::RVec<std::size_t>
   */
  ROOT::VecOps::RVec<std::size_t> sortedPassedIndices(const ROOT::VecOps::RVec<float>& pt,
                                                      const ROOT::VecOps::RVec<char>& selection);

  /**
   * @brief Return a vector of indices of the elements that passed a selection sorted by pT
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
   * @brief Return a vector of indices of the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector
   * @return ROOT::VecOps::RVec<std::size_t>
   */
  ROOT::VecOps::RVec<std::size_t> sortedPassedIndices(const ROOT::VecOps::RVec<TLV>& tlv,
                                                      const ROOT::VecOps::RVec<char>& selection1,
                                                      const ROOT::VecOps::RVec<char>& selection2);

  /**
   * @brief Return a vector of indices the the elements that passed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<float>& pt,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

  /**
   * @brief Return a vector of indices the the elements that passed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector
   * @return ROOT::VecOps::RVec<std::size_t>
   */
  ROOT::VecOps::RVec<std::size_t> sortedPassedIndices(const ROOT::VecOps::RVec<float>& pt,
                                                      const ROOT::VecOps::RVec<char>& selection1,
                                                      const ROOT::VecOps::RVec<char>& selection2);

  /**
   * @brief Number of objects passing a selection and minimum pT requirement
   *
   * @param tlv LorentzVectors
   * @param minPt minimum pT
   * @param selection1 first selection
   * @param selection2 second selection
   * @return std::size_t
   */
  std::size_t numberOfObjects(const ROOT::VecOps::RVec<TLV>& tlv,
                              const float minPt,
                              const ROOT::VecOps::RVec<char>& selection1,
                              const ROOT::VecOps::RVec<char>& selection2);

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
   * @param tlv LorentzVectors
   * @param minPt minimum pT
   * @param selection selection
   * @return std::size_t
   */
  std::size_t numberOfObjects(const ROOT::VecOps::RVec<TLV>& tlv,
                              const float minPt,
                              const ROOT::VecOps::RVec<char>& selection);

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
   * @param selection selection
   * @return std::size_t
   */
  std::size_t numberOfObjects(const ROOT::VecOps::RVec<float>& pts,
                              const float minPt,
                              const ROOT::VecOps::RVec<char>& selection);

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
   * @brief Number of objects passing a selection and minimum pT requirement
   *
   * @param pts Pt of the objects
   * @param minPt minimum pT
   * @param selection1 selection1
   * @param selection2 selection2
   * @return std::size_t
   */
  std::size_t numberOfObjects(const ROOT::VecOps::RVec<float>& pts,
                              const float minPt,
                              const ROOT::VecOps::RVec<char>& selection1,
                              const ROOT::VecOps::RVec<char>& selection2);

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

  /**
   * @brief Get vector based on given set of indices
   *
   * @tparam T
   * @param vector original vector
   * @param indices insides
   * @return ROOT::VecOps::RVec<T>
   */
  template<typename T>
  ROOT::VecOps::RVec<T> vectorFromIndices(const ROOT::VecOps::RVec<T>& vector,
                                          const ROOT::VecOps::RVec<std::size_t>& indices) {
    return ROOT::VecOps::Take(vector, indices);
  }

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that fail the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection the selection that is to be failed
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedVectorFailSel(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection);


  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that failed the selection while passing OR and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @param passedOR
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedVectorPassOneFailOneSel(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection,
                                      const std::vector<char>& passedOR);


  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that passed one selection, OR, and failed the other and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection1
   * @param passedSelection2
   * @param passedOR
   * @return std::vector<TLV>
   */

  std::vector<TLV> sortedVectorPassTwoFailOneSel(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection1,
                                      const std::vector<char>& passedSelection2,
                                      const std::vector<char>& passedOR);

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that failed two selections, but passed OR and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection1
   * @param passedSelection2
   * @param passedOR
   * @return std::vector<TLV>
   */

  std::vector<TLV> sortedVectorPassOneFailTwoSel(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection1,
                                      const std::vector<char>& passedSelection2,
                                      const std::vector<char>& passedOR);

  /**
   * @brief Return a vector of indices of the elements that failed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesFailSel(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection);

  /**
   * @brief Return a vector of indices the elements that failed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesFailSel(const std::vector<float>& pt,
                                               const std::vector<char>& selection);

  /**
   * @brief Return a vector of indices of the elements that failed selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection1 vector (failed selection)
   * @param selection Selection2 vector (failed selection)
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesFailSel(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

  /**
   * @brief Return a vector of indices of the elements that failed a selection and passed the other sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector (failed selection)
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesPassOneFailOneSel(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

  /**
   * @brief Return a vector of indices the the elements that failed a selection sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection1 vector (failed selection)
   * @param selection Selection2 vector (failed selection)
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesFailSel(const std::vector<float>& pt,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

  /**
   * @brief Return a vector of indices the the elements that passed a selection and failed the other sorted by pT
   *
   * @param pt pt of the object
   * @param selection Selection1 vector
   * @param selection Selection2 vector (failed selection)
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedIndicesPassOneFailOneSel(const std::vector<float>& pt,
                                               const std::vector<char>& selection1,
                                               const std::vector<char>& selection2);

};
