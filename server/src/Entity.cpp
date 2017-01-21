#include "Entity.hpp"
#include <iostream>

namespace eMMOji {
Entity::Entity(rapidjson::Value *data) {
  // copy data to our own
  rapidjson::Document d;
  rapidjson::Document::AllocatorType& a = d.GetAllocator();
  this->data = rapidjson::Value(*data, a);
}
Entity::Entity(rapidjson::Value *data, int id) {
  // copy data to our own
  rapidjson::Document d;
  rapidjson::Document::AllocatorType& a = d.GetAllocator();
  this->data = rapidjson::Value(*data, a);
  // set our id
  mId = id;
}
}
