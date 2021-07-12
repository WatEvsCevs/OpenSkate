#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <VescUart.h>

//Set up array for VESC data
const int num = 4;
int Array[num];

//Radio Init
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo myServo;

//Start VESC UART Script
VescUart UART;

void setup() {
  
  //Start Servo Pin
  myServo.attach(5);
  
  //Start Radio Pipe
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);

  //Open UART for VESC Connection
  Serial.begin(115200);
  while (!Serial) {;}
  UART.setSerialPort(&Serial);

}

void loop() {delay(5);
  radio.startListening();
  //Read Pot value and drive board
  if ( radio.available()) {
    while (radio.available()) {
      int angleV = 0;
      radio.read(&angleV, sizeof(angleV));
      myServo.write(angleV);
      }
    delay(5);
    radio.stopListening();
    //Get VESC Values & send to remote
    if ( UART.getVescValues() ) {
      Array[0] = UART.data.rpm;
      Array[1] = UART.data.inpVoltage;
      Array[2] = UART.data.ampHours;
      Array[3] = UART.data.tachometerAbs;
      radio.write(&Array, sizeof(Array));
    }
  }
}
