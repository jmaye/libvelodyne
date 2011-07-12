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

/** \file UDPConnection.h
    \brief This file defines the UDPConnection class, which is an interface
           for a UDP connection
  */

#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H

#include "exceptions/IOException.h"

#include <stdint.h>

/** The class UDPConnection is an interface for UDP communication.
    \brief UDP communication interface
  */
class UDPConnection {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  UDPConnection(const UDPConnection& other);
  /// Assignment operator
  UDPConnection& operator = (const UDPConnection& other);
  /** @}
    */

  /** \name Private members
    @{
    */
  /// UDP port
  uint16_t mPort;
  /// Timeout of the port
  double mTimeout;
  /// Socket for the port
  ssize_t mSocket;
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs UDP from parameters
  UDPConnection(uint16_t u16Port = 2368, double timeout = 2.5);
  /// Destructor
  ~UDPConnection();
 /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the timeout
  double getTimeout() const;
  /// Returns the port number
  uint16_t getPort() const;
  /// Sets the timeout
  void setTimeout(double f64Time);
 /** @}
    */

  /** \name Methods
    @{
    */
  /// Open the connection
  void open() throw (IOException);
  /// Close the connection
  void close() throw (IOException);
  /// Test if the connection is open
  bool isOpen() const;
  /// Read buffer from UDP
  void readBuffer(uint8_t* au8Buffer, ssize_t nbBytes) throw (IOException);
 /** @}
    */

protected:

};

#endif // UDPCONNECTION_H
