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

#include "com/SerialConnection.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

SerialConnection::SerialConnection(const std::string& devicePathStr,
  size_t baudrate, size_t databits, size_t stopbits, SerialParity parity,
  double timeout) :
  mDevicePathStr(devicePathStr),
  mBaudrate(baudrate),
  mDatabits(databits),
  mStopbits(stopbits),
  mSerialParity(parity),
  mTimeout(timeout),
  mHandle(0) {
}

SerialConnection::~SerialConnection() {
  close();
}

SerialConnection::SerialConnection(const SerialConnection& other) {
}

SerialConnection& SerialConnection::operator = (const SerialConnection& other) {
  return *this;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

const std::string& SerialConnection::getDevicePathStr() const {
  return mDevicePathStr;
}

void SerialConnection::setTimeout(double time) {
  mTimeout = time;
}

double SerialConnection::getTimeout() const {
  return mTimeout;
}

size_t SerialConnection::getBaudrate() const {
  return mBaudrate;
}

size_t SerialConnection::getDatabits() const {
  return mDatabits;
}

size_t SerialConnection::getStopbits() const {
  return mStopbits;
}

SerialParity SerialConnection::getParity() const {
  return mSerialParity;
}

void SerialConnection::setBaudrate(size_t baudrate) {
  mBaudrate = baudrate;
  close();
  open();
}

void SerialConnection::setDatabits(size_t databits) {
  mDatabits = databits;
  close();
  open();
}

void SerialConnection::setStopbits(size_t stopbits) {
  mStopbits = stopbits;
  close();
  open();
}

void SerialConnection::setParity(SerialParity parity) {
  mSerialParity = parity;
  close();
  open();
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void SerialConnection::open() throw (IOException) {
  mHandle = ::open(mDevicePathStr.c_str(), O_RDWR | O_NDELAY);
  if (mHandle == -1)
    throw IOException("SerialConnection::open(): Port opening failed");
  struct termios tios;
  memset(&tios, 0, sizeof(struct termios));
  switch (mBaudrate) {
    case 50L    : tios.c_cflag |= B50;
                  break;
    case 75L    : tios.c_cflag |= B75;
                  break;
    case 110L   : tios.c_cflag |= B110;
                  break;
    case 134L   : tios.c_cflag |= B134;
                  break;
    case 150L   : tios.c_cflag |= B150;
                  break;
    case 200L   : tios.c_cflag |= B200;
                  break;
    case 300L   : tios.c_cflag |= B300;
                  break;
    case 600L   : tios.c_cflag |= B600;
                  break;
    case 1200L  : tios.c_cflag |= B1200;
                  break;
    case 1800L  : tios.c_cflag |= B1800;
                  break;
    case 2400L  : tios.c_cflag |= B2400;
                  break;
    case 4800L  : tios.c_cflag |= B4800;
                  break;
    case 9600L  : tios.c_cflag |= B9600;
                  break;
    case 19200L : tios.c_cflag |= B19200;
                  break;
    case 38400L : tios.c_cflag |= B38400;
                  break;
    case 57600L : tios.c_cflag |= B57600;
                  break;
    case 115200L: tios.c_cflag |= B115200;
                  break;
    case 230400L: tios.c_cflag |= B230400;
                  break;
    default     :
      throw IOException("SerialConnection::open(): Invalid baudrate");
  }
  switch (mDatabits) {
    case 5 : tios.c_cflag |= CS5;
             break;
    case 6 : tios.c_cflag |= CS6;
             break;
    case 7 : tios.c_cflag |= CS7;
             break;
    case 8 : tios.c_cflag |= CS8;
             break;
    default:
      throw IOException("SerialConnection::open(): Invalid databits");
  }
  switch (mStopbits) {
    case 1 : break;
    case 2 : tios.c_cflag |= CSTOPB;
             break;
    default:
      throw IOException("SerialConnection::open(): Invalid stopbits");
  }
  switch (mSerialParity) {
    case none: break;
    case even: tios.c_cflag |= PARENB;
               break;
    case odd : tios.c_cflag |= PARENB | PARODD;
               break;
    default  :
      throw IOException("SerialConnection::open(): Invalid parity");
  }
  tios.c_cflag |= CLOCAL;
  tios.c_iflag = IGNPAR;
  if (tcflush(mHandle, TCIOFLUSH))
    throw IOException("SerialConnection::open(): flush failed");
  if (tcsetattr(mHandle, TCSANOW, &tios))
    throw IOException("SerialConnection::open(): setting attributes failed");
}

void SerialConnection::close() throw (IOException) {
  if (mHandle != 0) {
    if (tcdrain(mHandle))
      throw IOException("SerialConnection::close(): Port draining failed");
    if (tcflush(mHandle, TCIOFLUSH))
      throw IOException("SerialConnection::close(): Port flusing failed");
    if (::close(mHandle))
      throw IOException("SerialConnection::close(): Port closing failed");
    mHandle = 0;
  }
}

bool SerialConnection::isOpen() const {
  return (mHandle != 0);
}

void SerialConnection::readBuffer(uint8_t* au8Buffer, size_t nbBytes)
  throw (IOException) {
  if (isOpen() == false)
    open();
  double intPart;
  double fractPart = modf(mTimeout , &intPart);
  size_t numRead = 0;
  while (numRead < nbBytes) {
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fractPart * 1000000;
    fd_set readFlags;
    FD_ZERO(&readFlags);
    FD_SET(mHandle, &readFlags);
    ssize_t res = select(mHandle + 1, &readFlags, (fd_set*)0, (fd_set*)0,
      &waitd);
    if(res < 0)
      throw IOException("SerialConnection::readBuffer(): Read select failed");
    if (FD_ISSET(mHandle, &readFlags)) {
      FD_CLR(mHandle, &readFlags);
      res = read(mHandle, &au8Buffer[numRead], nbBytes - numRead);
      if ((res < 0) && (errno != EWOULDBLOCK))
        throw IOException("SerialConnection::readBuffer(): Read failed");
      if (res > 0) {
        numRead += res;
      }
    }
    else
      throw IOException("SerialConnection::readBuffer(): Read timeout");
  }
}

void SerialConnection::writeBuffer(const uint8_t* au8Buffer, size_t nbBytes)
  throw (IOException) {
  if (isOpen() == false)
    open();
  double intPart;
  double fractPart = modf(mTimeout , &intPart);
  size_t numWrite = 0;
  while (numWrite < nbBytes) {
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fractPart * 1000000;
    fd_set writeFlags;
    FD_ZERO(&writeFlags);
    FD_SET(mHandle, &writeFlags);
    ssize_t res = select(mHandle + 1, (fd_set*)0, &writeFlags,
      (fd_set*)0, &waitd);
    if(res < 0)
      throw IOException("SerialConnection::writeBuffer(): Write select failed");
    if (FD_ISSET(mHandle, &writeFlags)) {
      FD_CLR(mHandle, &writeFlags);
      res = write(mHandle, &au8Buffer[numWrite], nbBytes - numWrite);
      if ((res < 0) && (errno != EWOULDBLOCK))
        throw IOException("SerialConnection::writeBuffer(): Write failed");
      if (res > 0) {
        numWrite += res;
      }
    }
    else
      throw IOException("SerialConnection::writeBuffer(): Write timeout");
  }
}
