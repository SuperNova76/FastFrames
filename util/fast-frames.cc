#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Logger.h"
#include "FastFrames/MainFrame.h"

#include <memory>

int main (int argc, const char **argv) {

  Logger::get().setLogLevel(LoggingLevel::INFO);

  auto config = std::make_shared<ConfigSetting>();

  MainFrame frame{config};

  frame.init();

  frame.execute();

  LOG(INFO) << "Finished running the code\n";

  return 0;
}
