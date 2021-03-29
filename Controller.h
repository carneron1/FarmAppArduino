#include <string>
using namespace std;

class Controller{
  protected:
  uint8_t sensorPinNumber;
  int relePinNumber;
  float targetValue;
  bool releState;
  bool manualMode;

  public:
  void setSensorPinNumber(int pinNumber);
  int getSensorPinNumber();
  bool getReleState();
  void setReleState(bool state);
  bool getManualMode();
  void setManualMode(bool state);
  float getTargetValue();
  void setTargetValue(float value);
  void publishTopic(string msg, string topic);
};

  void Controller::setSensorPinNumber(int pinNumber){
    sensorPinNumber = pinNumber;
  };

  int Controller::getSensorPinNumber(){
    return sensorPinNumber;
  };

  bool Controller::getReleState(){
    return releState;
  };
  void Controller::setReleState(bool state){
    releState = state;
  };
  bool Controller::getManualMode(){
    return manualMode;
  };
  void Controller::setManualMode(bool state){
    manualMode = state;
  };
  float Controller::getTargetValue(){
    return targetValue;
  };
  void Controller::setTargetValue(float value){
    targetValue = value;
  };
  void Controller::publishTopic(string msg, string topic){
    
  };