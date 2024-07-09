/**
 * @file DuplicateEventsChecker.h
 * @brief Header file for the DuplicateEventChecker class
 *
 */

#pragma once

#include "TChain.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

/**
 * @brief Class to check if a given list of files contains events with the same runNumber and eventNumbers
 *
 */
class DuplicateEventChecker {
  public:

    /**
     * @brief Construct a new Duplicate Event Checker object
     *
     * @param files
     */
    explicit DuplicateEventChecker(const std::vector<std::string>& files) :
      m_files(files) {
    }

    /**
     * @brief Deleted default constructor
     *
     */
    explicit DuplicateEventChecker() = delete;

    /**
     * @brief Destroy the Duplicate Event Checker object
     *
     */
    ~DuplicateEventChecker() = default;

    /**
     * @brief Check the TChainfor events that have the same runNumber and eventNumber
     *
     */
    void checkDuplicateEntries() {
      m_processedEvents.clear();
      m_duplicateRunNumbers.clear();
      m_duplicateEventNumbers.clear();

      TChain chain("reco");
      for (const auto& ifile : m_files) {
        chain.AddFile(ifile.c_str());
      }

      unsigned int runNumber;
      unsigned long long eventNumber;

      chain.SetBranchAddress("runNumber", &runNumber);
      chain.SetBranchAddress("eventNumber", &eventNumber);

      for (auto ievent = 0; ievent < chain.GetEntries(); ++ievent) {
        chain.GetEntry(ievent);
        if (isDuplicate(runNumber, eventNumber)) {
          auto itr = std::find(m_duplicatePairs.begin(), m_duplicatePairs.end(), std::make_pair(runNumber, eventNumber));

          // only add unique combination of run numbers and event numbers
          if (itr == m_duplicatePairs.end()) {
            m_duplicateRunNumbers.emplace_back(runNumber);
            m_duplicateEventNumbers.emplace_back(eventNumber);
            m_duplicatePairs.emplace_back(std::make_pair(runNumber, eventNumber));
          }
        }
      }
    }

    /**
     * @brief Get the list of duplicate runNumbers
     *
     * @return const std::vector<unsigned int>&
     */
    std::vector<unsigned int> duplicateRunNumbers() const {return m_duplicateRunNumbers;}

    /**
     * @brief Get the list of duplicate eventNumbers
     *
     * @return const std::vector<unsigned long long>&
     */
    std::vector<unsigned long long> duplicateEventNumbers() const {return m_duplicateEventNumbers;}

  private:

    /**
     * @brief Check if the event is duplicate by adding it to the set and checking if it was already there
     *
     * @param runNumber
     * @param eventNumber
     * @return true
     * @return false
     */
    bool isDuplicate(const unsigned int runNumber, const unsigned long long eventNumber) {
      return ! (m_processedEvents.insert(std::make_pair(runNumber, eventNumber)).second);
    }

    std::vector<std::string> m_files;

    std::vector<unsigned int> m_duplicateRunNumbers;
    std::vector<unsigned long long> m_duplicateEventNumbers;
    std::vector<std::pair<unsigned int, unsigned long long> > m_duplicatePairs;

    std::set<std::pair<unsigned int, unsigned long long> > m_processedEvents;
};
