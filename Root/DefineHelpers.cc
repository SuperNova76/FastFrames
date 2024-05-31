/**
 * @file DefineHelpers.cc
 * @brief Helper functions for Define
 *
 */

#include "FastFrames/DefineHelpers.h"

#include "FastFrames/Logger.h"

#include <algorithm>

using ROOT::VecOps::RVec;

std::vector<TLV> DefineHelpers::sortedPassedVector(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (!passedSelection.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

RVec<TLV> DefineHelpers::sortedPassedVector(const RVec<TLV>& tlv,
                                            const RVec<char>& passedSelection) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs that pass selection
  RVec<TLV> result = tlv[passedSelection];
  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

std::vector<TLV> DefineHelpers::sortedPassedVector(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection,
                                                   const std::vector<char>& passedOR) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection.size() || size != passedOR.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (!passedSelection.at(i)) continue;
    if (!passedOR.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

RVec<TLV> DefineHelpers::sortedPassedVector(const RVec<TLV>& tlv,
                                            const RVec<char>& passedSelection,
                                            const RVec<char>& passedOR) {

  const auto selection = passedSelection && passedOR;
  return sortedPassedVector(tlv, selection);
}

std::vector<TLV> DefineHelpers::sortedPassedVector(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection1,
                                                   const std::vector<char>& passedSelection2,
                                                   const std::vector<char>& passedOR) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection1.size() || size != passedSelection2.size() || size != passedOR.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (!passedSelection1.at(i)) continue;
    if (!passedSelection2.at(i)) continue;
    if (!passedOR.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

RVec<TLV> DefineHelpers::sortedPassedVector(const RVec<TLV>& tlv,
                                            const RVec<char>& passedSelection1,
                                            const RVec<char>& passedSelection2,
                                            const RVec<char>& passedOR) {
  const auto selection = passedSelection1 && passedSelection2 && passedOR;
  return sortedPassedVector(tlv, selection);
}

std::vector<std::size_t> DefineHelpers::sortedPassedIndices(const std::vector<TLV>& tlv,
                                                            const std::vector<char>& selection) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t>> forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2) { return element1.first > element2.first; });

  std::vector<std::size_t> result;
  for (const auto &ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

RVec<std::size_t> DefineHelpers::sortedPassedIndices(const RVec<TLV>& tlv,
                                                     const RVec<char>& selection) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  auto sorted_indices = ROOT::VecOps::Argsort(tlv, [](const TLV& vec1, const TLV& vec2)
                                              { return vec1.pt() > vec2.pt(); });
  auto sorted_selection = ROOT::VecOps::Take(selection, sorted_indices);

  return sorted_indices[sorted_selection];
}

std::vector<std::size_t> DefineHelpers::sortedPassedIndices(const std::vector<float>& pt,
                                                            const std::vector<char>& selection) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection.at(i)) continue;
    forSorting.emplace_back(std::make_pair(pt.at(i), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

RVec<std::size_t> DefineHelpers::sortedPassedIndices(const RVec<float>& pt,
                                                     const RVec<char>& selection) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  auto sorted_indices = ROOT::VecOps::Argsort(pt, [](float x, float y)
                                              { return x > y; });
  auto sorted_selection = ROOT::VecOps::Take(selection, sorted_indices);

  return sorted_indices[sorted_selection];
}

std::vector<std::size_t> DefineHelpers::sortedPassedIndices(const std::vector<TLV>& tlv,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

RVec<std::size_t> DefineHelpers::sortedPassedIndices(const RVec<TLV>& tlv,
                                                     const RVec<char>& selection1,
                                                     const RVec<char>& selection2) {

  const auto selection = selection1 && selection2;
  return sortedPassedIndices(tlv, selection);
}

std::vector<std::size_t> DefineHelpers::sortedPassedIndices(const std::vector<float>& pt,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(pt.at(i), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

RVec<std::size_t> DefineHelpers::sortedPassedIndices(const RVec<float>& pt,
                                                     const RVec<char>& selection1,
                                                     const RVec<char>& selection2) {
  auto selection = selection1 && selection2;
  return sortedPassedIndices(pt, selection);
}

std::size_t DefineHelpers::numberOfObjects(const std::vector<TLV>& tlv,
                                           const float minPt,
                                           const std::vector<char>& selection) {

  std::size_t result(0);
  for (std::size_t i = 0; i < tlv.size(); ++i) {
    if (!selection.at(i)) continue;
    if (tlv.at(i).pt() < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const RVec<TLV>& tlv,
                                           const float minPt,
                                           const RVec<char>& selection) {

  std::size_t result(0);
  for (std::size_t i = 0; i < tlv.size(); ++i) {
    if (!selection.at(i)) continue;
    if (tlv.at(i).pt() < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const std::vector<TLV>& tlv,
                                           const float minPt,
                                           const std::vector<char>& selection1,
                                           const std::vector<char>& selection2) {

  std::size_t result(0);
  for (std::size_t i = 0; i < tlv.size(); ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    if (tlv.at(i).pt() < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const RVec<TLV>& tlv,
                                           const float minPt,
                                           const RVec<char>& selection1,
                                           const RVec<char>& selection2) {

  std::size_t result(0);
  for (std::size_t i = 0; i < tlv.size(); ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    if (tlv.at(i).pt() < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const std::vector<float>& pts,
                                           const float minPt,
                                           const std::vector<char>& selection) {

  std::size_t result(0);
  for (std::size_t i = 0; i < pts.size(); ++i) {
    if (!selection.at(i)) continue;
    if (pts.at(i) < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const RVec<float>& pts,
                                           const float minPt,
                                           const RVec<char>& selection) {

  std::size_t result(0);
  for (std::size_t i = 0; i < pts.size(); ++i) {
    if (!selection.at(i)) continue;
    if (pts.at(i) < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const std::vector<float>& pts,
                                           const float minPt,
                                           const std::vector<char>& selection1,
                                           const std::vector<char>& selection2) {
  std::size_t result(0);
  for (std::size_t i = 0; i < pts.size(); ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    if (pts.at(i) < minPt) continue;
    ++result;
  }

  return result;
}

std::size_t DefineHelpers::numberOfObjects(const RVec<float>& pts,
                                           const float minPt,
                                           const RVec<char>& selection1,
                                           const RVec<char>& selection2) {
  std::size_t result(0);
  for (std::size_t i = 0; i < pts.size(); ++i) {
    if (!selection1.at(i)) continue;
    if (!selection2.at(i)) continue;
    if (pts.at(i) < minPt) continue;
    ++result;
  }

  return result;
}

std::vector<TLV> DefineHelpers::sortedVectorFailSel(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (passedSelection.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

std::vector<TLV> DefineHelpers::sortedVectorPassOneFailOneSel(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection,
                                                   const std::vector<char>& passedOR) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection.size() || size != passedOR.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (passedSelection.at(i)) continue;
    if (!passedOR.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

std::vector<TLV> DefineHelpers::sortedVectorPassTwoFailOneSel(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection1,
                                                   const std::vector<char>& passedSelection2,
                                                   const std::vector<char>& passedOR) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection1.size() || size != passedSelection2.size() || size != passedOR.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (!passedSelection1.at(i)) continue;
    if (passedSelection2.at(i)) continue;
    if (!passedOR.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

std::vector<TLV> DefineHelpers::sortedVectorPassOneFailTwoSel(const std::vector<TLV>& tlv,
                                                   const std::vector<char>& passedSelection1,
                                                   const std::vector<char>& passedSelection2,
                                                   const std::vector<char>& passedOR) {

  // do some checks first
  const std::size_t size = tlv.size();
  if (size != passedSelection1.size() || size != passedSelection2.size() || size != passedOR.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  // select TLVs
  std::vector<TLV> result;
  for (std::size_t i = 0; i < size; ++i) {
    if (passedSelection1.at(i)) continue;
    if (passedSelection2.at(i)) continue;
    if (!passedOR.at(i)) continue;

    result.emplace_back(tlv.at(i));
  }

  // sort them based on pT
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2) { return v1.pt() > v2.pt(); });

  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesFailSel(const std::vector<TLV>& tlv,
                                                            const std::vector<char>& selection) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t>> forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (selection.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2) { return element1.first > element2.first; });

  std::vector<std::size_t> result;
  for (const auto &ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesFailSel(const std::vector<float>& pt,
                                                            const std::vector<char>& selection) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (selection.at(i)) continue;
    forSorting.emplace_back(std::make_pair(pt.at(i), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesFailSel(const std::vector<TLV>& tlv,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (selection1.at(i)) continue;
    if (selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesPassOneFailOneSel(const std::vector<TLV>& tlv,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checks first
  const std::size_t size = tlv.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection1.at(i)) continue;
    if (selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesFailSel(const std::vector<float>& pt,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (selection1.at(i)) continue;
    if (selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(pt.at(i), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> DefineHelpers::sortedIndicesPassOneFailOneSel(const std::vector<float>& pt,
                                                            const std::vector<char>& selection1,
                                                            const std::vector<char>& selection2) {

  // some checkes first
  const std::size_t size = pt.size();
  if (size != selection1.size() || size != selection2.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection1.at(i)) continue;
    if (selection2.at(i)) continue;
    forSorting.emplace_back(std::make_pair(pt.at(i), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}
