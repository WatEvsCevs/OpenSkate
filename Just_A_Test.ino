#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <nRF24L01.h>
#include <RF24.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

int gotArray[4];

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;

void setup() {
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); //Initialises the I2C addr 0x3D
  
  display.clearDisplay();

  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  //Display Logo
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,25);
  display.println("OpenSkate");
  display.display();
  delay(2000);
  

  

}

void loop() {
  delay(5);
  display.clearDisplay();
  //Read value and sent to board
  radio.stopListening();
  int potValue = analogRead(A0);
  int angleValue = map(potValue, 0, 1023, 0, 180);
  radio.write(&angleValue, sizeof(angleValue));  

  delay(5);

  radio.startListening();
  while (!radio.available());
  radio.read( &gotArray, sizeof(gotArray) );   
  
  DisplayData();
  

}




void DisplayData() {
  //Displays ES Battery
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.print("ES ");
  display.print(gotArray[1]);
 
  //Displays RM Battery (probs not gonna happen)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(80,0);
  display.print("RM ");
  
  //Displays ES speed
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30,25);
  display.print(gotArray[0]);
 
  //Displays ES ODE
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,50);
  display.print(gotArray[3]);

  //Displays ES amp draw 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90,50);
  display.print(gotArray[2]);
  display.print("Ah");    

  
  display.display();
}
