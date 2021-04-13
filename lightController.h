#include <Arduino.h>
#include <string>

class LightController {

  private:
  uint8_t sensorPinNumber;
  uint8_t ledPinNumber;
  bool ledState;

  public:
  LightController(uint8_t pinN, uint8_t ledPin);
  void checkStatus();
  char* getLedState();
  void setLedState(bool state);
};

LightController::LightController(uint8_t pinN, uint8_t ledPin){
    sensorPinNumber = pinN;
    ledPinNumber = ledPin;
    ledState = false;
};

void LightController::checkStatus(){
  if(digitalRead(sensorPinNumber)==HIGH){
    digitalWrite(ledPinNumber, LOW);
    ledState = false;
  }else {
    digitalWrite(ledPinNumber, HIGH);
    ledState = true;
    }
};

char* LightController::getLedState(){
  if (ledState) return "on";
  else return "off";
};

void LightController::setLedState(bool state){
  ledState = state;
  if (state==true){
    digitalWrite(ledPinNumber, HIGH);
  }else{
    digitalWrite(ledPinNumber, LOW);
  }
};
