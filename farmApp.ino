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

//************Test MQTT*******************

const char *root_topic_subscribe = "/maximilianocalderon87@gmail.com/subs";
const char *root_topic_publish = "/maximilianocalderon87@gmail.com/subs";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];


void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();



void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
}

void loop() {

  Serial.println(tempControl.getTemp(dht));
  delay(500);
    if (!client.connected()) {
		reconnect();
	}

  if (client.connected()){
    String str = "Mensaje enviado";
    str.toCharArray(msg,50);
    //client.publish(root_topic_publish,msg);

  }
  client.loop();
}

//**************FUNCIONES**************

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


void reconnect() {

	while (!client.connected()) {
		Serial.print("Intentando conexión Mqtt...");
		String clientId = "FARMAPP-";
		clientId += String(random(0xffff), HEX);
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Conectado");
				if(client.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Reintento en 5 segundos");
			delay(5000);
		}
	}
}


void callback(char* topic, byte* payload, unsigned int length){
	String incoming = "";
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Mensaje -> " + incoming);

}
