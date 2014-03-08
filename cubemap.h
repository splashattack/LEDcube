//#include <Arduino.h>
#include <bitset>

const char LAYERS = 4;

class cubemap {
  std::bitset<16> frame[LAYERS];

public:
  updateLayer(bitset& pattern)
  {
    frame[pattern[0]] = pattern.to_ulong & 0xFFFF;
  }
};


