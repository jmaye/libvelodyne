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

#include "com/UDPConnectionServer.h"

#include <arpa/inet.h>
#include <cstring>
#include <cmath>

#include "exceptions/IOException.h"
#include "exceptions/SystemException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

UDPConnectionServer::UDPConnectionServer(short port, double timeout) :
    mPort(port),
    mTimeout(timeout),
    mSocket(0) {
}

UDPConnectionServer::~UDPConnectionServer() {
  close();
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void UDPConnectionServer::read(std::istream& stream) {
}

void UDPConnectionServer::write(std::ostream& stream) const {
  stream << "port: " << mPort << std::endl
    << "timeout: " << mTimeout;
}

void UDPConnectionServer::read(std::ifstream& stream) {
}

void UDPConnectionServer::write(std::ofstream& stream) const {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

short UDPConnectionServer::getPort() const {
  return mPort;
}

void UDPConnectionServer::setTimeout(double timeout) {
  mTimeout = timeout;
}

double UDPConnectionServer::getTimeout() const {
  return mTimeout;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void UDPConnectionServer::open() {
  if (isOpen())
    return;
  mSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (mSocket < 0) {
    mSocket = 0;
    throw SystemException(errno, "UDPConnectionServer::open()::socket()");
  }
  struct sockaddr_in server;
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(mPort);
  if (bind(mSocket, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) ==
      -1) {
    close();
    throw SystemException(errno, "UDPConnectionServer::open()::bind()");
  }
}

void UDPConnectionServer::close() {
  if (mSocket != 0) {
    ssize_t res = ::close(mSocket);
    if (res < 0)
      throw SystemException(errno, "UDPConnectionServer::close()::close()");
  }
  mSocket = 0;
}

bool UDPConnectionServer::isOpen() const {
  return (mSocket != 0);
}

size_t UDPConnectionServer::read(char* buffer, size_t numBytes) {
  if (!isOpen())
    open();
  double intPart;
  double fracPart = modf(mTimeout, &intPart);
  struct timeval waitd;
  waitd.tv_sec = intPart;
  waitd.tv_usec = fracPart * 1e6;
  fd_set readFlags;
  FD_ZERO(&readFlags);
  FD_SET(mSocket, &readFlags);
  ssize_t res = select(mSocket + 1, &readFlags, (fd_set*)0, (fd_set*)0, &waitd);
  if(res < 0)
    throw SystemException(errno, "UDPConnectionServer::read()::select()");
  if (FD_ISSET(mSocket, &readFlags)) {
    FD_CLR(mSocket, &readFlags);
    struct sockaddr_in client;
    socklen_t size;
    res = recvfrom(mSocket, buffer, numBytes, 0, (struct sockaddr*)&client,
      &size);
    if (res < 0)
        throw SystemException(errno,
          "UDPConnectionServer::read()::read()");
    return res;
  }
  else
    throw IOException("UDPConnectionServer::read(): timeout occured");
  return 0;
}

void UDPConnectionServer::write(const char* buffer, size_t numBytes) {
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
        "UDPConnectionServer::write()::select()");
    if (FD_ISSET(mSocket, &writeFlags)) {
      FD_CLR(mSocket, &writeFlags);
      //TODO: MODIFY THIS PART
      struct sockaddr_in client;
      res = sendto(mSocket, &buffer[bytesWritten], numBytes - bytesWritten, 0,
        (const struct sockaddr*)&client, sizeof(struct sockaddr_in));
      if (res < 0)
        throw SystemException(errno,
          "UDPConnectionServer::write()::write()");
      bytesWritten += res;
    }
    else
      throw IOException("UDPConnectionServer::write(): timeout occured");
  }
}
