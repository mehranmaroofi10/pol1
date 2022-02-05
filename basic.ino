
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

String ap = " Smart_Home  ";
String ap2;
uint8_t mac[6];
char const macStr[19] = {0};

void ID_Seting() {
    ap2 = ap + GetMyMacAddress();
    char *macStr = new char[ap2.length()+ 1 ];
    strcpy(macStr, ap2.c_str());
    Serial.print("ap2"); Serial.print("    " ); Serial.print( ap2); Serial.println(" String");
}

String GetMyMacAddress()
{
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.macAddress(mac);
  sprintf(macStr, "%02X%02X%02X%02X%02X%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]); // no :'s

  return  String(macStr);
}
