#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Logger.h"
#include "FastFrames/FastFramesExecutor.h"

#include "TClass.h"
#include "TSystem.h"

#include <exception>
#include <memory>

int main (int /*argc*/, const char** /*argv*/) {

  Logger::get().setLogLevel(LoggingLevel::DEBUG);

  auto config = std::make_shared<ConfigSetting>();

  FastFramesExecutor executor(config);

  executor.runFastFrames();

  return 0;
}
