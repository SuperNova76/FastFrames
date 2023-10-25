#include "FastFrames/MainFrame.h"

#include <iostream>

int main (int argc, const char **argv) {

  MainFrame frame{};

  frame.init();

  frame.execute();

  std::cout << "finish\n";

  return 0;
}
