/**
 * @file HistoContainer.h
 * @brief File containing classes for hsitogram storage
 *
 */

#pragma once

#include "TH1D.h"
#include "TH2D.h"

#include "ROOT/RResultPtr.hxx"

#include <memory>
#include <vector>
#include <string>

/**
 * @brief Class that stores histograms for each Variable
 *
 */
class VariableHisto {
public:

  /**
   * @brief Construct a new Variable Histo object
   *
   * @param name Name of the Variable
   */
  explicit VariableHisto(const std::string& name) :
    m_name(name) {}

  /**
   * @brief Destroy the Variable Histo object
   *
   */
  ~VariableHisto() = default;

  /**
   * @brief Deleted copy constructor
   *
   * @param other
   */
  VariableHisto(const VariableHisto& other) = delete;

  /**
   * @brief Move constructor
   *
   * @param other
   */
  VariableHisto(VariableHisto&& other) = default;

  /**
   * @brief Deleted assignment operator
   *
   * @param other
   * @return VariableHisto&
   */
  VariableHisto& operator =(const VariableHisto& other) = delete;

  /**
   * @brief Default forwarding operator
   *
   * @param other
   * @return VariableHisto&
   */
  VariableHisto& operator =(VariableHisto&& other) = default;

  /**
   * @brief Get name of the Variable
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set the histogram from the RDataFrame results object
   * This triggers the event loop!
   * Need to make a copy as RDF owns the pointer
   *
   * @param h
   */
  void setHisto(ROOT::RDF::RResultPtr<TH1D>& h) {m_histo = std::move(h);}

  /**
   * @brief Get the histogram
   *
   * @return ROOT::RDF::RResultPtr<TH1D>
   */
  inline ROOT::RDF::RResultPtr<TH1D> histo() const {return m_histo;}

  /**
   * @brief Get the unique ptr histogram
   * 
   * @return const std::unique_ptr<TH1D>& 
   */
  inline const std::unique_ptr<TH1D>& histoUniquePtr() const {return m_histoUniquePtr;}

  /**
   * @brief Merge histograms (add them)
   *
   * @param h Other histogram
   */
  void mergeHisto(ROOT::RDF::RResultPtr<TH1D> h);

  /**
   * @brief Copy the RResultsPtr to the unique ptr 
   * 
   * @param h 
   * @return * void 
   */
  void copyHisto(ROOT::RDF::RResultPtr<TH1D> h);

private:
  std::string m_name;
  ROOT::RDF::RResultPtr<TH1D> m_histo;
  std::unique_ptr<TH1D> m_histoUniquePtr;

};

/**
 * @brief Class that stores 2D histograms for each Variable
 *
 */
class VariableHisto2D {
public:

  /**
   * @brief Construct a new Variable Histo 2D object
   *
   * @param name Name of the combination
   */
  explicit VariableHisto2D(const std::string& name) :
    m_name(name) {}

  /**
   * @brief Destroy the Variable Histo 2D object
   *
   */
  ~VariableHisto2D() = default;

  /**
   * @brief Deleted copy constructor
   *
   * @param other
   */
  VariableHisto2D(const VariableHisto2D& other) = delete;

  /**
   * @brief Move constructor
   *
   * @param other
   */
  VariableHisto2D(VariableHisto2D&& other) = default;

  /**
   * @brief Deleted assignment operator
   *
   * @param other
   * @return VariableHisto&
   */
  VariableHisto2D& operator =(const VariableHisto2D& other) = delete;

  /**
   * @brief Default forwarding operator
   *
   * @param other
   * @return VariableHisto&
   */
  VariableHisto2D& operator =(VariableHisto2D&& other) = default;

  /**
   * @brief Get name of the Variable
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set the histogram from the RDataFrame results object
   * This triggers the event loop!
   * Need to make a copy as RDF owns the pointer
   *
   * @param h
   */
  void setHisto(ROOT::RDF::RResultPtr<TH2D>& h) {m_histo = std::move(h);}

  /**
   * @brief Get the histogram
   *
   * @return ROOT::RDF::RResultPtr<TH1D>
   */
  inline ROOT::RDF::RResultPtr<TH2D> histo() const {return m_histo;}

  /**
   * @brief Get the unique ptr histogram
   * 
   * @return const std::unique_ptr<TH2D>& 
   */
  inline const std::unique_ptr<TH2D>& histoUniquePtr() const {return m_histoUniquePtr;}

  /**
   * @brief Merge histograms (add them)
   *
   * @param h Other histogram
   */
  void mergeHisto(ROOT::RDF::RResultPtr<TH2D> h);

