#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "TemperatureController.h"

#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
TemperatureController tempControl(DHTPIN);

//*********** MQTT Y WIFI CONFIG *******

const char* ssid = "TP-LINK_914E";
const char* password = "fender1243";
const char* mqtt_server = "mqtt.dioty.co";
const int mqtt_port = 1883;
const char* mqtt_user = "maximilianocalderon87@gmail.com";
const char* mqtt_pass = "1c110101";

WiFiClient espClient;
PubSubClient client(espClient);
String msg;

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();
void readMsg();

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
  reconnect();
	}
  readMsg();
  client.loop();
  delay(1000);
}

//**************FUNCIONES**************

void readMsg(){
  if (msg.length()>0){
    Serial.println(msg);
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
        Serial.println("fallo Suscripción");
      }
		} else {
			Serial.print("falló :( con error -> ");
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

void callback(char* topic, byte* payload, unsigned int length){
	String incoming = "";
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
  msg = incoming;
}
