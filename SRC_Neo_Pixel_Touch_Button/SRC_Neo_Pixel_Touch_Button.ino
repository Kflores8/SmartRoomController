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
//void printDetail(uint8_t type, int value);
bool enableMusicPlayer, buttonState, onoff, blinker;

Adafruit_NeoPixel pixel(12, 17, NEO_GRB + NEO_KHZ800);  //declaring my object
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
  Serial.begin(9600);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
    
  delay(2000);
  Serial.println(F("DFPlayer Mini online."));
           
           for (int i = 0; i < 5; i++) {
            enableMusicPlayer = myDFPlayer.begin(Serial1);
            delay(200);
            if (enableMusicPlayer) {
            break;
            }
            myDFPlayer.volume(20);  //Set volume value. From 0 to 30
            myDFPlayer.play(1);  //Play the first mp3
           }
}

void loop() {
  button1.tick();  //check the state of the button
  static unsigned long timer = millis();
  
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


void click1() {  //function that is going to tell Serial printf to log clicks on and off
  buttonState = !buttonState; //toggling buttonState
  Serial.printf("Button single click /n");
}

void doubleClick1() {
  blinker = !blinker;
  Serial.printf("Button double click /n"); 
}
