#pragma once

#include "Math/Vector4D.h"

#include <vector>

using TLV = ROOT::Math::PtEtaPhiEVector;

namespace DefineHelpers {

  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection,
                                      const std::vector<char>& passedOR);

  std::vector<TLV> sortedPassedVector(const std::vector<TLV>& tlv,
                                      const std::vector<char>& passedSelection);
}
