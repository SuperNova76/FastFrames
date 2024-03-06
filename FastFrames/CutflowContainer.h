/**
 * @file CutflowContainer.h
 * @brief CutflowContainer header
 *
 */
#pragma once

#include "ROOT/RResultPtr.hxx"

#include "TH1D.h"

#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class holding the cutflow information from RDataFrame
 *
 */
class CutflowContainer {
public:

  /**
   * @brief Construct a new Cutflow Container object
   *
   * @param name
   */
  explicit CutflowContainer(const std::string& name) noexcept;

  /**
   * @brief Construct a new Cutflow Container object
   *
   */
  CutflowContainer() = delete;

  /**
   * @brief Destroy the Cutflow Container object
   *
   */
  ~CutflowContainer() = default;

  /**
   * @brief Construct a new Cutflow Container object
   *
   * @param other
   */
  CutflowContainer(const CutflowContainer& other) = delete;

  /**
   * @brief Construct a new Cutflow Container object
   *
   * @param other
   */
  CutflowContainer(CutflowContainer&& other) = default;

  /**
   * @brief Asignment operator
   *
   * @param other
   * @return * CutflowContainer&
   */
  CutflowContainer& operator =(const CutflowContainer& other) = delete;

  /**
   * @brief Move operator
   *
   * @param other
   * @return CutflowContainer&
   */
  CutflowContainer& operator =(CutflowContainer&& other) = default;

  /**
   * @brief Name of the container
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Add the Booked Yields object
   *
   * @param yields
   * @param error
   * @param title
   */
  void addBookedYields(const ROOT::RDF::RResultPtr<double>& yields,
                       const ROOT::RDF::RResultPtr<double>& error,
                       const std::string& title) {

    m_bookedYields.emplace_back(yields);
    m_bookedYieldErrors.emplace_back(error);
    m_titles.emplace_back(title);
  }

  /**
   * @brief Copy the values for cutflows from result pointers to values
   *
   * @param other
   */
  void copyValues(CutflowContainer& other);

  /**
   * @brief Add the results to existing values
   *
   * @param other
   */
  void mergeValues(CutflowContainer& other);

  /**
   * @brief Turn the values into histogram
   *
   * @return std::unique_ptr<TH1D>
   */
  std::unique_ptr<TH1D> cutflowHisto() const;

private:
  std::string m_name;
  std::vector<ROOT::RDF::RResultPtr<double> > m_bookedYields;
  std::vector<ROOT::RDF::RResultPtr<double> > m_bookedYieldErrors;
  std::vector<std::string> m_titles;
  std::vector<double> m_yields;
  std::vector<double> m_yieldErrors;
};
