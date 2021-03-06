#include <OneButton.h>
#include <colors.h>  //adding installed library
#include <Adafruit_NeoPixel.h>  //adding installed library
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

const int PIXELPIN = 17; //declare pixel pin as 17
const int PIXELCOUNT = 76; //declare 16 pixels
int i;  //declaring an variable for my 'for' loop
int t; //declaring a variable for Rainbow

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
bool enableMusicPlayer, buttonState, onoff, blinker;

Adafruit_NeoPixel pixel(76, 17, NEO_GRB + NEO_KHZ800);  //declaring my object
OneButton button1(23, false, INPUT);  //object defined as pin 23, circuit is pull down, and button is considered an input 

void setup() {
  button1.attachClick(click1); //initializing the object
  button1.attachDoubleClick(doubleClick1); //initializing the object
  button1.setClickTicks(500); //setting timer on clicks off
  button1.setPressTicks(1500); //setting timer on click ticks on
  buttonState = false;  //the button state is set to off initialilly
  pixel.begin();  //pixel begin
  pinMode(PIXELPIN, OUTPUT);  //use PIXELPIN as an output
  pixel.setBrightness(250); //set brightness to max
  pixel.show(); //initializing all as of
  Serial1.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
    for (int i = 0; i < 5; i++) {
    enableMusicPlayer = myDFPlayer.begin(Serial1);
    delay(200);
    if (enableMusicPlayer) {
      break;
      }
    }
    
  delay(2000);
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
}

void loop() {
  button1.tick();
  static unsigned long timer = millis();
    if (millis() - timer > 30000000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  
  touchSensorBegin();
}

void touchSensorBegin (void) {  

  if (buttonState == true){
      for (i = 0; i < 12; i = i + 1) {  //telling LED use 0 -76 when incrementing lights
        pixel.clear();
        pixel.setPixelColor(i+1, maize);   //setting pixel color as rainbow[] % is equal to the remainder of the div
        pixel.setPixelColor(i+2, blue);
        pixel.setPixelColor(i+3, maize);
        pixel.show();
        delay(10);
      }
    
      for (i = 12; i > 0; i = i - 1) {
        pixel.clear();
        pixel.setPixelColor(i+1, blue);   //setting pixel color 
        pixel.setPixelColor(i+2, maize);
        pixel.show();
        delay(10);
      }
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
  Serial.printf("Button single click /n");
}

void doubleClick1() {
  blinker = !blinker;
  Serial.printf("Button double click /n"); 
}
