#include "cubemap.h"

unsigned long atob(const char* pattern_string)
{
    const byte length = 17;
    unsigned long output_pattern = 0;
    for (byte i = 0; i < length; i++)
    {
        output_pattern |= atol(&pattern_string[i]) << length - 1 - i;
    }
    return output_pattern;
}


void cubemap::updateLayer(const unsigned long input_bits)
{
    byte select = (byte) (input_bits & 0xFFFF0000) >> 16;
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
