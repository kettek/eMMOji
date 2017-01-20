#pragma once
#include <queue>

namespace eMMOji {
class IdPool {
  public:
    int  getId();
    void revokeId(int);
  private:
    int              mCurrentId = 0;
    std::queue<int>  mFreeIds;
};
}
