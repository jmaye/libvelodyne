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

/** \file UDPConnectionClient.h
    \brief This file defines the UDPConnectionClient class, which is an
           interface for a client UDP connection
  */

#ifndef UDPCONNECTIONCLIENT_H
#define UDPCONNECTIONCLIENT_H

#include <arpa/inet.h>

#include <string>

#include "base/Serializable.h"

/** The class UDPConnectionClient is an interface for a client UDP
    communication.
    \brief Client UDP communication interface
  */
class UDPConnectionClient :
  public virtual Serializable {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  UDPConnectionClient(const UDPConnectionClient& other);
  /// Assignment operator
  UDPConnectionClient& operator = (const UDPConnectionClient& other);
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs UDP connection from parameters
  UDPConnectionClient(const std::string& serverIP, short port,
    double timeout = 2.5);
  /// Destructor
  virtual ~UDPConnectionClient();
 /** @}
    */

  /** \name Accessors
    @{
    */
  /// Sets the timeout of the connection
  void setTimeout(double timeout);
  /// Returns the timeout of the connection
  double getTimeout() const;
  /// Returns the port
  short getPort() const;
  /// Returns the server IP
  const std::string& getServerIP() const;
 /** @}
    */

  /** \name Methods
    @{
    */
  /// Open the connection
  void open() ;
  /// Close the connection
  void close();
  /// Test if the connection is open
  bool isOpen() const;
  /// Read buffer from UDP
  void readBuffer(char* buffer, ssize_t numBytes);
  /// Write buffer to UDP
  void writeBuffer(const char* buffer, ssize_t numBytes);
 /** @}
    */

protected:
  /** \name Stream methods
    @{
    */
  /// Reads from standard input
  virtual void read(std::istream& stream);
  /// Writes to standard output
  virtual void write(std::ostream& stream) const;
  /// Reads from a file
  virtual void read(std::ifstream& stream);
  /// Writes to a file
  virtual void write(std::ofstream& stream) const;
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Server IP
  std::string mServerIP;
  /// Server network structure
  struct sockaddr_in mServer;
  /// UDP port
  short mPort;
  /// Timeout of the port
  double mTimeout;
  /// Socket for the port
  ssize_t mSocket;
  /** @}
    */

};

#endif // UDPCONNECTIONCLIENT_H
