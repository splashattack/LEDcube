#include <Arduino.h>

unsigned long atob(const char* pattern_string);

class cubemap {
public:
  static const char LAYERS = 4;
  void updateLayer(const unsigned long pattern);
  void inputLayer(const char* pattern);
  unsigned int getLayer(const byte layer);
  
private:
  unsigned long _frame[LAYERS];

};


