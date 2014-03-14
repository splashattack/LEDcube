#ifndef CUBEMEM_H
#define CUBEMEM_H

#include <Arduino.h>

class cubemem
{

public:
    void update_layer(const unsigned long& pattern);

    cubemem(byte layers);

private:
    byte _layers;
    unsigned long _frame[];

};


#endif
