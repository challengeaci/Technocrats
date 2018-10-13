#include <SoftwareSerial.h>
SoftwareSerial myserial(2,3);// rx,tx
void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  Serial.begin(115200);
  myserial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int x,y;
  Serial.print("\nSensor 1 connected to A0\n");
  x=analogRead(A0);
  Serial.print(x);
  delay(2000);
  Serial.print("\nSensor 2 connected to A1\n");
  y=analogRead(A1);
  Serial.print(y);
  delay(2000);
  String data= ","+String(x)+","+String(y)+"!";
  myserial.println(data);
  Serial.println(data);
}
