#include <dht.h>
 //what pin is connected to 
#define DHT11_PIN 6

//inititialize dht sensor
dht DHT;


void setup() {                
  Serial.begin(9600);
//  DHT.begin();
  
}

void loop() {
   int chk = DHT.read11(DHT11_PIN);
// wait a few seconds betwee measurements
delay (5000);

//float h = dht.readHumidity();
//float t = dht.readTemperature();

float h = DHT.humidity;
float t = DHT.temperature;


if (isnan(h)|| isnan (t)){
  Serial.println("Failed to read from Dhtll sensor");
  return;
}
 Serial.print(" {\"Humidity\": ");
 Serial.print(h);

  Serial.print(",\"Temperature\": ");
 Serial.print(t);
 Serial.println(" } ");
 

 

   
}
    

