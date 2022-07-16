/***************************************************
DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the basic function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <OneButton.h>
#include <colors.h>  //adding installed library
#include <Adafruit_NeoPixel.h>  //adding installed library

Adafruit_NeoPixel pixel(12, 17, NEO_GRB + NEO_KHZ800);  //declaring my object

DFRobotDFPlayerMini myDFPlayer;
const int PIXELPIN = 17; //declare pixel pin as 17
const int PIXELCOUNT = 12; //declare 16 pixels
int i;  //declaring an variable for my 'for' loop


void printDetail(uint8_t type, int value);
bool enableMusicPlayer, buttonState, onoff;

OneButton button1(23, false, INPUT);  //object defined as pin 23, circuit is pull down, and button is considered an input 

void setup() {
  button1.attachClick(click1); //initializing the object
  button1.attachDoubleClick(doubleClick1); //initializing the object
  button1.setClickTicks(500); //setting timer on clicks off
  button1.setPressTicks(2000); //setting timer on click ticks on
  buttonState = false;  //the button state is set to off initialilly
  pixel.begin();  //pixel begin
  pinMode(PIXELPIN, OUTPUT);  //use PIXELPIN as an output
  pixel.setBrightness(250); //set brightness to max
  pixel.show(); //initializing all as of
  Serial1.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("THOR Music engaged"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
//  if (!myDFPlayer.begin(Serial1)) {  //Use softwareSerial to communicate with mp3.
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
//    while(true){
//      delay(0); // Code to compatible with ESP8266 watch dog.
//    }
//
    for (int i = 0; i < 5; i++) {
    enableMusicPlayer = myDFPlayer.begin(Serial1);
    delay(200);
    if (enableMusicPlayer) {
      break;
      }
    }
  delay(2000);
  Serial.println(F("DFPlayer Mini online."));
//    if (buttonState == true){
//    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
//    myDFPlayer.play(1);  //Play the first mp3
//    Serial.printf("Zepplin go!");
//  }
}
void loop() {
  button1.tick();  //check the state of the button
  static unsigned long timer = millis();
    
    if (buttonState == true) {
    
    for (i = 0; i >13; i = i + 1) {
    pixel.fill(maize, i += 1, 12);   //setting pixel color as maize
    pixel.show();

        for (int i = 0; i < 5; i++) {
        enableMusicPlayer = myDFPlayer.begin(Serial1);
        delay(200);
          if (enableMusicPlayer) {
          break;
          }
        }
      }
    } 
//          if (buttonState == false) {  //if buttonState is 1 then
//          //onoff = !onoff; //toggle on and off
//          pixel.fill(black, i -= 1, 12); 
//          pixel.show();
     //}
//  if (millis() - timer > 3000000) {
//    timer = millis();
//    myDFPlayer.next();  //Play next mp3 every 3 second.
//  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}

void click1() {  //function that is going to tell Serial printf to log clicks on and off
  buttonState = !buttonState; //toggling buttonState
  Serial.printf("touch sensor single tap");
}

void doubleClick1() {
  buttonState = !buttonState;
  delay(50);
  Serial.printf("touchsensor double tap"); 
}
