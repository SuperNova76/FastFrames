#pragma once

#include <iostream>
#include <string>

class UniqueSampleID {
public:

  UniqueSampleID(const int dsid, const std::string& campaign, const std::string& simulation);

  ~UniqueSampleID() = default;

  bool operator == (const UniqueSampleID& rhs) {
    return m_dsid == rhs.m_dsid && m_campaign == rhs.m_campaign && m_simulation == rhs.m_simulation;
  }

  bool operator < (const UniqueSampleID& other) const {
    if (m_dsid != other.dsid()) {
      return m_dsid < other.dsid(); 
    }
    if (m_campaign != other.campaign()) {
      return m_campaign < other.campaign(); 
    }
    return m_simulation < other.simulation(); 
  }

  friend std::ostream& operator<<(std::ostream& os, const UniqueSampleID& id) {
    os << id.dsid() << " " << id.campaign() << " " << id.simulation();
    return os;
  }

  int dsid() const {return m_dsid;}

  const std::string& campaign() const {return m_campaign;}

  const std::string& simulation() const {return m_simulation;}

private:
  int m_dsid;
  std::string m_campaign;
  std::string m_simulation;
 
};
