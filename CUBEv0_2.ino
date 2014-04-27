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
const int positions[16] = {512, 2048, 2, 8, 16384, 4096, 64, 16, 256, 1024, 1, 4, 32768, 8192, 128, 32};

//This bitmasks input strings to the correct output format based on the positions array.
unsigned int mapLayer(unsigned int pattern)
{
  unsigned int mapped_pattern = 0;
  for (int i = 0; i < 16; i++)
  {
    if ((pattern & (1 << i)) != 0)
    {
      mapped_pattern |= positions[i];
    }
  }
  return mapped_pattern;
}




 

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
  
  
 //initialization pattern
 char* test_patterns[4] ={"0001 1111111111111111", "0010 1111111111111111", "0011 1111111111111111", "0100 1111111111111111"};
  for (int i = 0; i < 4; i++)
  {
    RemoveSpaces(test_patterns[i]);
    my_cube.inputLayer(test_patterns[i]);
    delay(100);
  }
 char* test_patterns2[4] ={"0001 0000000000000000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"};
   for (int i = 3; i >= 0; i--)
  {
    RemoveSpaces(test_patterns2[i]);
    my_cube.inputLayer(test_patterns2[i]);
    delay(100);
  }
}



//This ISR pulls the data for the current state of the cube and manipuilates it, and displays the correct pattern
//on the cube itself. Basically, this is where all the dirty work happens.
byte k = 0;
byte last_k = 0;

ISR(TIMER1_COMPA_vect)
{

     //freeze cube
   digitalWrite(latchPin, LOW);
   
   //pull data from the matrix
   unsigned int unmapped_input = my_cube.getLayer(k);
   
   //translate data
   unsigned int input;
   input = mapLayer(unmapped_input);
   
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




//This funtion removes any whitespace from the input string.
void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}



void loop()
{
  boxshrink();
  boxshrink2();
  boxshrink3();
  boxshrink4();
}



//PATTERNS!!!!!!!!

