#include "Client.hpp"

namespace eMMOji {

int Client::loadFromJson(const char* json) {
  if (data.Parse(json).HasParseError()) {
    return 1;
  }

  if (!data.HasMember("name")) {
    data["name"].SetString("user", 4);
  }
  return 0;
}

}
