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

/** \file VelodyneControl.h
    \brief This file defines the VelodyneControl class, which is the interface
           for controlling the Velodyne
  */

#ifndef VELODYNECONTROL_H
#define VELODYNECONTROL_H

#include "com/SerialConnection.h"
#include "exceptions/OutOfBoundException.h"

#include <string>

/** The class VelodyneControl represents the interface for controlling the
    Velodyne.
    \brief Velodyne controller
  */
class VelodyneControl {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodyneControl(const VelodyneControl& other);
  /// Assignment operator
  VelodyneControl& operator = (const VelodyneControl& other);
  /** @}
    */

  /** \name Constants
    @{
    */
  /// Maximum spinning rate
  static const size_t mMaxRPM = 900;
  /// Minimum spinning rate
  static const size_t mMinRPM = 300;
  /** @}
    */

  /** \name Private members
    @{
    */
  /// Command string
  std::string mCommandString;
  /// Serial connection to the Velodyne
  SerialConnection mSerialConnection;
  /** @}
    */

public:
  /** \name Constructors/Destructor
    @{
    */
  /// Constructs object from parameters
  VelodyneControl(const std::string& devicePathStr = "/dev/ttyUSB0",
    size_t baudrate = 9600, size_t databits = 8,
    size_t stopbits = 1, SerialParity parity = none,
    double timeout = 2.5);
   /// Destructor
  ~VelodyneControl();
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Set the RPM
  void setRPM(size_t RPM) throw (OutOfBoundException);
  /** @}
    */

protected:

};

#endif // VELODYNECONTROL_H
