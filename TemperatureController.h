#include <PubSubClient.h>
#include "Controller.h"
#include "DHT.h"
#include <string>

#define DHTTYPE DHT22
#define DHTPIN 2


using namespace std;

class TemperatureController : Controller { //dht 11/ dht 22

  private:

  public:
  TemperatureController(int pinN){
    sensorPinNumber = pinN;
  };

  float getHum(DHT dht); //Humedad
  float getTemp(DHT dht); //Temperatura
  void subscribeTopics(PubSubClient client);

};

float TemperatureController::getHum(DHT dht){
  return dht.readHumidity();
};

float TemperatureController::getTemp(DHT dht){
  return dht.readTemperature();
};
void TemperatureController::subscribeTopics(PubSubClient client){


};



