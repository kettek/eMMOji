#pragma once
#include "ServerConfig.hpp"
#include <websocketpp/server.hpp>
#include "IdPool.hpp"

namespace eMMOji {
typedef websocketpp::server<ServerConfig> server;
typedef server::connection_ptr connection_ptr;

class Server {
  private:
    IdPool mIdPool;
    server mServer;
  public:
    Server();
    void onOpen(websocketpp::connection_hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg);
	  void onHttp(websocketpp::connection_hdl);
    void gogogo(uint16_t port);
};
}
