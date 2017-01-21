#include "Map.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace eMMOji {
void Map::loadData(rapidjson::Document *json) {
  if (json == nullptr) {
    std::cout << "ERR: " << "json is null" << std::endl;
    return;
  }
  // verify map
  std::vector<std::string> verify = {"n", "w", "h", "m"};
  bool verify_results = true;
  for (auto i : verify) {
    if (!json->HasMember(i.c_str())) {
      std::cout << "ERR: " << "map has no " << i << std::endl;
      verify_results = false;
    }
  }
  if (!verify_results) {
    std::cout << "M: " << "failure!" << std::endl;
    return;
  }
  // load in entities
  const rapidjson::Value& entities = (*json)["m"];
  if (!entities.IsArray()) {
    std::cout << "ERR: " << "m is not an array" << std::endl;
    return;
  }
  for (rapidjson::Value::ConstValueIterator itr = entities.Begin(); itr != entities.End(); ++itr) {
    const rapidjson::Value& entity = *itr;
    std::cout << "M: " << entity["d"].GetString() << " @ " << entity["x"].GetInt() << "x" << entity["y"].GetInt() << std::endl;
  }
}

int Map::addEntity(rapidjson::Value *data) {
  // get new entity id
  int id = entityIds.getId();
  // add it to our current entities
  activeEntities.emplace_front(Entity{data, id});

  return id;
//  return &activeEntities.front();
}
}
