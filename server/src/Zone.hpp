/* Zone
````````````````````````````````````````````````````````````````````````````````
A Zone is an instance of Clients that are tied to a Map.

Maps are loaded into Zone(s).
*/
#pragma once
#include <map>
#include <rapidjson/document.h>

#include "Client.hpp"
#include "Map.hpp"

namespace eMMOji {
class Zone {
  public:
    void addCommands(int clientId, rapidjson::Value data);
    std::map<int, Client*>  connectedClients;
    Map                     activeMap;
};
}
