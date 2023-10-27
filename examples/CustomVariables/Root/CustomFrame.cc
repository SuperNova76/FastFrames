#include "CustomFrame/CustomFrame.h"

CustomFrame::CustomFrame(const std::shared_ptr<ConfigSetting>& config) :
  MainFrame(config)
{
}

ROOT::RDF::RNode CustomFrame::defineVariables(const ROOT::RDataFrame& df) {

  return df;
}
