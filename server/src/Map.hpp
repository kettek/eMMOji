/* Map
````````````````````````````````````````````````````````````````````````````````
Map is a live instance created from a JSON Document. It contains Entities.
*/
#pragma once
#include <rapidjson/document.h>
#include <list>
#include "Entity.hpp"
#include "IdPool.hpp"

namespace eMMOji {
class Map {
  public: 
    void loadData(rapidjson::Document *json);
    int addEntity(rapidjson::Value *json);
    std::list<Entity>                 activeEntities;
  private:
    IdPool entityIds;
    int width;
    int height;
};
}
