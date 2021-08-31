#include <SPI.h>  
#include "RF24.h"


RF24 myRadio (7, 8);      // Radio what's new?
byte addrs[][6] = {"T"};  // Data pipe to send on

const byte slaveAddress[5] = {'R','x','A','A','A'};

const int num = 2;        //Array Stuff
int Array[num];


int potPin = A0;          // Throttle Input;



uint32_t start, dbStart;                  //Setup for latching button
const byte btn = 2, dbTime = 15;
bool pinState = true,
     btnState = true,
     prevBtnState = true,
     latch = false;

void setup() {

  pinMode(btn,INPUT); //Set D2 as input

  // Set up the radio
  myRadio.begin();                       // Start radio
  myRadio.setChannel(100);               // Could be changed 1 ... 125
  myRadio.setPALevel(RF24_PA_MAX);       // Mo Power Babeh!!!
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openWritingPipe( addrs[0]);    // Write to pipe called "T"
  delay(750);
}

void loop() {
//++--------------------Latching Button---------------------++
  // debounce button ++++++++++++++++
  if (digitalRead(btn) != pinState)  // get state of pin 2
  {
    dbStart = millis();     // reset db timer
    pinState = !pinState;   // now they are equal, won't enter
  }                         // here again unless pin state changes
  if (millis() - dbStart > dbTime) // db timer has elapsed
    btnState = pinState;           // button state is valid 
//+++++++++++++++++++++++++++++++++++  
  if(btnState != prevBtnState && btnState == HIGH) // btnState changed
  {
    // if button pressed and NOT latched
    if(!latch)
    {
      latch = true;    // prevents entry while latched
    }  
    else
    {
      // if button pressed while latched,
      latch = false;
    }
    prevBtnState = btnState;
  }
//++--------------------------------------------------------++
  
  //Get values and store Array
  int potValue = analogRead(potPin);   // Read Pot value
  Array[0] = potValue;
  Array[1] = latch;

  //Send Array
  myRadio.write(&Array, sizeof(Array));   
} 
