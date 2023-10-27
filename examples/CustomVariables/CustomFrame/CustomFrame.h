#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"

#include "ROOT/RDataFrame.hxx"

#include <memory>

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame(const std::shared_ptr<ConfigSetting>& config);

  virtual ~CustomFrame();

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDataFrame& df) override final;

private:

};
