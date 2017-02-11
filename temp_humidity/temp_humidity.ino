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
Serial.print("Current humidity = ");
Serial.print(h);
Serial.print("% ");
Serial.print("temperature = ");
Serial.print(t);
Serial.println("C ");
delay(5000);//Wait 5 seconds before accessing sensor again.
//Fastest should be once every two seconds.

 
 }
    

