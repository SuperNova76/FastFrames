/**
 * @file Variable.cc
 * @brief Variable
 *
 */

#include "FastFrames/Variable.h"

Variable::Variable(const std::string& name) noexcept :
  m_name(name),
  m_definition(""),
  m_title("title"),
  m_binning(Binning()),
  m_isNominalOnly(false),
  m_type(VariableType::UNDEFINED)
{
}

ROOT::RDF::TH1DModel Variable::histoModel1D() const {
  if (m_binning.hasRegularBinning()) {
    return ROOT::RDF::TH1DModel("", m_title.c_str(), axisNbins(), axisMin(), axisMax());
  } else {
    const auto& edges = binEdges();
    return ROOT::RDF::TH1DModel("", m_title.c_str(), edges.size() - 1, edges.data());
  }
}