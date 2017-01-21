#pragma once
#include <rapidjson/document.h>

namespace eMMOji {
enum ClientState {
  CONNECTING  = 0,
  LOGGED_IN   = 1,
  READY       = 2
};
class Client {
  public:
    std::string name;
    int loadFromJson(const char * json);
    int clientState = 0;
    int sessionId = -1;
    int entityId  = -1; // entity as stored in a Map
    int zoneId    = -1; // zone this client is residing in
    rapidjson::Document data;
};
}
