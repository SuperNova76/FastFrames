#pragma once

#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"

#include "Math/Vector4D.h"
#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>

using TLV = ROOT::Math::PtEtaPhiEVector;

class UniqueSampleID;

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame() = default;

  virtual ~CustomFrame() = default;

  virtual void init() override final {MainFrame::init();}

  virtual ROOT::RDF::RNode defineVariables(ROOT::RDF::RNode mainNode,
                                           const UniqueSampleID& id) override final;
  
  virtual ROOT::RDF::RNode defineVariablesNtuple(ROOT::RDF::RNode mainNode,
                                                 const UniqueSampleID& id) override final;

private:

  bool passes4Jets50GeV1Btag(const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                             const std::vector<char>& selected,
                             const std::vector<char>& btagged) const;


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
                                      const std::vector<char>& passedOR) const;

  /**
   * @brief Helper function to take TLorentzVector, select only elements
   * that paased the selection and sort the resulting vector based on pT
   *
   * @param tlv
   * @param passedSelection
   * @return std::vector<TLV>
   */
  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection) const;

  /**
   * @brief Return a vector of indices the elements that passed a selection sorted by pT
   *
   * @param tlv TLorentzVector for the object
   * @param selection Selection vector
   * @return std::vector<std::size_t>
   */
  std::vector<std::size_t> sortedPassedIndices(const std::vector<TLV>& tlv,
                                               const std::vector<char>& selection) const;

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
                                               const std::vector<char>& selection2) const;


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
                                   const std::vector<std::size_t>& indices) const {

    std::vector<T> result;
    for (const std::size_t index : indices) {
      result.emplace_back(vector.at(index));
    }

    return result;
  }
  ClassDefOverride(CustomFrame, 1);

};
