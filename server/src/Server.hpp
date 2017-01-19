#pragma once
#include "ServerConfig.hpp"
#include <websocketpp/server.hpp>

namespace eMMOji {
typedef websocketpp::server<ServerConfig> server;
typedef server::connection_ptr connection_ptr;

class Server {
  private:
    int mNextSessionId = 1;
    server mServer;
  public:
    Server();
    void onOpen(websocketpp::connection_hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void gogogo(uint16_t port);
};
}
