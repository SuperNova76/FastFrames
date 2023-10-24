#pragma once

#include <string>

class UniqueSampleID {
public:

  UniqueSampleID(const int dsid, const std::string& campaign, const std::string& simulation);

  ~UniqueSampleID() = default;

  bool operator == (const UniqueSampleID& rhs) {
    return m_dsid == rhs.m_dsid && m_campaign == rhs.m_campaign && m_simulation == rhs.m_simulation;
  }

private:
  int m_dsid;
  std::string m_campaign;
  std::string m_simulation;
 
};
