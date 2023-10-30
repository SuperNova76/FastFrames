#include "FastFrames/DefineHelpers.h"
#include "FastFrames/Logger.h"

#include <algorithm>
#include <exception>

using namespace DefineHelpers;

std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
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
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2){return v1.pt() > v2.pt();});

  return result;
}

std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
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
  std::sort(result.begin(), result.end(), [](const TLV& v1, const TLV& v2){return v1.pt() > v2.pt();});

  return result;
}