#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
SoftwareSerial espSerial(2, 3); // RX, TX
int temperatura=25;
int humedad=60;
int radiacionUV=1234;
char datos[12];
int posicion=0;
String EnviaraEsp="",T="",H="",RUV="",DatosFromEsp;
unsigned long Tiempo_Tx;
DHT dht(A0, 11);//Dht pin,DhtType el pin A0 es usado como entrada analogica para leer los datos del sensor DHT11.
int sensorPinUV = A1;

void setup() {
  // put your setup code here, to run once:
    espSerial.begin(9600);//Puerto serial para comunicacion con NodeMCU
    Serial.begin(9600);//Puerto Serial par visualizad datos de ingreso y salida en el Monitor Serial
    dht.begin();//inicializo el sensor dht(Libreria)
    Tiempo_Tx=millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()-Tiempo_Tx)>18000){//Transmision cada cierto intervalo de tiempo
    leerSensores();
    TransmitirDatos();
    Tiempo_Tx=millis();
  }
}

void leerSensores(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humedad = dht.readHumidity();
  // Read temperature as Celsius (the default)
    temperatura = dht.readTemperature();
    radiacionUV=analogRead(sensorPinUV);
  
  
  
  
  }
void TransmitirDatos(){
    T=String(temperatura,DEC);
    H=String(humedad,DEC);
    RUV=String(radiacionUV,DEC);
    EnviaraEsp=H+"-"+T+"-"+RUV;
    Serial.println(EnviaraEsp);
    espSerial.println(EnviaraEsp);
    Serial.println("Valores de los Sensores: ");
    Serial.println("Temperatura: "+T);
    Serial.println("Humedad: "+H);
    Serial.println("Radiacion UV: "+RUV);
    while (espSerial.available()>0){
        delay(5);
        datos[posicion]=espSerial.read();
        DatosFromEsp+= datos[posicion];
        posicion++;}
    Serial.println("Datos desde el ESP: "+DatosFromEsp);
 }
