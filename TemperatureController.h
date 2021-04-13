
#include "DHT.h"
#include <string>


using namespace std;

class TemperatureController { //dht 11/ dht 22

  private:
  uint8_t sensorPinNumber;
  public:
  TemperatureController(int pinN);
  float getHum(DHT dht); //Humedad
  float getTemp(DHT dht); //Temperatura

};

TemperatureController::TemperatureController(int pinN){
    sensorPinNumber = pinN;
  };

float TemperatureController::getHum(DHT dht){
  return dht.readHumidity();
};

float TemperatureController::getTemp(DHT dht){
  return dht.readTemperature();
};



