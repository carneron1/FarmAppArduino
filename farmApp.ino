#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "TemperatureController.h"
#include "HumidityController.h"
#include "lightController.h"

#define DHTPIN D4
#define RELEPIN D0
#define HUMPIN A0
#define DHTTYPE DHT22
#define LEDPIN D8
#define LDRPIN D7

DHT dht(DHTPIN, DHTTYPE);
TemperatureController tempControl(DHTPIN);
HumidityController humControl(HUMPIN, RELEPIN);
LightController lightControl(LDRPIN, LEDPIN);

//*********** MQTT Y WIFI CONFIG *******

const char* ssid = "**nombre_de_red**";
const char* password = "**password**";
const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "mqtt.dioty.co";

const int mqtt_port = 1883;
const char* mqtt_user = "maximilianocalderon87@gmail.com";
const char* mqtt_pass = "1c110101";

WiFiClient espClient;
PubSubClient client(espClient);
String msg;
char* topic;
bool manualMode = false;
bool lightManual = false;
bool irrigationManual = false;

void callback(char* topic, byte* payload, unsigned int length); // Se ejecuta cuando recibe algun msg de un topic
void reconnect(); //Reconecta con el broker mqtt y se subscribe a los topics
void setup_wifi(); //Inicaliza wifi
void readMsg(); //Recibe el topic, el msg y ejecuta el codigo correspondiente

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
  pinMode(RELEPIN,OUTPUT);
  digitalWrite(RELEPIN, LOW);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LDRPIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
	}
  if(!manualMode){
    humControl.checkStatus();
    lightControl.checkStatus();
  }
  readMsg();
  client.loop();
  delay(1000);
}

//**************FUNCIONES**************

void readMsg(){
  if (msg.length()>0){
    Serial.println(msg);
    Serial.println(topic);
    
    char val[50];

    if (msg == "ambientHumidity"){
      String aux = String(tempControl.getHum(dht));
      aux.toCharArray(val,50);
      client.publish("/maximilianocalderon87@gmail.com/getAmbientHumidity",val);
    }

    if (msg == "ambientTemp"){
      String aux = String(tempControl.getTemp(dht));
      aux.toCharArray(val,50);
      client.publish("/maximilianocalderon87@gmail.com/getAmbientTemp",val);
    }

    if (msg == "floorHumidity"){ //seco=640 mojado=220
      String aux = String(humControl.getHum());
      aux.toCharArray(val,50);
      client.publish("/maximilianocalderon87@gmail.com/getFloorHumidity",val);
    }

    if (msg == "floorHumidityTarget"){ 
      String aux = String(humControl.getTargetValue());
      aux.toCharArray(val,50);
      client.publish("/maximilianocalderon87@gmail.com/getFloorHumidityTarget",val);
    }

    if (msg == "irrigationState"){ 
      client.publish("/maximilianocalderon87@gmail.com/getIrrigationState",humControl.getReleState());
    }

    if (msg == "manualMode"){
      char* val;
      if (manualMode) val = "true";
      else val = "false";
      client.publish("/maximilianocalderon87@gmail.com/getManualMode",val);
    };

    if (msg == "lightState"){ 
      client.publish("/maximilianocalderon87@gmail.com/getLightState",lightControl.getLedState());
    };

    // if (msg == "manualLightState"){ 
    //   client.publish("/maximilianocalderon87@gmail.com/getLightState",lightControl.getLedState());
    // };

    if (msg == "manualLightState"){
      char* val;
      if (lightManual) val = "on";
      else val = "off";
      client.publish("/maximilianocalderon87@gmail.com/getLightManualState",val);
    };

    if (msg == "manualIrrigationState"){
      char* val;
      if (irrigationManual) val = "on";
      else val = "off";
      client.publish("/maximilianocalderon87@gmail.com/getIrrigationManualState",val);
    };

    if (String(topic) == "/maximilianocalderon87@gmail.com/setHumidityTarget"){
      float val = msg.toFloat();
      humControl.setTargetValue(val);
    };
    
    if (String(topic) == "/maximilianocalderon87@gmail.com/setManualMode"){
      if (msg == "true"){
        manualMode = true;
        digitalWrite(LEDPIN, LOW);
        digitalWrite(RELEPIN, LOW);
        lightManual = false;
        irrigationManual = false;
      }else{
        manualMode = false;
      }
    }

    if (String(topic) == "/maximilianocalderon87@gmail.com/setLightManual"){
      if (msg == "on"){
        lightManual = true;
        digitalWrite(LEDPIN, HIGH);
      }else{
        lightManual = false;
        digitalWrite(LEDPIN, LOW);
      }
    }

    if (String(topic) == "/maximilianocalderon87@gmail.com/setIrrigationManual"){
      if (msg == "on"){
        irrigationManual = true;
        digitalWrite(RELEPIN, HIGH);
      }else{
        irrigationManual = false;
        digitalWrite(RELEPIN, LOW);
      }
    }

    if (String(topic) == "/maximilianocalderon87@gmail.com/isConnected"){
      char* val = "true";
      client.publish("/maximilianocalderon87@gmail.com/getIrrigationManualState",val);
    };
    msg="";
  }
}

void reconnect() {

	while (!client.connected()) {
		Serial.print("Intentando conexión Mqtt...");
		String clientId = "FARMAPP-";
		clientId += String(random(0xffff), HEX);
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Conectado");
      if(client.subscribe("/maximilianocalderon87@gmail.com/requestSensorValue")){
      Serial.println("Suscripcion sensores ok");
      }else{
        Serial.println("fallo Suscripción sensores");
      }
      if(client.subscribe("/maximilianocalderon87@gmail.com/setHumidityTarget")){
        Serial.println("Suscripcion humedad objetivo ok");
      }else{
        Serial.println("fallo Suscripción humedad");
      }
      if(client.subscribe("/maximilianocalderon87@gmail.com/setManualMode")){
        Serial.println("Suscripcion modo manual ok");
      }else{
        Serial.println("fallo Suscripción modo manual");
      }
      if(client.subscribe("/maximilianocalderon87@gmail.com/setIrrigationManual")){
        Serial.println("Suscripcion irrigacion manual ok");
      }else{
        Serial.println("fallo Suscripción irrigacion manual");
      }
      if(client.subscribe("/maximilianocalderon87@gmail.com/setLightManual")){
        Serial.println("Suscripcion iluminacion manual ok");
      }else{
        Serial.println("fallo Suscripción iluminacion manual");
      }
      if(client.subscribe("/maximilianocalderon87@gmail.com/isConnected")){
        Serial.println("Suscripcion iluminacion manual ok");
      }else{
        Serial.println("fallo Suscripción iluminacion manual");
      }
		} else {
			Serial.print("falló con error -> ");
			Serial.print(client.state());
			Serial.println(" Reintento en 5 segundos");
			delay(5000);
		}
	}
}

void setup_wifi(){
	delay(10);
	Serial.println();
	Serial.print("Conectando a red: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conexión WiFi OK");
	Serial.println("IP: ");
	Serial.println(WiFi.localIP());
}

void callback(char* t, byte* payload, unsigned int length){
	String incoming = "";
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
  msg = incoming;
  topic = t;
}
