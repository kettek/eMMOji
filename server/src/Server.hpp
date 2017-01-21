#pragma once
#include "ServerConfig.hpp"
#include <websocketpp/server.hpp>
#include "IdPool.hpp"
#include <map>
#include "Client.hpp"

#include "Zone.hpp"

#include "rapidjson/document.h"

namespace eMMOji {
typedef websocketpp::server<ServerConfig> server;
typedef server::connection_ptr connection_ptr;

class Server {
  private:
    server mServer;
    // Ids/Clients
    IdPool mIdPool;
    std::map<int, Client> mIdToClients;
    std::map<Client, int> mClientToIds;
    // Zones
    IdPool                mZonePool;
    std::map<int, Zone>   mZones;
    int                   mZoneStartId = -1;
    // ZONE
    int createZone();
    // DATA
    std::map<std::string, std::map<std::string, rapidjson::Document>> mData;
    void loadData(std::string section, std::string entry);
  public:
    Server();
    void onOpen(websocketpp::connection_hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg);
	  void onHttp(websocketpp::connection_hdl);
    void gogogo(uint16_t port);
};
}
