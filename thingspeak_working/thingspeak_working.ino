// Esp upload to thingspeak using arduni mega complete working code
#include <DHT.h>        //Attention: For new  DHT11 version  library you will need the Adafruit_Sensor library
                        //Download from here: https://github.com/adafruit/Adafruit_Sensor



//Attention: For new  DHT11 version  libraries you will need the Adafruit_Sensor library
//Download from here:https://github.com/adafruit/Adafruit_Sensor
//and install to Arduino software

#define DHTPIN 6                // Connect the signal pin of DHT11 sensor to digital pin 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);


String apiKey = "YFQJHA81LPGWGYBI";   
String ssid="Free 2017";    // Wifi network SSID
String password ="wifi3210";  // Wifi network password

boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (Serial1.available()){
        c=Serial1.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

//========================================================================
boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(Serial1.find(">")){
    Serial1.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    Serial1.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
//================================================================================ setup
void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  
  dht.begin();          // Start DHT sensor
  
  Serial1.begin(115200);  // enable software serial
                          // Your esp8266 module's speed is probably at 115200. 
                          // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
                          // and upload. Then change to 9600 and upload again
  
  //Serial1.println("AT+CIOBAUD=9600");         // set esp8266 serial speed to 9600 bps
  //Serial1.println("AT+UART_CUR=9600,8,1,0,0");         // set esp8266 serial speed to 9600 bps
  
  showResponse(1000);
  
  Serial1.println("AT+RST");         // reset esp8266
  showResponse(1000);

  Serial1.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}


// ====================================================================== loop
void loop() {

  // Read sensor values
   float t = dht.readTemperature();
   float h = dht.readHumidity();
        if (isnan(t) || isnan(h)) {
        if (DEBUG) Serial.println("Failed to read from DHT");
      }
      else {
          if (DEBUG)  Serial.println("Temp="+String(t)+" *C");
          if (DEBUG) Serial.println("Humidity="+String(h)+" %");
           thingSpeakWrite(t,h);                                      // Write values to thingspeak
      }
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(20000);  
}




