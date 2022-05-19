#include "PlanetXSegment.h"

PlanetXSegment::PlanetXSegment(RJPin pin)
{
  _clkPin = pins[pin][0];
  _dioPin = pins[pin][1];
  _display = new TM1637Display(_clkPin, _dioPin);
  memset(_data, 0, 4);
}

PlanetXSegment::PlanetXSegment()
{
  _display = new TM1637Display();
  memset(_data, 0, 4);
}

void PlanetXSegment::setPin(uint8_t clkPin, uint8_t dioPin)
{
  _clkPin = clkPin;
  _dioPin = dioPin;
  _display->setPin(_clkPin, _dioPin);
}

void PlanetXSegment::setBrightness(uint8_t brightness, bool on)
{
  _display->setBrightness(brightness, on);
}

void PlanetXSegment::setSegment(uint8_t pos, uint8_t seg, bool on)
{
  if (on) {
    _data[pos] |= seg;
    return;
  }
  _data[pos] &= (~seg);
}

void PlanetXSegment::setDigit(uint8_t pos, uint8_t digit)
{
  if (pos > 3) return;
  setSegmentData(pos, _display->encodeDigit(digit));
}

void PlanetXSegment::setDot(uint8_t pos, bool on)
{
  if (pos > 3) return;

  if (on) {
    _data[pos] |= 0x80;
    return;
  }

  _data[pos] &= 0x7f;
}

void PlanetXSegment::show()
{
  _display->setSegments(_data);
}

void PlanetXSegment::showInteger(int num, bool leadingZero)
{
  memset(_data, 0, 4);
  setInteger(num, leadingZero);
  show();
}

void PlanetXSegment::showNumber(float num, uint8_t decimalPlace, bool leadingZero)
{
  if (decimalPlace == 0 || decimalPlace > 3) {
    showInteger(round(num), leadingZero);
    return;
  }

  memset(_data, 0, 4);
  for (int i = 0; i < 3; i++) {
    setDot(i, i == 3 - decimalPlace);
  }
  if ((int)num == 0) {
    int32_t tempNum = round(num * round(pow(10, decimalPlace))) + (num < 0 ? -1 : 1) * round(pow(10, decimalPlace));
    if (setInteger(tempNum, leadingZero)) {
      setDigit(3 - decimalPlace, 0);
    }
  } else {
    int32_t tempNum = round(num * round(pow(10, decimalPlace)));
    setInteger(tempNum, leadingZero);
  }
  show();
}

void PlanetXSegment::clear()
{
  memset(_data, 0, 4);
  show();
}

bool PlanetXSegment::setInteger(int32_t num, bool leadingZero)
{
  setSegmentData(0, 0);
  setSegmentData(1, 0);
  setSegmentData(2, 0);
  setSegmentData(3, 0);
  if (leadingZero) {
    setDigit(0, 0);
    setDigit(1, 0);
    setDigit(2, 0);
    setDigit(3, 0);
  }
  if (num > 9999 || num < -999) { // Show Err.
    memset(_data, 0, 4);
    setSegmentData(1, SEG_A | SEG_D | SEG_E | SEG_F | SEG_G);
    setSegmentData(2, SEG_E | SEG_G);
    setSegmentData(3, SEG_E | SEG_G);
    return false;
  }

  // Set digit.
  setDigit(3, abs(num % 1000 % 100 % 10));
  if (num > 9) {
    setDigit(2, num % 1000 % 100 / 10);
  }
  if (num > 99) {
    setDigit(1, num % 1000 / 100);
  }
  if (num > 999) {
    setDigit(0, num / 1000);
  }

  // Set minus.
  if (num < 0) {
    setSegmentData(2, SEG_G);
  }
  if (num < -9) {
    setSegmentData(1, SEG_G);
    setDigit(2, -1 * num % 100 / 10);
  }
  if (num < -99) {
    setSegmentData(0, SEG_G);
    setDigit(1, -1 * num / 100);
  }

  return true;
}

void PlanetXSegment::setSegmentData(uint8_t pos, uint8_t data)
{
  _data[pos] = (_data[pos] & 0x80) | data;
}
