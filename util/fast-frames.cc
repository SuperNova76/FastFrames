#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Logger.h"
#include "FastFrames/MainFrame.h"

#include "TClass.h"
#include "TSystem.h"

#include <exception>
#include <memory>

int main (int /*argc*/, const char** /*argv*/) {

  Logger::get().setLogLevel(LoggingLevel::DEBUG);

  auto config = std::make_shared<ConfigSetting>();

  ROOT::EnableImplicitMT(config->numCPU());

  TClass* c(nullptr);
  std::unique_ptr<MainFrame> baseFrame(nullptr);

  //const std::string name = "CustomFrame";
  const std::string name = "";

  if (!name.empty()) {
    // read the custom code for adding variables
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

  baseFrame->setConfig(config);

  baseFrame->init();

  baseFrame->executeHistograms();

  LOG(INFO) << "Finished running the code\n";

  return 0;
}
