#include <Arduino.h>
#include <string>

class HumidityController {

  private:
  uint8_t sensorPinNumber;
  uint8_t relePinNumber;
  float targetValue;
  bool releState;

  public:
  HumidityController(uint8_t pinN, uint8_t relePin);
  float getHum();
  void setTargetValue (float val);
  float getTargetValue();
  void checkStatus();
  char* getReleState();
};

HumidityController::HumidityController(uint8_t pinN, uint8_t relePin){
    sensorPinNumber = pinN;
    relePinNumber = relePin;
    targetValue = 90;
    releState = false;
  };

float HumidityController::getHum(){
  int val = (analogRead(sensorPinNumber));
  int res = 100-(val-220)*100/410;
  if(!releState) res = res + 25;
  if (res>100) res = 100;
  if (res<0) res = 0;
  return res;
};

void HumidityController::setTargetValue(float val){
  targetValue = val;
};

float HumidityController::getTargetValue(){
  return targetValue;
};

void HumidityController::checkStatus(){
  float status = getHum();
  Serial.println(status);
  if (releState==false&&(status<=(targetValue-10))){
    releState = true;
    digitalWrite(relePinNumber, HIGH);
  };
  if (releState==true&&(status>=(targetValue+10))){
    releState = false;
    digitalWrite(relePinNumber, LOW);
  };
};

char* HumidityController::getReleState(){
  if (releState) return "on";
  else return "off";
};
