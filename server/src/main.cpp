#include <iostream>

#include "Server.hpp"

#define API_VERSION "0.0.1"
#define APP_VERSION "0.0.1"

int main(int argc, char *argv[]) {
  std::cout << "Starting eMMOji " << APP_VERSION << "/" << API_VERSION << std::endl;

  eMMOji::Server mServer;
  mServer.gogogo(9002);

  return 0;
}
