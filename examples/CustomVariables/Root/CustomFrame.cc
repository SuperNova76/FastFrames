#include "CustomFrame/CustomFrame.h"

#include "FastFrames/DefineHelpers.h"
#include "FastFrames/UniqueSampleID.h"

ROOT::RDF::RNode CustomFrame::defineVariables(ROOT::RDF::RNode mainNode,
                                              const std::shared_ptr<Sample>& /*sample*/,
                                              const UniqueSampleID& /*id*/) {

  // You can also use the UniqueSampleID object to apply a custom defione
  // based on the sample and the subsample
  //   sample->name(): is the name of the sample defined in the config
  //   id.dsid() returns sample DSID
  //   id.campaign() returns sample campaign
  //   id.simulation() return simulation flavour
  // You can use it in your functions to apply only per sample define

  auto LeadingJetPt = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& jets,
                         const std::vector<char>& passed) {
    auto sortedJets = DefineHelpers::sortedPassedVector(jets, passed);
    if (sortedJets.empty()) return -1.;
    return sortedJets.at(0).pt();
  };

  // we just wrap around the function into a lambda
  auto Select = [this](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                       const std::vector<char>& selected,
                       const std::vector<char>& btagged) {

    return this->passes4Jets50GeV1Btag(fourVec, selected, btagged);

  };

  LOG(INFO) << "Adding variable: leading_jet_pt_NOSYS\n";
  // add leading jet pT
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "leading_jet_pt_NOSYS", // name of the new column
                                         LeadingJetPt, // functor (function that is called)
                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS"}); // what it depends on

  LOG(INFO) << "Adding variable: passed_4j50GeV_1btag_NOSYS\n";
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "passed_4j50GeV_1btag_NOSYS",
                                         Select,
                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS", "jet_DL1dv01_FixedCutBEff_77_select"});

  return mainNode;
}

ROOT::RDF::RNode CustomFrame::defineVariablesNtuple(ROOT::RDF::RNode mainNode,
                                                    const std::shared_ptr<Sample>& /*sample*/,
                                                    const UniqueSampleID& /*id*/) {

  auto LeadingElectronPtTight = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& electrons,
                                   const std::vector<char>& passed,
                                   const std::vector<char>& passedTight) {

    auto sortedElectrons = DefineHelpers::sortedPassedVector(electrons, passed, passedTight);
    if (sortedElectrons.empty()) return -1.;
    return sortedElectrons.at(0).pt();
  };

  LOG(INFO) << "Adding variable: leading_tight_electron_pt_NOSYS\n";
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "leading_tight_electron_pt_NOSYS", // name of the new column
                                         LeadingElectronPtTight, // functor (function that is called)
                                         {"el_TLV_NOSYS", "el_select_or_NOSYS", "el_select_tight_NOSYS"}); // what it depends on

  return mainNode;
}

ROOT::RDF::RNode CustomFrame::defineVariablesTruth(ROOT::RDF::RNode node,
                                                   const std::string& treeName,
                                                   const std::shared_ptr<Sample>& /*sample*/,
                                                   const UniqueSampleID& /*sampleID*/) {

  if (treeName != "truth") return node; // do not do anything for other trees ("truth" = parton truth)
  // no systematics for truth
  // as in any Define, we can pass as a simple string that will be Just-In-Time compiled
  LOG(INFO) << "Adding truth variable: Ttbar_MC_ttbar_beforeFSR_pt_GeV\n";
  node = node.Define("Ttbar_MC_ttbar_beforeFSR_pt_GeV", "Ttbar_MC_ttbar_beforeFSR_pt/1e3");

  return node;
}

ROOT::RDF::RNode CustomFrame::defineVariablesNtupleTruth(ROOT::RDF::RNode node,
                                                         const std::string& /*treeName*/,
                                                         const std::shared_ptr<Sample>& /*sample*/,
                                                         const UniqueSampleID& /*sampleID*/) {
  return node;
}

ROOT::RDF::RNode CustomFrame::defineVariablesRegion(ROOT::RDF::RNode mainNode,
                                                    const std::shared_ptr<Sample>& /*sample*/,
                                                    const UniqueSampleID& /*id*/,
                                                    const std::string& /*regionName*/) {

  // ***** DO NOT USE NOW, USES TOO MUCH MEMORY AND CAN CRASH YOUR MACHINE *****
  // ***** IT IS BEING INVESTIGATED BY THE ROOT TEAM                       *****

  // now you can add new variables only for some regions

  //if (regionName == "myRegion") {
  //  mainNode = MainFrame::systematicDefine(mainNode,
  //                                         "leading_jet_pt_GeV",
  //                                         LeadingJetPt,
  //                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS"});
  //}

  return mainNode;
}


bool CustomFrame::passes4Jets50GeV1Btag(const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                                        const std::vector<char>& selected,
                                        const std::vector<char>& btagged) const {

  // we only want to select events where we have at least 4 jets, each with 50 GeV and at least one btag

  // first get the ordered indices of the selected jets (that passed the current selection)

  std::vector<std::size_t> sortedIndices = DefineHelpers::sortedPassedIndices(fourVec, selected);

  // now get the ordered vectors for four momentum and btagging
  std::vector<ROOT::Math::PtEtaPhiEVector> sortedTLV = DefineHelpers::vectorFromIndices(fourVec, sortedIndices);
  std::vector<char> sortedTag = DefineHelpers::vectorFromIndices(btagged, sortedIndices);

  // now we can loop over them and count number of correct indices
  std::size_t nJets(0);
  std::size_t nTags(0);
  for (std::size_t i = 0; i < sortedTLV.size(); ++i) {
    if (sortedTLV.at(i).pt() < 50e3) continue;

    nJets++;
    if (sortedTag.at(i)) nTags++;
  }

  return (nJets >= 4) && (nTags > 0);
}
