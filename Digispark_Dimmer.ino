#include "GyverTimer.h"

#define POTENTIOMETER_PIN 3
#define MOSFET_PIN 1

#define READ_POTENTIOMETER_TIMES 10
#define COLD_START_INTERVAL 3

int _lastPotentiometerValue = -1;
int _mosfetValue = 0;

bool _isColdStart = true;
int _coldStartMaxMosfetValue = 0;

GTimer _updateInterval(MS, 10);

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
}

void loop() {
  
  if (_updateInterval.isReady()) {

    readPotentiometer();
    
    if (_isColdStart == true) {
      _coldStartMaxMosfetValue = calculateMosfetValue();

      _mosfetValue += COLD_START_INTERVAL;

      if (_mosfetValue >= _coldStartMaxMosfetValue) {
        _isColdStart = false;
      }
    }
    else {
      _mosfetValue = calculateMosfetValue();
    }

    analogWrite(MOSFET_PIN, _mosfetValue);
  }
}

int calculateMosfetValue() {
  return map(_lastPotentiometerValue, 0, 1024, 0, 255);
}

void readPotentiometer() {
  int sum = 0;
  for (int counter = 0; counter < READ_POTENTIOMETER_TIMES; counter++) {
    sum += analogRead(POTENTIOMETER_PIN);
    delay(1);
  }
  _lastPotentiometerValue = sum / READ_POTENTIOMETER_TIMES;
}
