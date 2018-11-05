#include "SoftwareSerial.h"
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myStatus = "";
int prueba=0,TempThingspeak=0,HumiThingSpeak=0,RadUVThingSpeak=0;






SoftwareSerial mySerial(13, 15, false, 256);//(D7,D8)
volatile char tokenChar = '-';
char datos[12];
String CadenaDatosArrive="",Temperatura="",Humedad="",RadiacionUV="",dataReceived="";
unsigned long Tiempo_Tx;
int posicion=0;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Tiempo_Tx=millis();
    WiFi.mode(WIFI_STA); 
    ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()-Tiempo_Tx)>18000){
        RecibirDatos();
        SepararDatos();
        thingSpeak();
        Tiempo_Tx=millis();
        
  }
}
void RecibirDatos(){
        while (mySerial.available()>0){
        delay(200);
        datos[posicion]=mySerial.read();
        CadenaDatosArrive+= datos[posicion];
        posicion++;}
        dataReceived=CadenaDatosArrive;
        Serial.println("Los datos que llegan juntos son: "+CadenaDatosArrive);
       
       
}
void SepararDatos(){
  Humedad=dataReceived.substring(0, dataReceived.indexOf(tokenChar));
  dataReceived = dataReceived.substring(Humedad.length() + 1);
  Temperatura = dataReceived.substring(0, dataReceived.indexOf(tokenChar));
  dataReceived = dataReceived.substring(Temperatura.length() + 1);
  RadiacionUV = dataReceived.substring(0, dataReceived.indexOf(tokenChar));
  Serial.println("Los datos que llegaron desde Arduino son: ");
  Serial.println("Humedad: "+Humedad );
  Serial.println("Temperatura: "+Temperatura );
  Serial.println("RadiacionUV: "+RadiacionUV );
  TempThingspeak=Temperatura.toInt();
  HumiThingSpeak=Humedad.toInt();
  RadUVThingSpeak=RadiacionUV.toInt();
 
}

void thingSpeak(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, HumiThingSpeak);
  ThingSpeak.setField(2, TempThingspeak);
  ThingSpeak.setField(3, RadUVThingSpeak);
  ThingSpeak.setField(4, prueba++);
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  }
