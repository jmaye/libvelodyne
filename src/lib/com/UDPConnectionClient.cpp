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

#include "com/UDPConnectionClient.h"

#include <cstring>
#include <cmath>

#include "exceptions/IOException.h"
#include "exceptions/SystemException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

UDPConnectionClient::UDPConnectionClient(const std::string& serverIP, short
    port, double timeout):
    mServerIP(serverIP),
    mPort(port),
    mTimeout(timeout),
    mSocket(0) {
  memset(&mServer, 0, sizeof(mServer));
  mServer.sin_family = AF_INET;
  mServer.sin_port = htons(mPort);
  if (inet_aton(mServerIP.c_str(), &mServer.sin_addr) == 0)
    throw SystemException(errno,
      "UDPConnectionClient::UDPConnectionClient()::inet_aton()");
}

UDPConnectionClient::~UDPConnectionClient() {
  close();
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void UDPConnectionClient::read(std::istream& stream) {
}

void UDPConnectionClient::write(std::ostream& stream) const {
  stream << "server IP: " << mServerIP << std::endl
    << "port: " << mPort << std::endl
    << "timeout: " << mTimeout;
}

void UDPConnectionClient::read(std::ifstream& stream) {
}

void UDPConnectionClient::write(std::ofstream& stream) const {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

short UDPConnectionClient::getPort() const {
  return mPort;
}

const std::string& UDPConnectionClient::getServerIP() const {
  return mServerIP;
}

void UDPConnectionClient::setTimeout(double timeout) {
  mTimeout = timeout;
}

double UDPConnectionClient::getTimeout() const {
  return mTimeout;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void UDPConnectionClient::open() {
  mSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (mSocket < 0)
    throw SystemException(errno,
      "UDPConnectionClient::open()::open()");
}

void UDPConnectionClient::close() {
  if (mSocket != 0) {
    ssize_t res = ::close(mSocket);
    if (res < 0)
      throw SystemException(errno,
        "UDPConnectionClient::close()::close()");
  }
  mSocket = 0;
}

bool UDPConnectionClient::isOpen() const {
  return (mSocket != 0);
}

void UDPConnectionClient::readBuffer(char* buffer, ssize_t numBytes) {
  if (isOpen() == false)
    open();
  ssize_t bytesRead = 0;
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
        "UDPConnectionClient::readBuffer()::select()");
    if (FD_ISSET(mSocket, &readFlags)) {
      FD_CLR(mSocket, &readFlags);
      struct sockaddr_in server;
      socklen_t size;
      res = recvfrom(mSocket, &buffer[bytesRead], numBytes - bytesRead, 0,
        (struct sockaddr*)&server, &size);
      if (res < 0)
        throw SystemException(errno,
          "UDPConnectionClient::readBuffer()::read()");
      bytesRead += res;
    }
    else
      throw IOException("UDPConnectionClient::readBuffer(): timeout occured");
  }
}

void UDPConnectionClient::writeBuffer(const char* buffer, ssize_t numBytes) {
  if (isOpen() == false)
    open();
  ssize_t bytesWritten = 0;
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
        "UDPConnectionClient::writeBuffer()::select()");
    if (FD_ISSET(mSocket, &writeFlags)) {
      FD_CLR(mSocket, &writeFlags);
      res = sendto(mSocket, &buffer[bytesWritten], numBytes - bytesWritten, 0,
        (const struct sockaddr*)&mServer, sizeof(struct sockaddr_in));
      if (res < 0)
        throw SystemException(errno,
          "UDPConnectionClient::writeBuffer()::write()");
      bytesWritten += res;
    }
    else
      throw IOException("UDPConnectionClient::writeBuffer(): timeout occured");
  }
}
