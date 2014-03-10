#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <Arduino.h>

class cubemap
{

public:
    static const byte LAYERS = 4;
    void updateLayer(const unsigned long& pattern);

private:
    unsigned long frame[LAYERS];

};


#endif
