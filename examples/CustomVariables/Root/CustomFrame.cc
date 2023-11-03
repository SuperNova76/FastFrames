#include "CustomFrame/CustomFrame.h"

#include "FastFrames/UniqueSampleID.h"

ROOT::RDF::RNode CustomFrame::defineVariables(ROOT::RDF::RNode mainNode,
                                              const UniqueSampleID& /*id*/) {

  // You can also use the UniqueSampleID object to apply a custom defione
  // based on the sample:
  //   id.dsid() returns sample DSID
  //   id.campaign() returns sample campaign
  //   id.simulation() return simulation flavour
  // You can use it in your functions to apply only per sample define

  auto LeadingJetPt = [this](const std::vector<ROOT::Math::PtEtaPhiEVector>& jets,
                         const std::vector<char>& passed) {
    auto sortedJets = this->sortedPassedVector(jets, passed);
    return sortedJets.at(0).pt();    
  };

  // we just wrap around the function into a lambda
  auto Select = [this](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                       const std::vector<char>& selected,
                       const std::vector<char>& btagged) {

    return this->CustomFrame::passes4Jets50GeV1Btag(fourVec, selected, btagged);

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
                                                    const UniqueSampleID& /*id*/) {

  auto LeadingElectronPtTight = [this](const std::vector<ROOT::Math::PtEtaPhiEVector>& electrons,
                                       const std::vector<char>& passed,
                                       const std::vector<char>& passedTight) {
    
    auto sortedElectrons = this->sortedPassedVector(electrons, passed, passedTight);
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


bool CustomFrame::passes4Jets50GeV1Btag(const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                                        const std::vector<char>& selected,
                                        const std::vector<char>& btagged) const {

  // we only want to select events where we have at least 4 jets, each with 50 GeV and at least one btag

  // first get the ordered indices of the selected jets (that passed the current selection)

  std::vector<std::size_t> sortedIndices = this->sortedPassedIndices(fourVec, selected);

  // now get the ordered vectors for four momentum and btagging
  std::vector<ROOT::Math::PtEtaPhiEVector> sortedTLV = this->vectorFromIndices(fourVec, sortedIndices);
  std::vector<char> sortedTag = this->vectorFromIndices(btagged, sortedIndices);

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



/// --------------------------------------------------------------------------
/// Other helper methods that can be used
/// --------------------------------------------------------------------------
std::vector<TLV> CustomFrame::sortedPassedVector(const std::vector<TLV>& tlv,
                                                 const std::vector<char>& passedSelection,
                                                 const std::vector<char>& passedOR) const {

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

std::vector<TLV> CustomFrame::sortedPassedVector(const std::vector<TLV>& tlv,
                                                 const std::vector<char>& passedSelection) const {

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

std::vector<std::size_t> CustomFrame::sortedPassedIndices(const std::vector<TLV>& tlv,
                                                          const std::vector<char>& selection) const {


  // some checkes first
  const std::size_t size = tlv.size();
  if (size != selection.size()) {
    LOG(ERROR) << "Sizes of the vectors do not match!\n";
    throw std::invalid_argument("");
  }

  std::vector<std::pair<double, std::size_t> > forSorting;

  for (std::size_t i = 0; i < size; ++i) {
    if (!selection.at(i)) continue;
    forSorting.emplace_back(std::make_pair(tlv.at(i).pt(), i));
  }

  std::sort(forSorting.begin(), forSorting.end(), [](const auto& element1, const auto& element2){return element1.first > element2.first;});

  std::vector<std::size_t> result;
  for (const auto& ielement : forSorting) {
    result.emplace_back(ielement.second);
  }
  return result;
}

std::vector<std::size_t> CustomFrame::sortedPassedIndices(const std::vector<TLV>& tlv,
                                                          const std::vector<char>& selection1,
                                                          const std::vector<char>& selection2) const {

  // some checkes first
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
