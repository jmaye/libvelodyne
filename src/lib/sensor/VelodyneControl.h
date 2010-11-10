#ifndef VELODYNECONTROL_H
#define VELODYNECONTROL_H

#include "SerialConnection.h"
#include "OutOfBoundException.h"
#include "IOException.h"

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
  VelodyneControl(std::string devicePathStr = "/dev/ttyUSB0",
    uint32_t u32Baudrate = 9600, uint32_t u32Databits = 8,
    uint32_t u32Stopbits = 1, SerialParity parity = none,
    double f64Timeout = 2.5);
  ~VelodyneControl();

  void setRPM(uint32_t u32RPM) throw(OutOfBoundException, IOException);

protected:

};

#endif // VELODYNECONTROL_H
