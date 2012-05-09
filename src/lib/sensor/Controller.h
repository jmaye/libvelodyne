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

/** \file Controller.h
    \brief This file defines the Controller class, which is the interface
           for controlling the Velodyne
  */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "com/SerialConnection.h"

/** The class Controller represents the interface for controlling the
    Velodyne.
    \brief Velodyne controller
  */
class Controller {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  Controller(const Controller& other);
  /// Assignment operator
  Controller& operator = (const Controller& other);
  /** @}
    */

public:
  /** \name Constants
    @{
    */
  /// Maximum spinning rate
  static const size_t mMaxRPM = 900;
  /// Minimum spinning rate
  static const size_t mMinRPM = 300;
  /** @}
    */

  /** \name Constructors/Destructor
    @{
    */
  /// Constructs object from serial port
  Controller(SerialConnection& serialConnection);
   /// Destructor
  ~Controller();
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Set the RPM
  void setRPM(size_t RPM);
  /** @}
    */

protected:
  /** \name Protected members
    @{
    */
  /// Command string
  std::string mCommandString;
  /// Serial connection to the Velodyne
  SerialConnection& mSerialConnection;
  /** @}
    */

};

#endif // CONTROLLER_H
