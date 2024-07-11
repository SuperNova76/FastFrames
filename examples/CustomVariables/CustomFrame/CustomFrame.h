#pragma once

#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Sample.h"
#include "FastFrames/UniqueSampleID.h"

#include "Math/Vector4D.h"
#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>
#include <string>

using TLV = ROOT::Math::PtEtaPhiEVector;

class UniqueSampleID;

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame() = default;

  virtual ~CustomFrame() = default;

  virtual void init() override final {MainFrame::init();}

  virtual ROOT::RDF::RNode defineVariables(ROOT::RDF::RNode mainNode,
                                           const std::shared_ptr<Sample>& sample,
                                           const UniqueSampleID& id) override final;

  virtual ROOT::RDF::RNode defineVariablesRegion(ROOT::RDF::RNode mainNode,
                                                 const std::shared_ptr<Sample>& sample,
                                                 const UniqueSampleID& id,
                                                 const std::string& regionName) override final;

  virtual ROOT::RDF::RNode defineVariablesNtuple(ROOT::RDF::RNode mainNode,
                                                 const std::shared_ptr<Sample>& sample,
                                                 const UniqueSampleID& id) override final;

  virtual ROOT::RDF::RNode defineVariablesTruth(ROOT::RDF::RNode node,
                                                const std::string& treeName,
                                                const std::shared_ptr<Sample>& sample,
                                                const UniqueSampleID& sampleID) override final;

  virtual ROOT::RDF::RNode defineVariablesNtupleTruth(ROOT::RDF::RNode node,
                                                      const std::string& treeName,
                                                      const std::shared_ptr<Sample>& sample,
                                                      const UniqueSampleID& sampleID) override final;
private:

  bool passes4Jets50GeV1Btag(const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                             const std::vector<char>& selected,
                             const std::vector<char>& btagged) const;

  ClassDefOverride(CustomFrame, 1);

};
