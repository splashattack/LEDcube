#include "cubemap.h"
#include <cstring>

unsigned long atob(const char* pattern_string)
{
    const byte length = strlen(pattern_string);
    unsigned long output_pattern = 0;
    for (byte i = 0; i < length; i++)
    {
      char isolated[] = {pattern_string[i], '\0'};
      output_pattern |= atol(isolated) << length - 1 - i;
    }
    Serial.println(output_pattern, BIN);
    return output_pattern;
}


void cubemap::updateLayer(const unsigned long input_bits)
{
    unsigned long select = (input_bits & 0xFFFF0000) >> 16;
    unsigned int pattern = input_bits & 0xFFFF;
    if (select == 0)
    {
        delay(pattern);
    }
    else
    {
        _frame[select - 1] = pattern;
    }
}

void cubemap::inputLayer(const char* pattern)
{
  updateLayer(atob(pattern));
}

unsigned int cubemap::getLayer(const byte layer)
{
  return _frame[layer];
}
