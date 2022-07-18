
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <colors.h>  //adding installed library
#include <Adafruit_NeoPixel.h>  //adding installed library

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int SCREEN_ADDRESS = 0x3C;
const int PIXELPIN = 17; //declare pixel pin as 17
const int PIXELCOUNT = 16; //declare 16 pixels
const int LOGO_HEIGHT = 64;
const int LOGO_WIDTH = 128;

bool status; 
int i;  //declaring an variable for my 'for' loop
int t; //declaring a variable for Rainbow
int pixelPosition; 
int pixelPosition2;
float tempC, pressPA, humidRH, tempF, inHG, currentTempF, lastTemp, lastHG;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 
Adafruit_BME280 bme;
Adafruit_NeoPixel pixel(16, 17, NEO_GRB + NEO_KHZ800);  //declaring my object


void setup() {
  Serial.begin(9600);  // ensure all SPI devices start off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());

  display.begin(SCREEN_ADDRESS);
  pixel.begin();  //pixel begin
  while(!Serial);
  status = bme.begin(0x76);
  
        if (status == false) {
          Serial.printf("BME280 at address 0x%02X failed to start \n", 0x76);
        }
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.printf("SSD1306 allocation failed");
        for(;;); // Don't proceed, loop forever
        }
}

void loop() {
  /* setHue function needs 5 parameters
   *  int bulb - this is the bulb number
   *  bool activated - true for bulb on, false for off
   *  int color - Hue color from hue.h
   *  int - brightness - from 0 to 255
   *  int - saturation - from 0 to 255
   */
  setHue(3,true,HueBlue,255,255);
  Serial.printf("Get Hue Data: ");
  getHue(3);
  setHue(3,false,0,0,0);
  delay(5000);

  tempC = bme.readTemperature();
  pressPA = bme.readPressure();
  humidRH = bme.readHumidity();
  tempF = tempC*9/5+32;
  inHG = pressPA/3386; 
  Serial.printf("%f, %f, %f \n", tempF, inHG, humidRH);
  delay(1000);
  pixelPosition = map(tempF,0,100,0,16);
  pixelPosition2 = map(inHG,0,35,0,16);

        if (tempF != lastTemp) {
          setHue(3,true,HueBlue,255,255);
          Serial.printf("Get Hue Data: ");
          getHue(3);
          testdrawstyles();
          display.display();
          Serial.printf("%f, %f, %i \n", tempF, lastTemp, pixelPosition);
          lastTemp = tempF;
          setHue(3,false,0,0,0);
          delay(5000);
        }
          if (inHG != lastHG) {
          setHue(3,true,HueGreen,255,255);
          Serial.printf("Get Hue Data: ");
          getHue(3);
          Serial.printf("%f, %f, %i \n", inHG, lastHG, pixelPosition2+1);
          setHue(3,false,0,0,0);
          delay(5000);
        }
}

void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  //display.setRotation(2);
  display.printf("Good Day THORster!");
  display.display();
  delay(100);

  display.setTextSize(.7);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 10);
  display.printf("The temperature is");
  display.display();
  delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 20);
  display.printf("%f", tempF);
  display.display();
  delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 30);
  display.printf("The humidity is");
  display.display();
  delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 40);
  display.printf("%f", humidRH);
  display.display();
  delay(1000);

      display.setTextSize(.7);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 50);
  display.printf("The barometric pressure is");
  display.display();
  delay(1000);

  display.setTextSize(.5);  //Draw 5x-scale text
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(10, 60);
  display.printf("%f", humidRH);
  display.display();
  delay(1000);
  }

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);
}
