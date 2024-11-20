#include "TutorialClass/TutorialClass.h"

#include "FastFrames/DefineHelpers.h"
#include "FastFrames/UniqueSampleID.h"

ROOT::RDF::RNode TutorialClass::defineVariables(ROOT::RDF::RNode mainNode,
                                                const std::shared_ptr<Sample>& /*sample*/,
                                                const UniqueSampleID& /*id*/) {

  // You can also use the UniqueSampleID object to apply a custom defione
  // based on the sample and the subsample
  //   sample->name(): is the name of the sample defined in the config
  //   id.dsid() returns sample DSID
  //   id.campaign() returns sample campaign
  //   id.simulation() return simulation flavour
  // You can use it in your functions to apply only per sample define
  //
  auto SortedTLVs = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                       const std::vector<char>& selected) {
    return DefineHelpers::sortedPassedVector(fourVec,selected);
  };

  auto LeadingTLV = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec) {
    return fourVec.empty() ? ROOT::Math::PtEtaPhiEVector{-999, -999, -999, -999} : fourVec.at(0);
  };

  auto tlvPtGEV = [](const ROOT::Math::PtEtaPhiEVector& tlv) {
    return tlv.pt()/1.e3;
  };

  // add sorted passed jet TLV vector
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "sorted_jet_TLV_NOSYS",
                                         SortedTLVs,
                                         {"jet_TLV_NOSYS", "jet_select_baselineJvt_NOSYS"});

  // add leading jet TLV
  mainNode = MainFrame::systematicDefine(mainNode,
                                        "jet1_TLV_NOSYS",
                                        LeadingTLV,
                                        {"sorted_jet_TLV_NOSYS"});

  mainNode = MainFrame::systematicDefine(mainNode,
                                         "jet1_pt_GEV_NOSYS",
                                         tlvPtGEV,
                                         {"jet1_TLV_NOSYS"});


  return mainNode;
}

ROOT::RDF::RNode TutorialClass::defineVariablesNtuple(ROOT::RDF::RNode mainNode,
                                                      const std::shared_ptr<Sample>& /*sample*/,
                                                      const UniqueSampleID& /*id*/) {

  return mainNode;
}

ROOT::RDF::RNode TutorialClass::defineVariablesTruth(ROOT::RDF::RNode node,
                                                     const std::string& /*sample*/,
                                                     const std::shared_ptr<Sample>& /*sample*/,
                                                     const UniqueSampleID& /*sampleID*/) {
  return node;
}

ROOT::RDF::RNode TutorialClass::defineVariablesNtupleTruth(ROOT::RDF::RNode node,
                                                           const std::string& /*treeName*/,
                                                           const std::shared_ptr<Sample>& /*sample*/,
                                                           const UniqueSampleID& /*sampleID*/) {
  return node;
}
  
