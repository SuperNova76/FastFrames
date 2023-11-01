#pragma once

#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/DefineHelpers.h"

#include "Math/Vector4D.h"
#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>

class UniqueSampleID;

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame() = default;

  virtual ~CustomFrame() = default;

  virtual void init() override final {MainFrame::init();}

  virtual void executeHistograms() override final {MainFrame::executeHistograms();}

  virtual ROOT::RDF::RNode defineVariables(ROOT::RDF::RNode mainNode,
                                           const UniqueSampleID& id) override final;

private:

  bool passes4Jets50GeV1Btag(const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                             const std::vector<char>& selected,
                             const std::vector<char>& btagged) const;


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
