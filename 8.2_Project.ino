#include <DHT.h>

//Constants
#define DHTPIN 6     // what pin we're connected to
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

float temp; 
int i;
int j;
int lightsensor = A0;
int light;
int freq;
bool enter;
bool motionpresent;
bool light_on;

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
  
  if (digitalRead(2)==HIGH){
    Serial.print("Checking entering, ");
    for (i=0;i<1000; i++){
      if (digitalRead(3)==HIGH){
        enter=true; 
        i=1000;
      }
      delay(1);
    }
    
  }
}
void exitcheck(){
 if (digitalRead(3)==HIGH){
    Serial.print("Checking exiting, ");
    for (i=0;i<1000; i++){ 
      if (digitalRead(2)==HIGH){
        enter=false;
        i=1000; 
      }
      delay(1);
    }
  }
  else{
  } 
}
void motioncheck(){
  motionpresent = false;
  freq = 0;
  for (j=0;j<2000; j++){
    if ((digitalRead(5)==HIGH) or (digitalRead(4)==HIGH)){
      freq=freq+1;
    }
    if (freq > 250){
      motionpresent=true;
    }
    if (freq > 500){
      enter=true;
      j=2000;
    }
    if(freq<50){
      enter=false;
    }
    delay(5);
  }   
}
void systemoff(){
  Serial.print("System off");
  Serial.print(", ");
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
}

void systemon(){
  temp = dht.readTemperature();
  Serial.print(temp);
  Serial.print(", ");
  if (temp > 25){
    Serial.print("Fan on, ");
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
  }
  if (temp < 15){
    
    Serial.print("Heater on, ");
    digitalWrite(12,LOW); 
    digitalWrite(13,HIGH);
  }
  if (temp >=15 and temp<=25)
  {
    Serial.print("Heater off, Fan off, ");
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
  }
  light=analogRead(lightsensor);
  Serial.print(light);
  Serial.print(", ");
  if (light_on = false)
  {
    if (light < 200)
    {
      Serial.print("Lights on, ");
      digitalWrite(11,HIGH);
    }
  }
  else{
    if (light>1000){
      Serial.print("Lights off, ");
      digitalWrite(11,LOW);
    }
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
  if (motionpresent or enter){
    digitalWrite(LED_BUILTIN,HIGH);
    systemon();
  }  
  else{
    digitalWrite(LED_BUILTIN,LOW);
    systemoff();
  }

  if (enter){
    Serial.print("Entered, ");
  }
  else{
    Serial.print("Exited, ");
  }
  if (motionpresent){
    Serial.print("motion in room, ");
  }
  else{
    Serial.print("No motion, ");
  }
  Serial.println("");
  
  delay(100);
}
