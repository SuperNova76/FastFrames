#pragma once

#include "FastFrames/MainFrame.h"

#include "FastFrames/ConfigSetting.h"

#include "ROOT/RDataFrame.hxx"
#include "TClass.h"

#include <memory>

class UniqueSampleID;

class CustomFrame : public MainFrame {
public:

  explicit CustomFrame() = default;

  virtual ~CustomFrame() = default;

  virtual void init() override final {MainFrame::init();}

  virtual void executeHistograms() override final {MainFrame::executeHistograms();}

  virtual ROOT::RDF::RNode defineVariables(ROOT::RDF::RNode mainNode,
                                           const UniqueSampleID& id) override final;

private:

  ClassDefOverride(CustomFrame, 1);

};
