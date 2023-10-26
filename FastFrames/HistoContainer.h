#pragma once

#include "TH1D.h"

#include "ROOT/RDataFrame.hxx"

#include <memory>
#include <vector>
#include <string>

class VariableHisto {
public:
  VariableHisto(const std::string& name) :
    m_name(name) {}

  ~VariableHisto() {};

  VariableHisto(const VariableHisto& other) = delete;

  VariableHisto(VariableHisto&& other) = default;   

  VariableHisto& operator =(const VariableHisto& other) = delete;
  
  VariableHisto& operator =(VariableHisto&& other) = default;

  inline const std::string& name() const {return m_name;}

  void setHisto(ROOT::RDF::RResultPtr<TH1D>& h) {m_histo.reset(dynamic_cast<TH1D*>(h->Clone()));}

  inline const TH1D* histo() const {return m_histo.get();}

  void mergeHisto(const TH1D* h);

private:
  std::string m_name;
  std::unique_ptr<TH1D> m_histo;

};

class RegionHisto {
public:
  RegionHisto(const std::string& name) :
    m_name(name){}

  ~RegionHisto() = default;

  RegionHisto(const RegionHisto&) = delete;

  RegionHisto(RegionHisto&&) = default;

  RegionHisto& operator=(const RegionHisto&) = delete;

  RegionHisto& operator=(RegionHisto&&) = default;
  
  inline const std::string& name() const {return m_name;}

  inline void addVariableHisto(VariableHisto&& vh) {m_variables.emplace_back(std::move(vh));}

  inline const std::vector<VariableHisto>& variableHistos() const {return m_variables;}
  
  inline std::vector<VariableHisto>& variableHistos() {return m_variables;}

private:
  std::string m_name;
  std::vector<VariableHisto> m_variables;

};

class SystematicHisto {
public:
  SystematicHisto(const std::string& name) :
    m_name(name) {}

  ~SystematicHisto() = default;

  SystematicHisto(const SystematicHisto&) = delete;

  SystematicHisto(SystematicHisto&&) = default;

  SystematicHisto& operator=(const SystematicHisto&) = delete;

  SystematicHisto& operator=(SystematicHisto&&) = default;

  inline void addRegionHisto(RegionHisto&& rh) {m_regions.emplace_back(std::move(rh));}

  inline const std::vector<RegionHisto>& regionHistos() const {return m_regions;}

  inline const std::string& name() const {return m_name;}

  void merge(const SystematicHisto& histo);

private:

  std::string m_name;
  std::vector<RegionHisto> m_regions;

};