//this pattern makes a box that collapses into one of the corners
char* boxshrink_[8][4] ={{"0001 1111100110011111", "0010 1001000000001001", "0011 1001000000001001", "0100 1111100110011111"},//1  //1
                         {"0001 0111010101110000", "0010 0101000001010000", "0011 0111010101110000", "0100 0000000000000000"},//2  //2
                         {"0001 0011001100000000", "0010 0011001100000000", "0011 0000000000000000", "0100 0000000000000000"},//3  //3
                         {"0001 0001000000000000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //4
                         {"0001 0000000000000000", "0010 0000000000000000", "0001 0000000000000000", "0100 0000000000000000"},//5  //5
                         {"0001 0001000000000000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //6
                         {"0001 0011001100000000", "0010 0011001100000000", "0011 0000000000000000", "0100 0000000000000000"},//3  //7
                         {"0001 0111010101110000", "0010 0101000001010000", "0011 0111010101110000", "0100 0000000000000000"},//2  //8
                       };
void boxshrink() {
  int pause = 100;
    for(int i = 0; i < 8; i++){
//      Serial.print("i= ");
//      Serial.println(i);
      
    for (int j = 0; j < 4; j++)
    {
//      Serial.print("j= ");
//      Serial.println(j);
      RemoveSpaces(boxshrink_[i][j]);
      my_cube.inputLayer(boxshrink_[i][j]);
    }
    if( i == 0 || i == 4)
    {
      delay(pause*2);
    }
    else{
      delay(pause);
    }
  }
}

char* boxshrink2_[8][4] ={{"0001 1111100110011111", "0010 1001000000001001", "0011 1001000000001001", "0100 1111100110011111"},//1  //1
                         {"0001 0000011101010111", "0010 0000010100000101", "0011 0000011101010111", "0100 0000000000000000"},//2  //2
                         {"0001 0000000000110011", "0010 0000000000110011", "0011 0000000000000000", "0100 0000000000000000"},//3  //3
                         {"0001 0000000000000001", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //4
                         {"0001 0000000000000000", "0010 0000000000000000", "0001 0000000000000000", "0100 0000000000000000"},//5  //5
                         {"0001 0000000000000001", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //6
                         {"0001 0000000000110011", "0010 0000000000110011", "0011 0000000000000000", "0100 0000000000000000"},//3  //7
                         {"0001 0000011101010111", "0010 0000010100000101", "0011 0000011101010111", "0100 0000000000000000"},//2  //8
                       };
void boxshrink2() {
  int pause = 100;
    for(int i = 0; i < 8; i++){
//      Serial.print("i= ");
//      Serial.println(i);
      
    for (int j = 0; j < 4; j++)
    {
//      Serial.print("j= ");
//      Serial.println(j);
      RemoveSpaces(boxshrink2_[i][j]);
      my_cube.inputLayer(boxshrink2_[i][j]);
    }
    if( i == 0 || i == 4)
    {
      delay(pause*2);
    }
    else{
      delay(pause);
    }
  }
}
char* boxshrink3_[8][4]={{"0001 1111100110011111", "0010 1001000000001001", "0011 1001000000001001", "0100 1111100110011111"},//1  //1
                         {"0001 0000111010101110", "0010 0000101000001010", "0011 0000111010101110", "0100 0000000000000000"},//2  //2
                         {"0001 0000000011001100", "0010 0000000011001100", "0011 0000000000000000", "0100 0000000000000000"},//3  //3
                         {"0001 0000000000001000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //4
                         {"0001 0000000000000000", "0010 0000000000000000", "0001 0000000000000000", "0100 0000000000000000"},//5  //5
                         {"0001 0000000000001000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //6
                         {"0001 0000000011001100", "0010 0000000011001100", "0011 0000000000000000", "0100 0000000000000000"},//3  //7
                         {"0001 0000111010101110", "0010 0000101000001010", "0011 0000111010101110", "0100 0000000000000000"},//2  //8
                       };
void boxshrink3() {
  int pause = 100;
    for(int i = 0; i < 8; i++){
//      Serial.print("i= ");
//      Serial.println(i);
      
    for (int j = 0; j < 4; j++)
    {
//      Serial.print("j= ");
//      Serial.println(j);
      RemoveSpaces(boxshrink3_[i][j]);
      my_cube.inputLayer(boxshrink3_[i][j]);
    }
    if( i == 0 || i == 4)
    {
      delay(pause*2);
    }
    else{
      delay(pause);
    }
  }
}
char* boxshrink4_[8][4]={{"0001 1111100110011111", "0010 1001000000001001", "0011 1001000000001001", "0100 1111100110011111"},//1  //1
                         {"0001 1110101011100000", "0010 1010000010100000", "0011 1110101011100000", "0100 0000000000000000"},//2  //2
                         {"0001 1100110000000000", "0010 1100110000000000", "0011 0000000000000000", "0100 0000000000000000"},//3  //3
                         {"0001 1000000000000000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //4
                         {"0001 0000000000000000", "0010 0000000000000000", "0001 0000000000000000", "0100 0000000000000000"},//5  //5
                         {"0001 1000000000000000", "0010 0000000000000000", "0011 0000000000000000", "0100 0000000000000000"},//4  //6
                         {"0001 1100110000000000", "0010 1100110000000000", "0011 0000000000000000", "0100 0000000000000000"},//3  //7
                         {"0001 1110101011100000", "0010 1010000010100000", "0011 1110101011100000", "0100 0000000000000000"},//2  //8
                       };
void boxshrink4() {
  int pause = 100;
    for(int i = 0; i < 8; i++){
//      Serial.print("i= ");
//      Serial.println(i);
      
    for (int j = 0; j < 4; j++)
    {
//      Serial.print("j= ");
//      Serial.println(j);
      RemoveSpaces(boxshrink4_[i][j]);
      my_cube.inputLayer(boxshrink4_[i][j]);
    }
    if( i == 0 || i == 4)
    {
      delay(pause*2);
    }
    else{
      delay(pause);
    }
  }
}
