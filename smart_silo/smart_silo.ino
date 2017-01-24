#include<dht.h>
#define DHT11_PIN 2

dht DHT;
int  greenLED=9;
int redLED=7;
 

//code for red Led
int timeOn=200;
int timeOff=300;

//code for yellow Led
int onTime=100;
int offTime=900;

// if you require to change the pin number, Edit the pin with your arduino pin.


void setup() {
  
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  Serial.begin(9600);

  Serial.println("welcome to Smart Silo Project, Now measuring Temperature and Humidity ");
}

void loop() { // READ DATA

 digitalWrite(greenLED, HIGH);
  delay (onTime);
  
  int chk = DHT.read11(DHT11_PIN);

  Serial.println(" Humidity " );

  Serial.println(DHT.humidity, 1);

  Serial.println(" Temparature ");

  Serial.println(DHT.temperature, 1);

  delay(2000);

if (DHT.temperature >= 28)
{
   digitalWrite(greenLED, LOW);
      delay (timeOff);
      
  digitalWrite(redLED, HIGH);
  delay (timeOn);

  
}
else 
{
    digitalWrite(redLED, LOW);
  delay (timeOff);

}
}