  /**
   * @brief Copy the RResultsPtr to the unique ptr 
   * 
   * @param h 
   */
  void copyHisto(ROOT::RDF::RResultPtr<TH2D> h);

private:
  std::string m_name;
  ROOT::RDF::RResultPtr<TH2D> m_histo;
  std::unique_ptr<TH2D> m_histoUniquePtr;
};

/**
 * @brief Class that stores the VariableHistos for a Region
 *
 */
class RegionHisto {
public:

  /**
   * @brief Construct a new Region Histo object
   *
   * @param name Name of the region
   */
  explicit RegionHisto(const std::string& name) :
    m_name(name){}

  /**
   * @brief Destroy the Region Histo object
   *
   */
  ~RegionHisto() = default;

  /**
   * @brief Deleted copy constructor
   *
   */
  RegionHisto(const RegionHisto&) = delete;

  /**
   * @brief Default move constructor
   *
   */
  RegionHisto(RegionHisto&&) = default;

  /**
   * @brief Deleted assignment operator
   *
   * @return RegionHisto&
   */
  RegionHisto& operator=(const RegionHisto&) = delete;

  /**
   * @brief Default forwarding operator
   *
   * @return RegionHisto&
   */
  RegionHisto& operator=(RegionHisto&&) = default;

  /**
   * @brief Get name of the region
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Add VariableHisto to this region
   *
   * @param vh
   */
  inline void addVariableHisto(VariableHisto&& vh) {m_variables.emplace_back(std::move(vh));}

  /**
   * @brief Add VariableHisto2D for this region
   *
   * @param vh
   */
  inline void addVariableHisto2D(VariableHisto2D&& vh) {m_variables2D.emplace_back(std::move(vh));}

  /**
   * @brief Get all variableHisto (const)
   *
   * @return const std::vector<VariableHisto>&
   */
  inline const std::vector<VariableHisto>& variableHistos() const {return m_variables;}

  /**
   * @brief Get all variableHisto
   *
   * @return std::vector<VariableHisto>&
   */
  inline std::vector<VariableHisto>& variableHistos() {return m_variables;}

  /**
   * @brief Get all variableHisto2D (const)
   *
   * @return const std::vector<VariableHisto2D>&
   */
  inline const std::vector<VariableHisto2D>& variableHistos2D() const {return m_variables2D;}

  /**
   * @brief Get all variableHisto2D
   *
   * @return std::vector<VariableHisto2D>&
   */
  inline std::vector<VariableHisto2D>& variableHistos2D() {return m_variables2D;}

private:
  std::string m_name;
  std::vector<VariableHisto> m_variables;
  std::vector<VariableHisto2D> m_variables2D;

};

/**
 * @brief Class holding histograms for a given Systematic.
 * Stored RegionHistos
 *
 */
class SystematicHisto {
public:

  /**
   * @brief Construct a new Systematic Histo object
   *
   * @param name Name of the systematic
   */
  explicit SystematicHisto(const std::string& name) :
    m_name(name) {}

  /**
   * @brief Destroy the Systematic Histo object
   *
   */
  ~SystematicHisto() = default;

  /**
   * @brief Deleted copy constructor
   *
   */
  SystematicHisto(const SystematicHisto&) = delete;

  /**
   * @brief Default move constructor
   *
   */
  SystematicHisto(SystematicHisto&&) = default;

  /**
   * @brief Deleted assignment operator
   *
   * @return SystematicHisto&
   */
  SystematicHisto& operator=(const SystematicHisto&) = delete;

  /**
   * @brief Default forwarding operator
   *
   * @return SystematicHisto&
   */
  SystematicHisto& operator=(SystematicHisto&&) = default;

  /**
   * @brief Add RegionHisto to this Systematic
   *
   * @param rh
   */
  inline void addRegionHisto(RegionHisto&& rh) {m_regions.emplace_back(std::move(rh));}

  /**
   * @brief Get all RegionHistos
   *
   * @return const std::vector<RegionHisto>&
   */
  inline const std::vector<RegionHisto>& regionHistos() const {return m_regions;}

  /**
   * @brief Get name of the systematic
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Merge (add) SystematicHistos
   *
   * @param histo
   */
  void merge(const SystematicHisto& histo);

  /**
   * @brief Copy the histos without the RResultsPts (allows to free it) 
   * 
   * @return SystematicHisto 
   */
  SystematicHisto copy() const;

private:

  std::string m_name;
  std::vector<RegionHisto> m_regions;

};
