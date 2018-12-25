#include <ESP8266WiFi.h>

int ledDigitalOne[] = {12, 13, 14};

const boolean ON = HIGH;

const boolean OFF = LOW;   

const boolean RED[] = {ON, OFF, OFF};    
const boolean GREEN[] = {OFF, ON, OFF}; 
const boolean BLUE[] = {OFF, OFF, ON}; 
const boolean YELLOW[] = {ON, ON, OFF}; 
const boolean CYAN[] = {OFF, ON, ON}; 
const boolean MAGENTA[] = {ON, OFF, ON}; 
const boolean WHITE[] = {ON, ON, ON}; 
const boolean BLACK[] = {OFF, OFF, OFF}; 

const boolean* COLORS[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, BLACK};


const char* ssid     = "wifiname";
const char* password = "wifipassword";
 
const char* host = "host";
 
int value = 1; 
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  for(int i = 0; i < 3; i++){
   pinMode(ledDigitalOne[i], OUTPUT);
  }

  
}
  
void loop() {
  delay(1000);
   
  Serial.print("Connecting to ");
  Serial.println(host);
  
  WiFiClient Client;
  const int httpPort = 80;
  if (!Client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  String url = "/member?member_id=1";
  Serial.print("Requesting URL: ");
  Serial.println(host + url);
  Serial.println(String("TRY: ") + value + ".");
  
  Client.print(String("GET ") + url + "member_id=1" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(500);
  String reault;
  while(Client.available()){
    reault = Client.readStringUntil('\r');
    Serial.println();
  }
  int result = atoi( &reault[ 0 ] );

  Serial.println(reault);
  Serial.println(result);
  if (result >= 1000){
    setColor(ledDigitalOne, GREEN);
  }
  else if (result >= 750){
    setColor(ledDigitalOne, BLUE);
  }
  else if (result >= 500){
    setColor(ledDigitalOne, YELLOW);
  }
  else if (result >= 250){
    setColor(ledDigitalOne, MAGENTA);
  }
  else {
    setColor(ledDigitalOne, RED);
  }
  Serial.println("");
  Serial.println(String("Try nr. ") + value + " is finished.");
  Serial.println("Waiting for next try...");
  Serial.println("");
  value = value + 1;
  delay(1000);
}

void setColor(int* led, boolean* color){
 for(int i = 0; i < 3; i++){
   digitalWrite(led[i], color[i]);
 }
}

void setColor(int* led, const boolean* color){
  boolean tempColor[] = {color[0], color[1], color[2]};
  setColor(led, tempColor);
}
