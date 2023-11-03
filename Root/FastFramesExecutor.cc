/**
 * @file FastFramesExecutor.cc
 * @brief Main executor wrapper
 *
 */

#include "FastFrames/FastFramesExecutor.h"

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Logger.h"
#include "FastFrames/MainFrame.h"

#include "ROOT/RDataFrame.hxx" // for implicit MT
#include "TClass.h"
#include "TSystem.h"

#include <exception>
#include <string>

FastFramesExecutor::FastFramesExecutor(const std::shared_ptr<ConfigSetting>& config) noexcept :
m_config(config)
{
}

void FastFramesExecutor::runFastFrames() const {
    ROOT::EnableImplicitMT(m_config->numCPU());

    TClass* c(nullptr);
    std::unique_ptr<MainFrame> baseFrame(nullptr);

    const std::string& name = m_config->customFrameName();

    if (!name.empty()) {
      // read the custom code for adding variables
      LOG(INFO) << "Using custom class: " << name << "\n";
      const std::string libName = "lib" + name + ".so";
      gSystem->Load(libName.c_str());

      c = ::TClass::GetClass(name.c_str());
      if (!c) {
        LOG(ERROR) << "Cannot get class " << name << "\n";
        throw std::invalid_argument("");
      }
      baseFrame.reset(static_cast<MainFrame*>(c->New()));
      if (!baseFrame) {
        LOG(ERROR) << "Something went wrong with creating the custom class\n";
        throw std::invalid_argument("");
      }
    } else {
      baseFrame = std::make_unique<MainFrame>();
    }

    baseFrame->setConfig(m_config);

    baseFrame->init();

    if (m_runNtuples) {
      baseFrame->executeNtuples();
    } else {
      baseFrame->executeHistograms();
    }

    LOG(INFO) << "Finished running the code\n";
}
