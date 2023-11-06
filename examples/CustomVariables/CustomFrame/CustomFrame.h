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

  ClassDefOverride(CustomFrame, 1);

};
