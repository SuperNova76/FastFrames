/**
 * @file UniqueSampleID.h
 * @brief Sample identifier
 *
 */

#pragma once

#include <iostream>
#include <string>

#include "FastFrames/StringOperations.h"

/**
 * @brief Class that allows to identify unique samples (DSID, campaign, simulation type)
 *
 */
class UniqueSampleID {
public:

  /**
   * @brief Construct a new Unique Sample I D object
   *
   * @param dsid
   * @param campaign
   * @param simulation
   */
  UniqueSampleID(const int dsid, const std::string& campaign, const std::string& simulation) noexcept;

  /**
   * @brief Destroy the Unique Sample I D object
   *
   */
  ~UniqueSampleID() = default;

  /**
   * @brief Needed to match two UniqueSampleIDs in a map
   *
   * @param rhs
   * @return true
   * @return false
   */
  bool operator == (const UniqueSampleID& rhs) {
    return m_dsid == rhs.m_dsid && m_campaign == rhs.m_campaign && m_simulation == rhs.m_simulation;
  }

  /**
   * @brief Needed only for the std::map key definition
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator < (const UniqueSampleID& other) const {
    if (m_dsid != other.dsid()) {
      return m_dsid < other.dsid();
    }
    if (m_campaign != other.campaign()) {
      return m_campaign < other.campaign();
    }
    return m_simulation < other.simulation();
  }

  /**
   * @brief Allows to easily write DSID, campaign and simulation type to a stream
   *
   * @param os
   * @param id
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& os, const UniqueSampleID& id) {
    os << id.dsid() << " " << id.campaign() << " " << id.simulation();
    return os;
  }

  /**
   * @brief Get DSID
   *
   * @return int
   */
  int dsid() const {return m_dsid;}

  /**
   * @brief  Get campaign
   *
   * @return const std::string&
   */
  const std::string& campaign() const {return m_campaign;}

  /**
   * @brief Get simulation type
   *
   * @return const std::string&
   */
  const std::string& simulation() const {return m_simulation;}

  /**
   * @brief Is sample data?
   *
   * @return true
   * @return false
   */
  bool isData() const {return StringOperations::compare_case_insensitive(m_simulation, "data");}

private:
  int m_dsid;
  std::string m_campaign;
  std::string m_simulation;

};
