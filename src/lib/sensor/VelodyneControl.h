#ifndef VELODYNECONTROL_H
#define VELODYNECONTROL_H

#include "SerialConnection.h"
#include "OutOfBoundException.h"

#include <string>

#include <stdint.h>

class VelodyneControl {
  VelodyneControl(const VelodyneControl &other);
  VelodyneControl& operator = (const VelodyneControl &other);

  static const uint32_t mcu32MaxRPM = 900;
  static const uint32_t mcu32MinRPM = 300;

  std::string mCommandString;

  SerialConnection mSerialConnection;

public:
  VelodyneControl();
  ~VelodyneControl();

  void setRPM(uint32_t u32RPM) throw(OutOfBoundException);

protected:

};

#endif // VELODYNECONTROL_H
