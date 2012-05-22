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

#include "com/TCPConnectionClient.h"

#include <cstring>
#include <cmath>
#include <arpa/inet.h>

#include "exceptions/SystemException.h"
#include "exceptions/IOException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

TCPConnectionClient::TCPConnectionClient(const std::string& serverIP, short
    port, double timeout) :
    mServerIP(serverIP),
    mPort(port),
    mTimeout(timeout),
    mSocket(0) {
}

TCPConnectionClient::~TCPConnectionClient() {
  close();
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void TCPConnectionClient::read(std::istream& stream) {
}

void TCPConnectionClient::write(std::ostream& stream) const {
  stream << "server IP: " << mServerIP << std::endl
    << "port: " << mPort << std::endl
    << "timeout: " << mTimeout;
}

void TCPConnectionClient::read(std::ifstream& stream) {
}

void TCPConnectionClient::write(std::ofstream& stream) const {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

short TCPConnectionClient::getPort() const {
  return mPort;
}

const std::string& TCPConnectionClient::getServerIP() const {
  return mServerIP;
}

void TCPConnectionClient::setTimeout(double timeout) {
  mTimeout = timeout;
}

double TCPConnectionClient::getTimeout() const {
  return mTimeout;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void TCPConnectionClient::open() {
  mSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (mSocket < 0)
    throw SystemException(errno,
      "TCPConnectionClient::open()::socket()");
  struct sockaddr_in server;
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(mServerIP.c_str());
  server.sin_port = htons(mPort);
  if (connect(mSocket, (const struct sockaddr*)&server, sizeof(server)) == -1)
    throw SystemException(errno,
      "TCPConnectionClient::open()::connect()");
}

void TCPConnectionClient::close() {
  if (mSocket != 0) {
    ssize_t res = ::close(mSocket);
    if (res < 0)
      throw SystemException(errno, "TCPConnectionClient::close()::close()");
  }
  mSocket = 0;
}

bool TCPConnectionClient::isOpen() const {
  return (mSocket != 0);
}

void TCPConnectionClient::read(char* buffer, size_t numBytes) {
  if (!isOpen())
    open();
  size_t bytesRead = 0;
  while (bytesRead != numBytes) {
    double intPart;
    double fracPart = modf(mTimeout, &intPart);
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fracPart * 1e6;
    fd_set readFlags;
    FD_ZERO(&readFlags);
    FD_SET(mSocket, &readFlags);
    ssize_t res = select(mSocket + 1, &readFlags, (fd_set*)0, (fd_set*)0,
      &waitd);
    if(res < 0)
      throw SystemException(errno,
        "TCPConnectionClient::read()::select()");
    if (FD_ISSET(mSocket, &readFlags)) {
      FD_CLR(mSocket, &readFlags);
      res = ::read(mSocket, &buffer[bytesRead], numBytes - bytesRead);
      if (res < 0)
        throw SystemException(errno,
          "TCPConnectionClient::read()::read()");
      bytesRead += res;
    }
    else
      throw IOException("TCPConnectionClient::read(): timeout occured");
  }
}

void TCPConnectionClient::write(const char* buffer, size_t numBytes) {
  if (!isOpen())
    open();
  size_t bytesWritten = 0;
  while (bytesWritten != numBytes) {
    double intPart;
    double fracPart = modf(mTimeout, &intPart);
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fracPart * 1e6;
    fd_set writeFlags;
    FD_ZERO(&writeFlags);
    FD_SET(mSocket, &writeFlags);
    ssize_t res = select(mSocket + 1, (fd_set*)0, &writeFlags, (fd_set*)0,
      &waitd);
    if(res < 0)
      throw SystemException(errno,
        "TCPConnectionClient::write()::select()");
    if (FD_ISSET(mSocket, &writeFlags)) {
      FD_CLR(mSocket, &writeFlags);
      res = ::write(mSocket, &buffer[bytesWritten], numBytes - bytesWritten);
      if (res < 0)
        throw SystemException(errno,
          "TCPConnectionClient::write()::write()");
      bytesWritten += res;
    }
    else
      throw IOException("TCPConnectionClient::write(): timeout occured");
  }
}
