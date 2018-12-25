#include <SoftwareSerial.h>
SoftwareSerial esp8266(9,10); 
#include <PS2Keyboard.h>
PS2Keyboard keyboard;
const int DataPin = 2;
const int IRQpin =  3;
#define SSID "wifiname"
#define PASS "wifipassword"
#define IP "host"
String msg = "GET /data/input?member_id=1";
int error = 0;
int i = 0;
int j = 0;

void setup()
{
  Serial.begin(9600); //or use default 115200.
  esp8266.begin(9600);
  Serial.println("AT");
  esp8266.println("AT");
  delay(5000);
  if(esp8266.find("OK")){
    connectWiFi();
  }
  keyboard.begin(DataPin, IRQpin, PS2Keymap_German);
  Serial.println("Interational Keyboard Test:");
}

void loop(){
  start:
  error=0;
  updatebarcode();
  if (error==1){
    goto start;
  }
  delay(1000); 
}

void updatebarcode(){
  if (keyboard.available()) {
    char c[50] = { 0, };
    if (keyboard.available() != NULL && keyboard.available() != PS2_ENTER){
      i = 0;
      while(keyboard.available()) {
        c[i] = keyboard.read();
        i++;
        delay(10);
      }
      i=0;
      while(c[i] != NULL) {
        Serial.print(c[i]);
        i++;
      }
      Serial.println();

      String cmd = "AT+CIPSTART=\"TCP\",\"";
      cmd += IP;
      cmd += "\",80";
      Serial.println(cmd);
      esp8266.println(cmd);
      delay(2000);
      if(esp8266.find("Error")){
        return;
      }
      cmd = msg;
      cmd += "&code=";
      int n;
      if(i > 10){
        for(n = 0; n < 13; n++){
          cmd += c[n];
        }
      }
      else {
        for(n = 0; n < 5; n++){
          cmd += c[n];
        }
      }
      cmd += "\r\n";
      Serial.print("AT+CIPSEND=");
      esp8266.print("AT+CIPSEND=");
      Serial.println(cmd.length());
      esp8266.println(cmd.length());
      if(esp8266.find(">")){
        Serial.print(cmd);
        esp8266.print(cmd);
      }
      else{
        Serial.println("AT+CIPCLOSE");
        esp8266.println("AT+CIPCLOSE");
        //Resend
        error=1;
      }
      
    }
  } else {
    
  }
 
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}
