#ifndef _PLANETXSEGMENT_H_
#define _PLANETXSEGMENT_H_

#include <TM1637Display.h>
#include "RJPins.h"

class PlanetXSegment
{
public:
  PlanetXSegment(RJPin pin);
  PlanetXSegment();
  void setPin(uint8_t clkPin, uint8_t dioPin);
  void setBrightness(uint8_t brightness, bool on = true);
  void setSegment(uint8_t pos, uint8_t seg, bool on = true);
  void setDigit(uint8_t pos, uint8_t digit);
  void setDot(uint8_t pos, bool on = true);
  void show();
  void showInteger(int num, bool leadingZero = false);
  void showNumber(float num, uint8_t decimalPlace = 0, bool leadingZero = false);
  void clear();

private:
  uint8_t _clkPin;
  uint8_t _dioPin;
  TM1637Display *_display;
  uint8_t _data[4];
  bool setInteger(int32_t num, bool leadingZero = false);
  void setSegmentData(uint8_t pos, uint8_t data);
};

#endif
