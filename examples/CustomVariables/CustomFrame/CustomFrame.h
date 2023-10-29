#pragma once

#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"

#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame(const std::shared_ptr<ConfigSetting>& config);

  explicit CustomFrame() = default;

  virtual ~CustomFrame();

  virtual void init() override final {MainFrame::init();}

  virtual void executeHistograms() override final {MainFrame::executeHistograms();}

  virtual ROOT::RDF::RNode defineVariables(const ROOT::RDataFrame& df) override final;

private:

  ClassDefOverride(CustomFrame, 1);

};
