#include "cubemem.h"

void cubemem::update_layer(const unsigned long& input_bits)
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

// cubemem::cubemem(byte layers)
//     : _layers(layers),
// {
//     for 
//     _frame = {0};
// }

cubemem::cubemem()
{
    for (int i = _LAYERS; i > 0; i--)
    {
        update_layer(i << 16);
    }
}
