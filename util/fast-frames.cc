#include "FastFrames/Logger.h"
#include "FastFrames/MainFrame.h"

int main (int argc, const char **argv) {

  Logger::get().setLogLevel(LoggingLevel::INFO);

  MainFrame frame{};

  frame.init();

  frame.execute();

  LOG(INFO) << "Finished running the code\n";

  return 0;
}
