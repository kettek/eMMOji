#include "IdPool.hpp"

namespace eMMOji {
int IdPool::getId() {
  int id = 0;

  if (!mFreeIds.empty()) {
    id = mFreeIds.front();
    mFreeIds.pop();
  } else {
    id = mCurrentId++;
  }
  return id;
}
void IdPool::revokeId(int id) {
  mFreeIds.push(id);
}
}
