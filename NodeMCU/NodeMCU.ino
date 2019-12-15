void Line_Notify(String message) ;
void UARTreceive();

#include <ESP8266WiFi.h>
//#include <WiFiClientSecureAxTLS.h>

// Config connect WiFi
#define WIFI_SSID "Smoothy"
#define WIFI_PASSWORD "123987456"

// Line config
#define LINE_TOKEN "Q5nui5hlRCl3fl8YbvO1hkw0lQeczJ8Z1xcBuKAsVpC"
#define   RcvCount 7

//#define SW D2

String message = "push"; // ArduinoIDE
String UARTrcvData;                            // AUTO
char UARTrcvBuffer;                          // AUTO
int UARTrcvCount;                           // AUTO
unsigned long startTime;
unsigned long currentTime;


void setup() {
  //pinMode(SW, INPUT);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  startTime = 0;
  currentTime = 0;
}

void loop() {
  currentTime = millis();
  if(currentTime - startTime >= 1000){
    Serial.println("Enter !");
    UARTreceive();
    message = UARTrcvData;
    Line_Notify(message); 
  }
}

void Line_Notify(String message) {
  //axTLS::WiFiClientSecure client; 
  Serial.println(message);
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
   Serial.println(req);
  client.print(req);
    
  delay(20);

   Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void UARTreceive() { // ----------------------------------------------------------------
  UARTrcvCount = 0;
  UARTrcvBuffer = 'a';
  UARTrcvData = "";
  while (Serial.available() && Serial.read()!='s');
  while (Serial.available() && UARTrcvBuffer != '\r') {
      //UARTrcvCount = UARTrcvCount + 1;
      UARTrcvBuffer = Serial.read();
      UARTrcvData += UARTrcvBuffer;
  }
  Serial.println(UARTrcvData);
  serial_flush();
}

void serial_flush(){
  while(true){
    delay(20);
    if(Serial.available()){
      {
        while (Serial.available())
          Serial.read();
        continue;
      }
    }
    else
      break;  
  }
}

