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

/** \file SerialConnection.h
    \brief This file defines the SerialConnection class, which is an interface
           for a serial connection
  */

#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include "exceptions/IOException.h"

#include <string>

#include <stdint.h>

/** The enum SerialParity represents the different kinds of serial parity.
    \brief Serial parity
  */
enum SerialParity {
  /// No parity
  none = 0,
  /// Odd parity
  odd = 1,
  /// Even parity
  even = 2
};

/** The class SerialConnection is an interface for serial communication.
    \brief Serial communication interface
  */
class SerialConnection {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  SerialConnection(const SerialConnection &other);
  /// Assignment operator
  SerialConnection& operator = (const SerialConnection &other);
  /** @}
    */

  /** \name Private members
    @{
    */
  /// Path to the serial device
  std::string mDevicePathStr;
  // Baudrate of the device
  size_t mBaudrate;
  /// Data bits size
  size_t mDatabits;
  /// Stop bits number
  size_t mStopbits;
  /// Parity of the device
  SerialParity mSerialParity;
  /// Timeout of the device
  double mTimeout;
  /// Handle on the device
  ssize_t mHandle;
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs the serial connection from parameters
  SerialConnection(const std::string& devicePathStr = "/dev/ttyUSB0",
    size_t baudrate = 9600, size_t databits = 8, size_t stopbits = 1,
    SerialParity parity = none, double timeout = 2.5);
   /// Destructor
  ~SerialConnection();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the path to the device
  const std::string& getDevicePathStr() const;
  /// Returns the baudrate
  size_t getBaudrate() const;
  /// Returns the databits
  size_t getDatabits() const;
  /// Returns the stopbits
  size_t getStopbits() const;
  /// Returns the parity
  SerialParity getParity() const;
  /// Returns the timeout
  double getTimeout() const;
  /// Sets the timeout
  void setTimeout(double time);
  /// Set the baudrate
  void setBaudrate(size_t baudrate);
  /// Set the databits
  void setDatabits(size_t databits);
  /// Set the stopbits
  void setStopbits(size_t stopbits);
  /// Set the parity
  void setParity(SerialParity parity);
  /** @}
    */

  /** \name Methods
    @{
    */
  void open() throw (IOException);
  void close() throw (IOException);
  bool isOpen() const;
  void readBuffer(uint8_t* au8Buffer, size_t nbBytes) throw (IOException);
  void writeBuffer(const uint8_t* au8Buffer, size_t nbBytes)
    throw (IOException);
  /** @}
    */

protected:

};

#endif // SERIALCONNECTION_H
