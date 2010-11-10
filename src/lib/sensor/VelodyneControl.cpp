#include "VelodyneControl.h"

#include <iostream>
#include <sstream>

using namespace std;

VelodyneControl::VelodyneControl() : mCommandString("#HDLRPMnnn$") {
}

VelodyneControl::VelodyneControl(const VelodyneControl &other) {
}

VelodyneControl& VelodyneControl::operator = (const VelodyneControl &other) {
  return *this;
}

VelodyneControl::~VelodyneControl() {
}

void VelodyneControl::setRPM(uint32_t u32RPM) throw(OutOfBoundException,
  IOException) {
  if (u32RPM > mcu32MaxRPM || u32RPM < mcu32MinRPM)
    throw OutOfBoundException("VelodyneControl::setRPM: RPM should be in [300,900]");
  stringstream ss(stringstream::in | stringstream::out);
  ss << u32RPM;
  mCommandString.replace(7, 3, ss.str());
  mSerialConnection.open();
  mSerialConnection.writeBuffer((const uint8_t*)mCommandString.c_str(),
    mCommandString.size());
  mSerialConnection.close();
}
