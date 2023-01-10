// #include <Arduino.h>
// #include <Adafruit_Sensor.h>
// #include <DHT.h> 

// #define DHTTYPE DHT11                                               
// #define dht_dpin D1

// DHT dht(dht_dpin, DHTTYPE);                                                  

// void setup() {

//   dht.begin();    
//   Serial.begin(9600);

//   Serial.println("Serial Communication Started\n");  

// }

// void loop() { 
//   float h = dht.readHumidity();                                            
//   float t = dht.readTemperature();                                          
    
//   if (isnan(h) || isnan(t)) {                                               
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }
//   Serial.println("===================================");
//   Serial.print("H = ");
//   Serial.print(h);
//   Serial.print("\n ");
//   Serial.print("F = ");
//   Serial.println(t * 1.8 + 32);
//   Serial.print("C = ");
//   Serial.println(t); 
//   Serial.println("===================================");

//   delay(2000 );


// }

// int outputpin=A0;
// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   Serial.print("HelloWorld");
// }

// void loop() {

//   int analogValue = analogRead(outputpin);
//   float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
//   float celsius = millivolts/10;

//   Serial.print("================= ");
//   Serial.print(outputpin);
//   Serial.println(" ===============");
//   Serial.print("Analog Value: ");
//   Serial.println(analogValue);
//   Serial.println("====================================");
//   Serial.print("C = ");
//   Serial.println(celsius);

//   //---------- Here is the calculation for Fahrenheit ----------//

//   float fahrenheit = ((celsius * 9)/5 + 32);
//   Serial.print("F = ");
//   Serial.println(fahrenheit);
//   Serial.println("====================================");


  
//   delay(2000);
// }

// #include <Arduino.h>
// #include "ESP8266WiFi.h"

// // WiFi parameters to be configured
// const char* ssid = "hydro"; // Write here your router's username
// const char* password = "hydrohydro"; // Write here your router's passward

// void setup(void)
// {
//   Serial.begin(9600);
//   // Connect to WiFi
//   WiFi.begin(ssid, password);

//   // while wifi not connected yet, print '.'
//   // then after it connected, get out of the loop
//   while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.print(".");
//   }
//   //print a new line, then print WiFi connected and the IP address
//   Serial.println("");
//   Serial.println("WiFi connected");
//   // Print the IP address
//   Serial.println(WiFi.localIP());

// }
// void loop() {
//   // Nothing
// }



// GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_Sensor.h>
#include <DHT.h> 

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "TP-Link_8098"
#define WLAN_PASS       "14999703"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.0.101"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);

Adafruit_MQTT_Publish humid = Adafruit_MQTT_Publish(&mqtt, "humid");
Adafruit_MQTT_Publish tmp = Adafruit_MQTT_Publish(&mqtt, "tmp");

void MQTT_connect();

uint8_t ledPin = D6;
uint16_t potAdcValue = 0;
uint16_t ledBrightValue = 0;

#define DHTTYPE DHT22                                               
#define dht_dpin D1

DHT dht(dht_dpin, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".wifi connecting");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  dht.begin();

  delay(2000);
}

void loop() {
  float h = dht.readHumidity();                                            
  float t = dht.readTemperature();                                          
    
    // float h = 24;
    // float t = 23;

  if (isnan(h) || isnan(t)) {                                               
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("===================================");
  Serial.print("H = ");
  Serial.print(h);
  Serial.print("\n ");
  Serial.print("F = ");
  Serial.println(t * 1.8 + 32);
  Serial.print("C = ");
  Serial.println(t); 
  Serial.println("===================================");

  MQTT_connect();

  Serial.print("Sending humidity");
  Serial.print("...");
  if (! humid.publish(h)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print("Sending temperature");
  Serial.print("...");
  if (! tmp.publish(t)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  delay(2000);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


// GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD