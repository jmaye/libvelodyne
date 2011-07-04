/******************************************************************************
 * Copyright (C) 2011 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include "sensor/VelodyneControl.h"

#include <iostream>
#include <sstream>

#include <stdint.h>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodyneControl::VelodyneControl(const std::string& devicePathStr,
  size_t baudrate, size_t databits, size_t stopbits, SerialParity parity,
  double timeout) :
  mCommandString("#HDLRPMnnn$"),
  mSerialConnection(devicePathStr, baudrate, databits, stopbits, parity,
    timeout) {
}

VelodyneControl::VelodyneControl(const VelodyneControl& other) {
}

VelodyneControl& VelodyneControl::operator = (const VelodyneControl& other) {
  return *this;
}

VelodyneControl::~VelodyneControl() {
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void VelodyneControl::setRPM(size_t RPM) throw (OutOfBoundException) {
  if (RPM > mMaxRPM || RPM < mMinRPM)
    throw OutOfBoundException("VelodyneControl::setRPM(): RPM should be in [300,900]");
  std::stringstream ss(std::stringstream::in | std::stringstream::out);
  ss << RPM;
  mCommandString.replace(7, 3, ss.str());
  mSerialConnection.open();
  mSerialConnection.writeBuffer((const uint8_t*)mCommandString.c_str(),
    mCommandString.size());
  mSerialConnection.close();
}
