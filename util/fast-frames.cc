/**
 * @file fast-frames.cc
 * @brief main c++ executable
 *
 */

/*! \mainpage FastFrames
 * Documentation of the FastFrames code
 *
 */

#include "FastFrames/ConfigSetting.h"
#include "FastFrames/Logger.h"
#include "FastFrames/FastFramesExecutor.h"

#include "TClass.h"
#include "TSystem.h"

#include <exception>
#include <memory>

/**
 * @brief c++ main executable
 *
 */
int main (int /*argc*/, const char** /*argv*/) {

  Logger::get().setLogLevel(LoggingLevel::VERBOSE);

  auto config = std::make_shared<ConfigSetting>();

  config->setTestingValues();

  FastFramesExecutor executor(config);

  executor.runFastFrames();

  return 0;
}
