#define BLYNK_PRINT Serial

#include <FS.h>

#include<ESP8266WiFi.h>

#include<BlynkSimpleEsp8266.h>

#include <WiFiManager.h>


//**************************************************************************

//**************************************************************************
//ESP8266WiFiMulti WiFiMulti;
WiFiManager wifiManager; //add this item in this line
//**************************************************************************
const int Reset_Pin = 14; //D5 for reset factory
#define RelayPin1 5 //D1  // signal pin for relay 
#define SwitchPin1 12 //D2 // input signal from physical Button
#define wifiLed 16 //D0   // wifi Status LED 
#define VPIN_BUTTON_1 V1

//*************************************************
int Push_button_state_reset_factory = 0;  
//*************************************************

//*************************************************
int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
//****************************************************
int wifiFlag = 0;
//***************************************************************************
//***************************************************************************
char blynk_token[34] = "YOUR_BLYNK_TOKEN";
bool shouldSaveConfig = false;
BlynkTimer timer;
//*********************************************************************************
void( * resetFunc)(void) = 0;
BLYNK_WRITE(V2) // Reset
{
  if (param.asInt() == 1) {
    resetFunc();
    Serial.println("virtual Reset");
    delay(5000);
  }
//**********************************************************************************


//**********************************************************************************
}
float sensorSuhu() {
  pinMode(A0, INPUT);
  return analogRead(A0) * 0.322265625;
}

//*************************************************************************************
void relayOnOff(int relay) {

  switch (relay) {
  case 1:
    if (toggleState_1 == 1) {
      digitalWrite(RelayPin1, LOW); // turn on relay 1
      toggleState_1 = 0;
      Serial.println("Device1 ON");
    } else {
      digitalWrite(RelayPin1, HIGH); // turn off relay 1
      toggleState_1 = 1;
      Serial.println("Device1 OFF");
    }
    delay(100);
    break;
  default:
    break;
  }
  Serial.println("part1_worked");
}
//*********************************************************************************
void with_internet() {
  //Manual Switch Control
  if (digitalRead(SwitchPin1) == LOW) {
    delay(200);
    relayOnOff(1);
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); // Update Button Widget  
  }
}
void without_internet() {
  //Manual Switch Control
  if (digitalRead(SwitchPin1) == LOW) {
    delay(200);
    relayOnOff(1);
  }

}
////********************************************************************************
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);

}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}
//************************************************************************************
void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;                                         //wifi status D0 or gpoi 16
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED         
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
  }
}
//*************************************************************************

//************************************************************************  
void setup() {
  Serial.begin(9600);
  initialize();
  Blynk.begin(blynk_token, WiFi.SSID().c_str(), WiFi.psk().c_str(), IPAddress(80, 210, 31, 100), 9442);
  //  timer.setInterval(1000L, sendSensor);
  pinMode(RelayPin1, OUTPUT);
  pinMode(wifiLed, OUTPUT); //wifi status
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(Reset_Pin, INPUT_PULLUP);
  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  Serial.println("part2_worked");
  timer.setInterval(3000L, checkBlynkStatus);
  // check if Blynk server is connected every 3 seconds
  //   Serial.setDebugOutput(true);
}
void loop() {
//************************************************************************************************
  Push_button_state_reset_factory = digitalRead(Reset_Pin);
  if (Push_button_state_reset_factory == 0) {
    Serial.println("wifi config begun...................");
    delay(200);
    delay(1000);
    ESP.eraseConfig();                        //this block of code for reset factory
    delay(4000);
    ESP.reset();
//***************************************************************************************************

//****************************************************************************************************
  } else {
    delay(100);                         //this is for normal status of esp8266 and every thing is OK
    Serial.println("wifi_stable");
  }
//***************************************************************************************************
  Blynk.run();
  timer.run();
   
}
