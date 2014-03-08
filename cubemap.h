#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <bitset>
//#include <Arduino.h>

class cubemap
{
private:
    //std::bitset<16> frame[LAYERS];
    std::bitset<16> foo;
    std::bitset<16> bar (0xfa2);
    std::bitset<16> baz (std::string("0101111001"));;

public:
    void updateLayer(const bitset& pattern) const;
};


#endif
