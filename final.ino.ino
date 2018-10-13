String data,url2,d1,d2;
#include <SoftwareSerial.h>   
SoftwareSerial mySerial(D2,D3);

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid ="abcd";
const char* password ="12345678";
#define ORG "6n8x62"
#define DEVICE_TYPE "Nina"
#define DEVICE_ID "nina1234"
#define TOKEN "i_B&vbp)X3+i)*qMmZ"
//-------- Customise the above values --------

String command;

void callback(char* topic, byte* payload, unsigned int payloadLength);
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/data/fmt/json";
char topic1[]="iot-2/cmd/data/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

float d11,d22;

WiFiClient wifiClient;
PubSubClient client(server, 1883,callback,wifiClient);

int publishInterval = 5000; // 30 seconds
long lastPublishMillis;
void publishData();

//int k=1;

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);

 ////////////////// i added
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());

 //////////////////////////////
 mqttConnect();
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  if (millis() - lastPublishMillis > publishInterval)
  {
    publishData();
    lastPublishMillis = millis();
  }
  if (!client.loop())
  {
    mqttConnect();
  }
  if (mySerial.find(","))
  {
  url2=mySerial.readStringUntil('!');
  Serial.println(url2);
  d1 = getValue1(url2, ',', 0);
  d2 = getValue1(url2, ',', 1);
  Serial.println(d1);
  Serial.println(d2);
  }
d11 = d1.toFloat();
d22 = d2.toFloat();
publishData();
delay(1000);
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic1)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);

if(command == "on"){
  digitalWrite(D4,LOW);
  Serial.println("PUMP is Switched ON");
}
else if(command == "off"){
  digitalWrite(D4,HIGH);
  Serial.println("PUMP is Switched OFF");
}
else if(command == "pon"){
  digitalWrite(D5,LOW);
  Serial.println("PUMP is Switched ON");
}
else if(command == "poff"){
  digitalWrite(D5,HIGH);
  Serial.println("PUMP is Switched OFF");
}


command ="";
}

void publishData(){
  /*if((d11<650)||(d22<650))
  {
    digitalWrite(D4,HIGH);
    k=1;
  }
  else if(((d11>=650)||(d22>=650))&&(k==1))
  {
    digitalWrite(D4,LOW);
    k=0;
  }*/
  String payload = "{\"d\":{\"sv1\":";
  payload += d11;
  payload+="," "\"sv2\":";
  payload += d22;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

String getValue1(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
        
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
