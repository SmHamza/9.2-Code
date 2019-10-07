//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 6     // what pin we're connected to
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

float temp; 
int freq;
int i;
int j;
int lightsensor = A0;
int light;
bool enter;
bool motionpresent;
bool light_on;
bool motion1;
bool motion2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin(); //temperature
  pinMode(2, INPUT);// motion
  pinMode(3, INPUT);// motion
  pinMode(4, INPUT);// motion
  pinMode(5, INPUT);// motion
  pinMode(11, OUTPUT);// for lights
  pinMode(12, OUTPUT);// for AC or Fans
  pinMode(13, OUTPUT);// for Heater
  pinMode(LED_BUILTIN, OUTPUT);
  bool enter=false;
  bool motionpresent=false;
  bool light_on=false;
}

void entercheck(){
  motion1=false;
  for (i=0; i<300; i++){
    if (digitalRead(2)==HIGH){
      motion1=true;
      i=300;
    }
  }
  if (motion1){
    for (i=0;i<1000; i++){
      if (digitalRead(3)==HIGH){
        enter=true; 
        i=1000;
      }
      delay(2);
    }
  }
}
void exitcheck(){
  motion2=false;
  for (i=0; i<300; i++){
    if (digitalRead(3)==HIGH){
      motion2=true;
      i=300;
    }
  }
  if (motion2){
    for (i=0;i<1000; i++){ 
      if (digitalRead(2)==HIGH){
        enter=false;
        i=1000; 
      }
      delay(2);
    }
  }
  else{
  } 
}
void motioncheck(){
  motionpresent=false;
  freq=0;
  for (j=0;j<2000; j++){
    if ((digitalRead(5)==HIGH) or (digitalRead(4)==HIGH)){
      freq=freq+1;
    }
    delay(5);
  } 
  if (freq >250){
     motionpresent=true;
  }
  if (freq >500){
    enter=true;
  }
  if (freq <50){
    enter = false;  
  }
}
void systemoff(){
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
}

void systemon(){
  if (dht.readTemperature()-temp <10 or dht.readTemperature()-temp >-10){
    temp = dht.readTemperature();
  }
  Serial.print(temp);
  Serial.print(", ");
  if (temp > 25){
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    Serial.print("Heater on");
    Serial.print(", ");
    Serial.print("Fan off");
    Serial.print(", ");
  }
  if (temp < 15){
    digitalWrite(12,LOW); 
    digitalWrite(13,HIGH);
    Serial.print("Heater off");
    Serial.print(", ");
    Serial.print("Fan on");
    Serial.print(", ");
  }
  if (temp >=15 and temp<=25)
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    Serial.print("Heater off");
    Serial.print(", ");
    Serial.print("Fan off");
    Serial.print(", ");
  }
  if (light-analogRead(lightsensor);<10 or light-analogRead(lightsensor); >-10){
    light=analogRead(lightsensor);
  }
  light=analogRead(lightsensor);
  Serial.print(light);
  Serial.print(", ");
  if (not light_on)
  {
    if (light < 200){
      digitalWrite(11,HIGH);
      light_on = true;
    }
  }
  else{
    if (light>600){
      digitalWrite(11,LOW);
      light_on = false;
    }
  }
  if (light_on){
      Serial.print("Lights on");
      Serial.print(", ");
  }
  else{
    Serial.print("Lights off");
    Serial.print(", ");
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  if (not enter){
    entercheck();
  }
  else{
    exitcheck();
  }
  motioncheck();
  if (enter){
    Serial.print("Enter, ");
  }
  else{
    Serial.print("Exited, ");
  }
  if (motionpresent){
    Serial.print("Active, ");
  }
  else{
    Serial.print("Inactive, ");
  }
  if (motionpresent or enter){
    digitalWrite(LED_BUILTIN,HIGH);
    systemon();
  }  
  else{
    digitalWrite(LED_BUILTIN,LOW);
    systemoff();
  }
  delay(1000);
  Serial.println();
}

