#include <Wire.h>                    // wire library is used with the lcd display
#include "rgb_lcd.h"                //declaration of the lcd
#include <DHT.h>                   //got this from here: https://github.com/adafruit/Adafruit_Sensor
#define DHTPIN 6                  // Connect the signal pin of DHT11 sensor to digital pin 6
#define DHTTYPE DHT11            // gloabal declaration of the DHT11 module and type 
DHT dht(DHTPIN, DHTTYPE);       // initialization if the DHT pin and the type
rgb_lcd lcd;

#define trigPin 5
#define echoPin 4
int binHeight = 11.5;       //silo height is 18 cm
float binPercent;
float binPercenttwo;
float binpercentThree;


const int pinRed = 9;        // initialize the danger pin
const int pinNorm = 8;      // initialize normal pin

String apiKey = "YFQJHA81LPGWGYBI";     //channel's thingspeak WRITE API key

String ssid = "Free 2017";  // Wifi network SSID
String password = "wifi3210"; // Wifi network password

boolean DEBUG = true; //sets the serial monitor to provide errors if it encounter any during the execution
// Initializing colors of the LCD display
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//======================================================================== showResponce

// function checks if there is any serial read on the serial monitor then prints it on the LCD screen
void showResponse(int waitTime) {
  long t = millis();
  char c;
  while (t + waitTime > millis()) {
    if (Serial1.available()) {
      c = Serial1.read();
      if (DEBUG) Serial.print(c);
    }
  }

}

//========================================================================

// Function to connect to the thingspeak channel through authenticatiomn
boolean thingSpeakWrite(float value1, float value2,float value3) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if (Serial1.find("Error")) {
    if (DEBUG) Serial.println("AT+CIPSTART error");  // AT command to send data to thingspeak channel
    return false;
  }


  String getStr = "GET /update?api_key=";       // prepare GET string
  getStr += apiKey;

  getStr += "&field1=";                         // Temperature field
  getStr += String(value1);                      //Convering temperature field to string
  getStr += "&field2=";                          // Humidity /field
  getStr += String(value2);                       // Coverting humidity to string
  getStr +="&field3=";                            // Silo levels
  getStr += String(value3);
  // ...
  getStr += "\r\n\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());                   //combine data from all the strings from the arduino
  Serial1.println(cmd);
  if (DEBUG)  Serial.println(cmd);

  delay(100);
  if (Serial1.find(">")) {
    Serial1.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else {
    Serial1.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}

//================================================================================ setup

void setup()
{

  DEBUG = true;         // enable debug serial

  //Initializing the Pins of the Ultrasonic Sensor (Trig pin as the output and Echo pin as the input)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);                  // starts the serial
  pinMode(pinRed, INPUT);             // initialize the Danger Pin
  pinMode(pinNorm, INPUT);           // initialize the Normal  Pin
  dht.begin();                       // Start DHT sensor

  Serial1.begin(115200);             // enable software serial

  showResponse(1000);

  Serial1.println("AT+RST");         // reset esp8266
  showResponse(1000);

  Serial1.println("AT+CWMODE=1");     // set esp8266 as client
  showResponse(1000);

  Serial1.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\""); // set your home router SSID and password
  showResponse(5000);

  if (DEBUG)  Serial.println("Setup completed");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);

}

int CheckDistance()
{
 long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}
// ====================================================================== loop

void loop()
{
  // Calculate percentages of Silo fill
  int testDistance = CheckDistance();           /// get object distance using ping
   float binPercent = 11.5 - testDistance;
  float binPercenttwo = binPercent / 11.5;
  float binpercentThree =  binPercenttwo * 100;

  int siloHeight = binpercentThree;

  if (binpercentThree > 90) /// if silo is more than 90% full
  {
    Serial.println("Silo is almost full");
  }
  Serial.print(binPercent);
  Serial.println("Empty");
  Serial.print(testDistance);
  Serial.println(" cm");
  delay(500);


  // Read sensor values
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    if (DEBUG) Serial.println("Failed to read from DHT");
  }
  else {
    if (DEBUG)  Serial.println("Temp=" + String(t) + " *C");
    if (DEBUG) Serial.println("Humidity=" + String(h) + " %");

   if (siloHeight>0){
       thingSpeakWrite(t, h, siloHeight);                                     // Write values to thingspeak
   }
   
  }

  if (t > 29 || t < 25 || h > 50)
  {
    digitalWrite(pinRed, HIGH);
    digitalWrite(pinNorm, LOW);

  }
  else
  {
    digitalWrite(pinNorm, HIGH);
    digitalWrite(pinRed, LOW);
  }



  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  // Print a temperature and humidity to the LCD.
  //lcd.print("Temperature is", t "Humidity is", h );
  lcd.print("T=" + String(t) + "\n");
  lcd.print("H=" + String(h));


  delay(20000);     // thingspeak needs 15 sec delay between updates,
}






