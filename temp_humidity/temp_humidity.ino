
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>
#define DHT11_PIN 6
dht DHT;

//on/off sensor 1
//int sensor1=6;
/**int sensor2=7;
int sensor3=8;
int sensor4=9;
**/
int sensor1State=0;

int sensor2State=0;
/**
int sensor3State=0;
int sensor4State=0;

**/


// replace with your channel's thingspeak API key
String apiKey = "YFQJHA81LPGWGYBI";

// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX

int i=1;


// this runs once
void setup() {                
  // initialize the sensor at 7th pin as an input.
 // pinMode(sensor1,INPUT);
 /** pinMode(sensor2,INPUT);
  pinMode(sensor3,INPUT);
  pinMode(sensor4,INPUT);
  **/
  
  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  ser.begin(9600);
  
  // reset ESP8266
  ser.println("AT+RST");
}


// the loop 
void loop() {

 int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(1000);
  
  //reading the state of sensor1
  //sensor1State = digitalRead(sensor1);
       
       
    //sensor1State = digitalRead(DHT.temperature);
     sensor1State = DHT.temperature;
     sensor2State = DHT.humidity;
     Serial.println(sensor1State);
     Serial.println(sensor2State);

  
 /** sensor2State = digitalRead(sensor2);
  sensor3State = digitalRead(sensor3);
  sensor4State = digitalRead(sensor4);
  **/
  
  String state1=String(sensor1State);
  

  String state2=String(sensor2State);
   /** 
  
  String state3=String(sensor3State);
  
  
  String state4=String(sensor3State);
  **/
  
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  /*if(i==0)
  {
    Serial.println("1");
  getStr +="&field1=";
  getStr += String(state1);
  i++;
  }
  else if(i==1)
  {Serial.println("2");
  getStr +="&field2=";
  getStr += String(state2);
  i++;
  }
  else if(i==2)
  {
    Serial.println("3");
  getStr +="&field3=";
  getStr += String(state3);
  i++;
  }
  else if(i==3)
  {
    Serial.println("4");
  getStr +="&field4=";
  getStr += String(state4);
  i=0;
  }
  
  */
  getStr +="&field1=";
  getStr += String(state1);
  
  getStr +="&field2=";
  getStr += String(state2);
  
  /**
  getStr +="&field3=";
  getStr += String(state3);
   getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";
  
  **/

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);
   Serial.println("Attempting send");

 if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);

   
  }
  
  else{

   while (ser.available())
    {
         char c = ser.read();
         Serial.write(ser.read());
        if (c == '\r') Serial.print('\n');
    }
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
    
   
}
    
  // thingspeak needs 15 sec delay between updates
  delay(20000);  
}
