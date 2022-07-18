
 #include <colors.h>
 #include <OneButton.h>
 #include <wemo.h>
 #include <mac.h>
 #include <hue.h>
 #include <SPI.h>
 #include <Ethernet.h>
 
 
 OneButton button1(4, false, INPUT_PULLDOWN);  //object defined as pin 4, circuit is pull down, and button is considered an input
 
 const int BUTTONPIN = 4; 
 const int ETHERIN = 12;
 const int ETHEROUT = 11;
 const int RESET = 9; 
 const int CHIPSET = 10;
 
 int i;
 int t;;
 int inputValue;
 int wemoPosition;
 int myWemo[] = {0,1,2,3,4};
 int myHue[] = {0, 1, 2, 3, 4, 5};

 bool buttonState, doubleclick, longPressStart;
 
EthernetClient client;
bool status;   //user to ensure port openned correctly
byte thisbyte; //used to get IP address

void setup() {
  //initialize ethernet port and uSD module to off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  button1.attachClick(click1); //initializing the object
  button1.attachDoubleClick(doubleClick1); //initializing the object
  button1.attachLongPressStart(longPressStart1);//initializing the object
  button1.setClickTicks(750);
  button1.setPressTicks(1000);
  buttonState = false; 

  //Open Serial Communication and wait for port to open:
  Serial.begin(9600);
  
  //Start ethernet connection
  status = Ethernet.begin(mac);
  if (!status) {
    Serial.printf("failed to configure Ethernet using DHCP \n");
    //no point in continueing 
    while(1);
  }
}

void loop() {
  button1.tick();  //check the state of the button
    inputValue = digitalRead(buttonState);
    wemoPosition = map(position,0,3,0,4);
    
}

  void click1() {  //function that is going to tell Serial printf to log clicks on and off
  buttonState = !buttonState; //toggling buttonState

      if (buttonState == true) {
      switchON(i);
      //Serial.printf("Hello Wemo %i /n", wemo);
    }      
          else {
        switchOFF(i);
        //Serial.printf("Bye Wemo %i /n", wemo);
      }
}

void doubleClick1() {  //function that is going to tell Serial printf to log clicks on and off
  doubleclick = !doubleclick; //toggling buttonState
  //i++;
  //Serial.printf("%i /n", i);
  // if (i>4) {    
      //i=0;
    //}
  //switchON(myWemo[i]);
 }

 
void longPressStart1() {
  longPressStart = !longPressStart;
  //i++;
  //if (i>4) {
  //i=0;
  //}
  //switchON(myWemo[i]);
}



  
