#include <ESP8266WiFi.h>
int IRPin =2;
int led=13;
int value;
void setup(){
pinMode(IRPin,INPUT);
Serial.begin(9600);
pinMode(led,OUTPUT);
}
void loop(){
value = digitalRead(IRPin);
Serial.println(value);
if(digitalRead(IRPin)==0)
{
digitalWrite(led,HIGH);
Serial.println("object detected");
}
else
{
digitalWrite(led,LOW);
Serial.println("object not detected");
}
}
