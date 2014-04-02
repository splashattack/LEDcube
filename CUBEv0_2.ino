#include "cubemap.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <bitset>

const int dataPin = 10, latchPin = 11, clockPin = 12;
const int level[] = {7, 6, 5, 4}; //level select pins


//This is a 2 demensional array that maps out all of the output locations to the correct output
//address on the shift register based on the circuit wiring.
//
// Each entry corresponds to the binary string to light said specific LED column.
const int positions[4][4] = { 
  {32, 128, 8192, 32768},
  {4, 1, 1024, 256},
  {16, 64, 4096, 16384},
  {8, 2, 2048, 512} 
};




void setup()
{
  Serial.begin(9600);
  int last_k = 0;//variable used in ISR
  
  //INTERRUPT SETUP
  
  // Source:
  // Arduino timer CTC interrupt example
  // www.engblaze.com
  
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
 
  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
  
  
  
  //PIN MODE SETUP
  pinMode(dataPin,OUTPUT);//Blue wire (D10)
  pinMode(latchPin,OUTPUT);//Green wire (D11)
  pinMode(clockPin,OUTPUT);//Yellow wire (D12)
  
  pinMode(level[0],OUTPUT);//Orange wire (D7)
  pinMode(level[1],OUTPUT);//Brown wire (D6)
  pinMode(level[2],OUTPUT);//Purple wire wire (D5)
  pinMode(level[3],OUTPUT);//White wire (D4)
}



//This ISR pulls the data for the current state of the cube and manipuilates it, and displays the correct pattern
//on the cube itself. Basically, this is where all the dirty work happens.

ISR(TIMER1_COMPA_vect)
{
  int last_k=0;
  for(int k=0; k<4; k++)
    {
       //freeze cube
       digitalWrite(latchPin, LOW);
       
       //pull data from the matrix
       std::bitset<16> input = cubemap.frame[k];
       //translate data
       
       //push data to cube
       shiftOut(dataPin, clockPin, MSBFIRST, highByte(input));
       shiftOut(dataPin, clockPin, MSBFIRST, lowByte(input));
       
       //Turn off previous layer
       digitalWrite(level[last_k],LOW);
       
       //Display new pattern
       digitalWrite(latchPin, HIGH);
       //Turn on the current layer
       digitalWrite(level[k],HIGH);

    //Store this layer for use in next iteration of loop
    last_k = k;
    
    }
  
}
 


void loop()
{
    //
}
 

