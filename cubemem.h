#ifndef CUBEMEM_H
#define CUBEMEM_H

#include <Arduino.h>

class cubemem
{

public:
    void update_layer(const unsigned long& pattern);

    // cubemem(byte layers);

private:
    // Number of layers in the cube.
    static const byte _LAYERS = 4;

    // The cube's memory, holds the patterns on each layer.
    unsigned long _frame[_LAYERS];

    cubemem();

};


#endif
