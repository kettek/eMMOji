#pragma once
#include <string>
#include <rapidjson/document.h>

namespace eMMOji {
class Entity {
  public:
    Entity(rapidjson::Value *data);
    Entity(rapidjson::Value *data, int id);
    rapidjson::Value data;
    std::string image;
    int mId;
    int x;
    int y;
};
}
