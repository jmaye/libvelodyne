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

#include "sensor/Controller.h"

#include <sstream>

#include "exceptions/OutOfBoundException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

Controller::Controller(SerialConnection& serialConnection) :
    mCommandString("#HDLRPMnnn$"),
    mSerialConnection(serialConnection) {
}

Controller::~Controller() {
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void Controller::setRPM(size_t RPM) {
  if (RPM > mMaxRPM || RPM < mMinRPM)
    throw OutOfBoundException<size_t>(RPM,
      "Controller::setRPM(): RPM should be in [300, 900]",
      __FILE__, __LINE__);
  std::stringstream ss(std::stringstream::in | std::stringstream::out);
  ss << RPM;
  mCommandString.replace(7, 3, ss.str());
  mSerialConnection.write(
    reinterpret_cast<const char*>(mCommandString.c_str()),
    mCommandString.size());
}
