// My LED cube
// Caleb Gill
// Created 3.7.14
// v0.1

#include <cstdlib>
#include "cubemem.h"

const int dataPin = 10, latchPin = 11, clockPin = 12;
const int level[] = {7, 6, 5, 4};
int pause = 1000;

// EEPROM data structure with location start and number of pattern strings

// This is a 2 demensional array that maps out all of the output locations to the correct output
// address on the shift register based on the circuit wiring.
//
// Each entry corresponds to the binary string to light said specific LED column.
const int position_map[4][4] = { 
    {32, 128, 8192, 32768},
    {4, 1, 1024, 256},
    {16, 64, 4096, 16384},
    {8, 2, 2048, 512}
};

const char* test_pattern = "0010010100111011";

int atob (const char* pattern_string, const byte length)
{
    unsigned long output_pattern = 0;
    for (byte i = 0; i < length; i++)
    {
        output_pattern |= atoi(&pattern_string[i]) << length - 1;
    }
}

void setup()
{
    pinMode(dataPin,OUTPUT);//Blue wire (D10)
    pinMode(latchPin,OUTPUT);//Green wire (D11)
    pinMode(clockPin,OUTPUT);//Yellow wire (D12)

    pinMode(level[0],OUTPUT);//Orange wire (D7)
    pinMode(level[1],OUTPUT);//Brown wire (D6)
    pinMode(level[2],OUTPUT);//Purple wire wire (D5)
    pinMode(level[3],OUTPUT);//White wire (D4)


    Serial.begin(9600);
}

void loop()
{
    /* The `positions` array is addressed row, column which is [i][j]. */
    //test();
    //count();
    int led1 = position_map[1][2];
    int led2 = position_map[3][3];
    int led_out = led1 | led2;
    //digitalWrite(level[1], HIGH);


    //frame(255,255);
  
  
}

void update_layer(const unsigned int& pattern)
{
    digitalWrite(latchPin, LOW); //freeze display
    shiftOut(dataPin, clockPin, MSBFIRST, highByte(pattern));
    shiftOut(dataPin, clockPin, MSBFIRST, lowByte(pattern));
    digitalWrite(latchPin, HIGH); //display new number
  
}

void getpattern() {
    if (Serial.available())
    {
        char readBuffer[128];
        char dataBuffer[17];
        Serial.readBytes(readBuffer,128);
        if (true)//if header and footer present
        {
            //cut middle 17
            //first bit is choose layer
            //16 go to layer's memory
        }
        else
        {
            Serial.println("Error: Message cannot be read.");
        }
    }
}



