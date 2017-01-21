/* JSON
Lazy, non-standards conformant JSON parser
*/

namespace eMMOji {
namespace JSON {
  enum Type {
    NIL       = 0
    , OBJECT  = 1
    , STRING  = 2
    , INT     = 3
    , FLOAT   = 4
  };

  class Object {
    public:
      uint8_t type = 0;
    private:
      void    *value = nullptr;
  };

  parse(const char*) {

  }
}

}
