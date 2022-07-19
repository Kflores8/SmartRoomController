
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <wemo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <OneButton.h>
#include <colors.h>  //adding installed library
#include <Adafruit_NeoPixel.h>  //adding installed library
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int SCREEN_ADDRESS = 0x3C;
const int LOGO_HEIGHT = 64;
const int LOGO_WIDTH = 128;
const int PIXELPIN = 17; //declare pixel pin as 17
const int PIXELCOUNT = 12; //declare 12 pixels
const int BUTTONPIN = 4; 
const int ETHERIN = 12;
const int ETHEROUT = 11;
const int RESET = 9; 
const int CHIPSET = 10;
int i;  //declaring an variable for my 'for' loop
int timer2; //declaring a variable for Rainbow
float tempC, pressPA, humidRH, tempF, inHG, currentTempF, lastTemp, lastHG;

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
bool status, enableMusicPlayer, buttonState, onoff, blinker;
byte thisbyte;

Adafruit_NeoPixel pixel(12, 17, NEO_GRB + NEO_KHZ800);  //declaring my object
OneButton button1(23, false, INPUT);  //object defined as pin 23, circuit is pull down, and button is considered an input 
OneButton button2(4, false, INPUT);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 
Adafruit_BME280 bme;

void setup() {
  button1.attachClick(click1); //initializing the object
  button1.attachDoubleClick(doubleClick1); //initializing the object
  button1.setClickTicks(500); //setting timer on clicks off
  button1.setPressTicks(1500); //setting timer on click ticks on
  buttonState = false;  //the button state is set to off initialilly
  pixel.begin();  //pixel begin
  pinMode(PIXELPIN, OUTPUT);  //use PIXELPIN as an output
  pixel.setBrightness(200); //set brightness to max
  pixel.show(); //initializing all as of
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());

  display.begin(SCREEN_ADDRESS);
  while(!Serial);
  status = bme.begin(0x76);

  
        if (status == false) {
          Serial.printf("BME280 at address 0x%02X failed to start \n", 0x76);
        }
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.printf("SSD1306 allocation failed");
        for(;;); // Don't proceed, loop forever
        }
        
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
  
  myDFPlayer.volume(5);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
}

void loop() {
  touchSensorBegin();
  static unsigned long timer;
    if (millis() - timer > 10000) {
    timer = millis();
  }
  
//  if (myDFPlayer.available()) {
//    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
//  }
    if (millis() - timer2 > 20000) {
    checkBMEHue ();
    testdrawstyles();
    timer2 = millis();
  }
  checkWemo();
}

void touchSensorBegin (void){  
  button1.tick();
  if (buttonState == true){
      for (i = 0; i < 12; i = i + 1) {  //telling LED use 0 -76 when incrementing lights
        //pixel.clear();
        pixel.setPixelColor(i+1, maize);   //setting pixel color as rainbow[] % is equal to the remainder of the div
        pixel.setPixelColor(i+2, blue);
        pixel.setPixelColor(i+3, maize);
        pixel.show();
        delay(10);
        Serial.printf("Button single click /n");
      }
    
      for (i = 12; i > 0; i = i - 1) {
        pixel.clear();
        pixel.setPixelColor(i+1, blue);   //setting pixel color 
        pixel.setPixelColor(i+2, maize);
        pixel.show();
        delay(10);
        Serial.printf("Button single click false /n");
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
  //Serial.printf("Button single click /n");
}

void doubleClick1() {
  blinker = !blinker;
  //Serial.printf("Button double click /n"); 
}

void checkBMEHue (void) {
  
  setHue(3,true,HueBlue,255,255);
  Serial.printf("Get Hue Data: ");
  getHue(3);
  setHue(3,false,0,0,0);
  //delay(100);

  tempC = bme.readTemperature();
  pressPA = bme.readPressure();
  humidRH = bme.readHumidity();
  tempF = tempC*9/5+32;
  inHG = pressPA/3386; 
  Serial.printf("%f, %f, %f \n", tempF, inHG, humidRH);
  //delay(100);


        if (tempF > 80.00) {
          setHue(1,true,HueBlue,255,255);
          setHue(2,true,HueBlue,255,255);
          setHue(3,true,HueBlue,255,255);
          Serial.printf("Get Hue Data: ");
          display.display();
          Serial.printf("%f, %f \n", tempF, lastTemp);
          lastTemp = tempF;
        }

            else {
              setHue(1, false, 0, 0, 0);
              setHue(2, false, 0, 0, 0);
              setHue(3, false, 0, 0, 0);
        }
        
          if (inHG != lastHG) {
          setHue(6,true,HueGreen,255,255);
          setHue(4,true,HueGreen,255,255);
          setHue(5,true,HueGreen,255,255);
          Serial.printf("Get Hue Data: ");
          Serial.printf("%f, %f \n", inHG, lastHG);
          lastHG = inHG;
        }
            else {
              setHue(6, false, 0, 0, 0);
              setHue(4, false, 0, 0, 0);
              setHue(5, false, 0, 0, 0);
        }
}

void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(1);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 0);
  display.printf("Temperature:", tempF);
  display.display();
  //delay(1000);

  display.setTextSize(1);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 10);
  display.printf("%f", tempF);
  display.display();
  //delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 20);
  display.printf("Humidity:");
  display.display();
  //delay(1000);

  display.setTextSize(1);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 30);
  display.printf("%f", humidRH);
  display.display();
  //delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 40);
  display.printf("Barometric Pressure:");
  display.display();
  //delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 50);
  display.printf("%f", humidRH);
  display.display();
  //delay(1000);
  }

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);
}

void checkWemo (void){
   
int i;
int t;
int inputValue;
int myButton = 4; 
int myWemo[] = {0,1,2,3,4};

bool buttonState, doubleclick, longPressStart;

inputValue = digitalRead(myButton);
Serial.printf("%i \n", inputValue);

    if (buttonState == true) {
      switchON(myWemo[2]);
      Serial.printf("Hello Wemo %i /n", myWemo);
    }      
          else {
            switchOFF(myWemo[2]);
            Serial.printf("Bye Wemo %i /n", myWemo);
          }
}
