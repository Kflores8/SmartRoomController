/*
 *  Project:      HueHeader
 *  Description:  Create code that has a button that turns on and off the Hue Light at your pod, uses the encoder to change the brighness of the Hue bulb.  Create a method of cycling the Hue light throught the colors of the rainbow.
 *  Authors:      Kevin Flores
 *  Date:         12-JUL-2022
 */
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <Encoder.h>
#include <OneButton.h>

OneButton button1(4, true, true);
Encoder myEnc(2, 3);

 const int GREENLED = 5;
 const int REDLED = 6;
 const int rainbow[] = {HueRed, HueOrange, HueYellow, HueGreen, HueBlue, HueIndigo, HueViolet};
 
 int i;
 int t; 
 int position = 0;
 int lastPosition = 96;
 int inputValue;
 int currentTime;
 int lastSecond = 1000;

 bool buttonState, onoff;

void setup() {
  button1.attachClick(click1);
  button1.setClickTicks(50);
  buttonState = false; 
  Serial.begin(9600);
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  
  // ensure all SPI devices start off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);

  Ethernet.begin(mac);
  delay(5000);          //ensure Serial Monitor is up and running           
  //printIP();
  Serial.printf("LinkStatus: %i  \n, ",Ethernet.linkStatus());
}

void loop() {
  button1.tick();
  position = myEnc.read();
  inputValue = digitalRead(buttonState);
  currentTime = millis();
  hueBri = map(position, 0, 96, 0, 255);
  
    if (buttonState == true) {
        //setHue(3,true,HueGreen,255,255);
        digitalWrite (GREENLED, HIGH);
        digitalWrite (REDLED, LOW);
        //Serial.printf("%i \n", hueBri);
        
        if (position != lastPosition) {  //if position is not last position
          //hueBri = map(position,0,96,0,255);
          //Serial.printf("%i \n", position);  //print 
          lastPosition = myEnc.read();
        }
     }
          else {
            //setHue(3,true,HueRed,255,255);
            digitalWrite(GREENLED, LOW);
            digitalWrite (REDLED, HIGH);
            //Serial.printf("%i \n", buttonState);
          } 
          
     if((currentTime-lastSecond)>5000) {
     setHue(3,buttonState,rainbow[t = i%7],255,500);
     lastSecond = millis();
     Serial.printf("%i \n", hueBri);
     lastPosition = myEnc.read();
     }

     if (position != lastPosition) {  //if position is greater than or equal to position
    //hueBri = map(position, 0, 96, 0, 255);
    hueHue = map(position, 0, 96, 0, 7);
    //hueSat =
    setHue(3,buttonState,i++,255,500);
    Serial.printf("%i \n", position);  //print 
    lastPosition = myEnc.read(); 
     }
}

void click1() {  //function that is going to tell Serial printf to log clicks on and off
  buttonState = !buttonState; //toggling buttonState
}
