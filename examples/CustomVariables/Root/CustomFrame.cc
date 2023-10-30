#include "CustomFrame/CustomFrame.h"

#include "FastFrames/DefineHelpers.h"
#include "FastFrames/UniqueSampleID.h"


ROOT::RDF::RNode CustomFrame::defineVariables(ROOT::RDF::RNode mainNode,
                                              const UniqueSampleID& id) {

  auto LeadingJetPt = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& jets,
                         const std::vector<char>& passed) {
    auto sortedJets = DefineHelpers::sortedPassedVector(jets, passed);
    return sortedJets.at(0).pt();    
  };

  mainNode = MainFrame::systematicDefine(mainNode,
                                         "leading_jet_pt_NOSYS",
                                         LeadingJetPt,
                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS"});

  return mainNode;
}
