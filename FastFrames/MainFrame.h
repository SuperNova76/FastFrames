#pragma once

#include "FastFrames/MetadataManager.h"

class MainFrame {
public:
  explicit MainFrame();

  virtual ~MainFrame() = default;

  virtual void init();

  virtual void execute();

private:
  MetadataManager m_metadataManager;

};
