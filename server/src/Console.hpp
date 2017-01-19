#pragma once
#include <websocketpp/config/asio.hpp>

namespace eMMOji {
class Console {
  public:
    Console();
    ~Console();
  private:
    asio::thread t;
};

}
