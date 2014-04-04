#include "cubemap.h"

const int dataPin = 10, latchPin = 11, clockPin = 12;
const int layer_pins[] = {7, 6, 5, 4}; //level select pins


//This is a 2 demensional array that maps out all of the output locations to the correct output
//address on the shift register based on the circuit wiring.
//
// Each entry corresponds to the binary string to light said specific LED column.
//  [32, 128, 8192, 32768]
//  [4, 1, 1024, 256]
//  [16, 64, 4096, 16384]
//  [8, 2, 2048, 512]


//const int positions[16] = { 32, 128, 8192, 32768, 4, 1, 1024, 256, 16, 64, 4096, 16384, 8, 2, 2048, 512};
const int positions[16] = {512, 2048, 2, 8, 16384, 4096, 64, 16, 256, 1024, 1, 4, 32768, 8192, 128, 32};
//const int positions[4][4] =  {
//{32, 128, 8192, 32768},
//{4, 1, 1024, 256},
//{16, 64, 4096, 16384},
//{8, 2, 2048, 512}
//};

//boolean is_power_of_2(unsigned int n)
//{
//  /* http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2 */
//  return n && !(n & (n - 1));
//}

unsigned int mapLayer(unsigned int pattern)
{
  unsigned int mapped_pattern = 0;
  //byte _position = 0;
  for (int i = 0; i < 16; i++)
  {
    if ((pattern & (1 << i)) != 0)
    {
      mapped_pattern |= positions[i];
    }
  }
  //Serial.println(mapped_pattern, BIN);
  return mapped_pattern;
}

//unsigned int mapLayer(unsigned int layer)
//{
//  const byte COUNT = 15;
//  unsigned int mapped_layer = 0;
//  byte _position = 0;
//  for (int i = COUNT; i >= 0; i--)
//  {
//    _position = (layer & (1 << i)) >> i;
//    if (_position)
//    {
//      mapped_layer |= positions[COUNT - i];
//    }
//  }
//  return mapped_layer;
//}


/* This is supposed to make a box. */
const char* test_patterns[4] = {"00011111100110011111", "00101001000000001001", "00111001000000001001", "01001111100110011111"};

cubemap my_cube;

void setup()
{
  Serial.begin(9600);

  //INTERRUPT SETUP
  
  // Source:
  // Arduino timer CTC interrupt example
  // www.engblaze.com
  
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
 
  // set compare match register to 100 Hz @ 256 prescaler:
  //OCR1A = 6666;
  OCR1A = 200;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  //TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
  
  
  
  //PIN MODE SETUP
  pinMode(dataPin,OUTPUT);//Blue wire (D10)
  pinMode(latchPin,OUTPUT);//Green wire (D11)
  pinMode(clockPin,OUTPUT);//Yellow wire (D12)
  
  pinMode(layer_pins[0],OUTPUT);//Orange wire (D7)
  pinMode(layer_pins[1],OUTPUT);//Brown wire (D6)
  pinMode(layer_pins[2],OUTPUT);//Purple wire wire (D5)
  pinMode(layer_pins[3],OUTPUT);//White wire (D4)
  
  for (int i = 0; i < 4; i++)
  {
    my_cube.inputLayer(test_patterns[i]);
  }
}

byte k = 0;
byte last_k = 0;

//This ISR pulls the data for the current state of the cube and manipuilates it, and displays the correct pattern
//on the cube itself. Basically, this is where all the dirty work happens.

ISR(TIMER1_COMPA_vect)
{

     //freeze cube
   digitalWrite(latchPin, LOW);
   
   //pull data from the matrix
   unsigned int unmapped_input = my_cube.getLayer(k);
   //Serial.println(unmapped_input);
   unsigned int input;

   input = mapLayer(unmapped_input);
   //input = positions[15] | positions[0] | positions[3] | positions[12];

   
//   Serial.println(input, BIN);
//    Serial.println();
//   //translate data
   
   //input = 0xFFFF;
   
   //push data to cube
   shiftOut(dataPin, clockPin, MSBFIRST, highByte(input));
   shiftOut(dataPin, clockPin, MSBFIRST, lowByte(input));
   
   //Turn off previous layer
   digitalWrite(layer_pins[last_k], LOW);
   
   //Display new pattern
   digitalWrite(latchPin, HIGH);
   //Turn on the current layer
   digitalWrite(layer_pins[k],HIGH);

  //Store this layer for use in next iteration of loop
  last_k = k;
   k = (k + 1) % 4;
}



//


void loop()
{
}
 

