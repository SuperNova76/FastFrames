/**
 * @file Variable.h
 * @brief Variable
 *
 */

#pragma once

#include "FastFrames/Binning.h"

#include "ROOT/RDF/HistoModels.hxx"

#include <string>

/**
 * @brief Class responsible for the varaible definition
 *
 */
class Variable {
public:

  /**
   * @brief Construct a new Variable object
   *
   * @param name Name of the variable
   */
  explicit Variable(const std::string& name) noexcept;

  /**
   * @brief Deleted default constructor
   *
   */
  Variable()  = delete;

  /**
   * @brief Destroy the Variable object
   *
   */
  ~Variable() = default;

  /**
   * @brief Get the name of the variable
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Set the Definition of the variable (the column name)
   *
   * @param definition
   */
  inline void setDefinition(const std::string& definition) {m_definition = definition;}

  /**
   * @brief Get the definition of the variable (column name)
   *
   * @return const std::string&
   */
  inline const std::string& definition() const {return m_definition;}

  /**
   * @brief Set the title of the axes (histo title;X axis title;Y axis title)
   *
   * @param title
   */
  inline void setTitle(const std::string& title) {m_title = title;}

  /**
   * @brief Get the title
   *
   * @return const std::string&
   */
  inline const std::string& title() const {return m_title;}

  /**
   * @brief Set the Binning object (constant)
   *
   * @param min axis min
   * @param max axis max
   * @param nbins number of bins
   */
  inline void setBinning(const double min, const double max, const int nbins) {
    m_binning.setBinning(min, max, nbins);
  }

  /**
   * @brief Set the Binning object (variable bin edges)
   *
   * @param edges Bin edges
   */
  inline void setBinning(const std::vector<double>& edges) {
    m_binning.setBinning(edges);
  }

  /**
   * @brief Tells if the binning is constant or not
   *
   * @return true
   * @return false
   */
  inline bool hasRegularBinning() const {return m_binning.hasRegularBinning();}

  /**
   * @brief Get the bin edges
   *
   * @return const std::vector<double>&
   */
  inline const std::vector<double>& binEdges() const {return m_binning.binEdges();}

  /**
   * @brief Get axis min
   *
   * @return double
   */
  inline double axisMin() const {return m_binning.min();}

  /**
   * @brief Get axis max
   *
   * @return double
   */
  inline double axisMax() const {return m_binning.max();}

  /**
   * @brief Get the number of bins
   *
   * @return int
   */
  inline int axisNbins() const {return m_binning.nbins();}

  /**
   * @brief Get the 1D histogram model that is needed by RDataFrame
   *
   * @return ROOT::RDF::TH1DModel
   */
  ROOT::RDF::TH1DModel histoModel1D() const;

  /**
   * @brief Set the Is Nominal Only flag
   *
   * @param flag
   */
  inline void setIsNominalOnly(const bool flag) {m_isNominalOnly = flag;}

  /**
   * @brief is nominal only?
   *
   * @return true
   * @return false
   */
  inline bool isNominalOnly() const {return m_isNominalOnly;}

private:
  std::string m_name;
  std::string m_definition;
  std::string m_title;
  Binning m_binning;
  bool m_isNominalOnly;
};
