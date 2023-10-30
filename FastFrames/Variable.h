#pragma once

#include "FastFrames/Binning.h"

#include <string>

class Variable {
public:
  explicit Variable(const std::string& name) noexcept;

  Variable()  = delete;

  ~Variable() = default;

  inline const std::string& name() const {return m_name;}

  inline void setDefinition(const std::string& definition) {m_definition = definition;}

  inline const std::string& definition() const {return m_definition;}

  inline void setTitle(const std::string& title) {m_title = title;}

  inline const std::string& title() const {return m_title;}

  inline void setBinning(const double min, const double max, const int nbins) {
    m_binning.setBinning(min, max, nbins);
  }

  inline void setBinning(const std::vector<double>& edges) {
    m_binning.setBinning(edges);
  }

  inline bool hasRegularBinning() const {return m_binning.hasRegularBinning();}

  inline const std::vector<double>& binEdges() const {return m_binning.binEdges();}

  inline double axisMin() const {return m_binning.min();}
  inline double axisMax() const {return m_binning.max();}
  inline int axisNbins() const {return m_binning.nbins();}

private:
  std::string m_name;
  std::string m_definition;
  std::string m_title;
  Binning m_binning;
};
