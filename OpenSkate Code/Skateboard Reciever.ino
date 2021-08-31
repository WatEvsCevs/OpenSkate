#include "VescUart.h"
#include <SPI.h>
#include "RF24.h"

RF24 myRadio (7, 8);      //Radio, someone still loves you
byte addrs[][6] = {"T"}; // Data pipe

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

int x = 0;               // Throttle Value
int gotArray[2];         // Array Stuff
bool cruise = false;     // Cruise State


VescUart VESCUART;       //Enable the UART thingy

uint32_t LastTime = 0;   // Timer for Throttle and Cruise (don't know why it needs it but this works)

void setup() {

  Serial.begin(115200);  //Begin UART communication to VESC

  VESCUART.setSerialPort(&Serial);

  // Set up the radio
  myRadio.begin();                       // Start radio
  myRadio.setChannel(100); 
  myRadio.setPALevel(RF24_PA_MAX);       // Mo Power Babeh!!!
  myRadio.setDataRate( RF24_250KBPS ); 
  myRadio.openReadingPipe(1, addrs[0]);  // Read from pipe called "T"
  myRadio.startListening();              // Listen for signal from Boss
}


void loop() {
if (myRadio.available()) {        // Picked up transmission from Boss
      myRadio.read( &gotArray, sizeof(gotArray) );   // Read the array
      x = gotArray[0];            // Set the Throttle Position
      cruise = gotArray[1];      // Set the Cruise state
      delay(10);                  // Delay to see if the remote connection is lost
  }
  else 
  {
    x = 350;            // Reset to defult values when conn. is lost
    cruise = false;
  }

if((millis() - LastTime) > 10){       //Apply Values Class


    VESCUART.nunchuck.valueY = map(x, 0, 1023, 0, 255);   //Set throttle value stored in 'x'

    if (cruise == true)  {                  // If 'cruise' = 'true' enable cruise state
      VESCUART.nunchuck.lowerButton = true;      
    }
    else {
      VESCUART.nunchuck.lowerButton = false; // If 'cruise' = 'false' disable cruise state
    }

    VESCUART.setNunchuckValues();   //Send values to VESC

    LastTime = millis();

  }  
}

 



