/*
 * Project: Smart Room Controller
 * Description: Portion of THOR SRC for Touch Pad Function.
 * Author: Kevin flores
 * Date: 19-July-2022
 */
 
#include <OneButton.h>

OneButton button1(23, false, INPUT);  //object defined as pin 23, circuit is pull down, and button is considered an input 

bool buttonState, blinker;  //declaring variables as the state of the button and its blinker


void setup() {
  button1.attachClick(click1); //initializing the object
  button1.attachDoubleClick(doubleClick1); //initializing the object
  button1.setClickTicks(500); //setting timer on clicks off
  button1.setPressTicks(2000); //setting timer on click ticks on
  buttonState = false;  //the button state is set to off initialilly
  Serial.begin (9600);  //identifiying Serial Printf
  delay(1000); //Adding a 1 second delay
  Serial.printf(""); //opening Serial Printf to numerous variables
}

void loop() {
  button1.tick();  //check the state of the button

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
