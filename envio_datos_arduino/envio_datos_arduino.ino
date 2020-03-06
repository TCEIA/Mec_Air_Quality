#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS.h>


//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
#define RXD2 16
#define TXD2 17
float lat = 0,lon = 0;
int counter = 0;
int contador2=0;
TinyGPS gps;

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));  
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xFF);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
    

  //Parte_GPS
  while(Serial2.available()){ // check for gps dat
    Serial2.print(Serial2.read());
  } 
  while(not Serial2.available()){ // check for gps dat
    delayMicroseconds(500);
  } 
  while(Serial2.available()){ // check for gps dat
    if(gps.encode(Serial2.read()))// encode gps data 
    {  
      gps.f_get_position(&lat,&lon); // get latitude and longitude 
     } 
  }
    
   
String latitude = String(lat,6); 
  String longitude = String(lon,6);   
  Serial.println(latitude+";"+longitude); 
//End Parte_GPS


  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(latitude);
  LoRa.print(",");
  LoRa.print(longitude);
  LoRa.endPacket();
  delay(500);
  
} 
