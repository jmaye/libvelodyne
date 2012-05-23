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

#include "exceptions/BadArgumentException.h"
#include "exceptions/SystemException.h"
#include "exceptions/IOException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

SerialConnection::SerialConnection(const std::string& devicePathStr, size_t
    baudrate, size_t databits, size_t stopbits, SerialParity parity, FlowControl
    flowControl, double timeout) :
    mDevicePathStr(devicePathStr),
    mBaudrate(baudrate),
    mDatabits(databits),
    mStopbits(stopbits),
    mSerialParity(parity),
    mFlowControl(flowControl),
    mTimeout(timeout),
    mHandle(0) {
}

SerialConnection::~SerialConnection() {
  close();
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void SerialConnection::read(std::istream& stream) {
}

void SerialConnection::write(std::ostream& stream) const {
  stream << "device path: " << mDevicePathStr << std::endl
    << "baudrate: " << mBaudrate << std::endl
    << "data bits: " << mDatabits << std::endl
    << "stop bit: " << mStopbits << std::endl
    << "parity: " << mSerialParity << std::endl
    << "flow control: " << mFlowControl << std::endl
    << "timeout: " << mTimeout;
}

void SerialConnection::read(std::ifstream& stream) {
}

void SerialConnection::write(std::ofstream& stream) const {
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

SerialConnection::SerialParity SerialConnection::getParity() const {
  return mSerialParity;
}

SerialConnection::FlowControl SerialConnection::getFlowControl() const {
  return mFlowControl;
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

void SerialConnection::setFlowControl(FlowControl flowControl) {
  mFlowControl = flowControl;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void SerialConnection::open() {
  if (isOpen())
    return;
  mHandle = ::open(mDevicePathStr.c_str(), O_RDWR | O_NDELAY | O_NOCTTY);
  if (mHandle == -1)
    throw SystemException(errno, "SerialConnection::open()::open()");
  struct termios tios;
  memset(&tios, 0, sizeof(struct termios));
  speed_t speed;
  switch (mBaudrate) {
    case 0L     : speed = B0;
                  break;
    case 50L    : speed = B50;
                  break;
    case 75L    : speed = B75;
                  break;
    case 110L   : speed = B110;
                  break;
    case 134L   : speed = B134;
                  break;
    case 150L   : speed = B150;
                  break;
    case 200L   : speed = B200;
                  break;
    case 300L   : speed = B300;
                  break;
    case 600L   : speed = B600;
                  break;
    case 1200L  : speed = B1200;
                  break;
    case 1800L  : speed = B1800;
                  break;
    case 2400L  : speed = B2400;
                  break;
    case 4800L  : speed = B4800;
                  break;
    case 9600L  : speed = B9600;
                  break;
    case 19200L : speed = B19200;
                  break;
    case 38400L : speed = B38400;
                  break;
    case 57600L : speed = B57600;
                  break;
    case 115200L: speed = B115200;
                  break;
    case 230400L: speed = B230400;
                  break;
    default     :
      throw BadArgumentException<size_t>(mBaudrate,
        "SerialConnection::open(): invalid baudrate");
  }
  if (cfsetspeed(&tios, speed))
    throw SystemException(errno, "SerialConnection::open()");
  tios.c_cflag &= ~CSIZE;
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
      throw BadArgumentException<size_t>(mDatabits,
        "SerialConnection::open(): invalid databits");
  }
  tios.c_cflag &= ~CSTOPB;
  switch (mStopbits) {
    case 1 : break;
    case 2 : tios.c_cflag |= CSTOPB;
             break;
    default:
      throw BadArgumentException<size_t>(mStopbits,
        "SerialConnection::open(): invalid stopbits");
  }
  tios.c_cflag &= ~PARENB;
  tios.c_cflag &= ~PARODD;
  switch (mSerialParity) {
    case none: break;
    case even: tios.c_cflag |= PARENB;
               break;
    case odd : tios.c_cflag |= PARENB | PARODD;
               break;
    default  :
      throw BadArgumentException<size_t>(mSerialParity,
        "SerialConnection::open(): invalid parity");
  }
  tios.c_cflag &= ~CRTSCTS;
  if (mFlowControl == hardware)
    tios.c_cflag |= CRTSCTS;
  tios.c_cflag |= (CLOCAL | CREAD);
  tios.c_iflag |= (INPCK | ISTRIP);
  tios.c_iflag &= ~(IXON | IXOFF | IXANY);
  if (mFlowControl == software)
    tios.c_iflag |= (IXON | IXOFF | IXANY);
  tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  tios.c_oflag &= ~OPOST;
  if (tcflush(mHandle, TCIOFLUSH))
    throw SystemException(errno,
      "SerialConnection::open()::tcflush()");
  if (tcsetattr(mHandle, TCSANOW, &tios))
    throw SystemException(errno,
      "SerialConnection::open()::tcsetattr()");
}

void SerialConnection::close() {
  if (mHandle != 0) {
    if (tcdrain(mHandle))
      throw SystemException(errno, "SerialConnection::close()::tcdrain()");
    if (tcflush(mHandle, TCIOFLUSH))
      throw SystemException(errno, "SerialConnection::close()::tcflush()");
    if (::close(mHandle))
      throw SystemException(errno, "SerialConnection::close()::close()");
    mHandle = 0;
  }
}

bool SerialConnection::isOpen() const {
  return (mHandle != 0);
}

void SerialConnection::read(char* buffer, size_t numBytes) {
  if (!isOpen())
    open();
  double intPart;
  double fractPart = modf(mTimeout , &intPart);
  size_t bytesRead = 0;
  while (bytesRead < numBytes) {
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fractPart * 1e6;
    fd_set readFlags;
    FD_ZERO(&readFlags);
    FD_SET(mHandle, &readFlags);
    ssize_t res = select(mHandle + 1, &readFlags, (fd_set*)0, (fd_set*)0,
      &waitd);
    if(res < 0)
      throw SystemException(errno, "SerialConnection::read()::select()");
    if (FD_ISSET(mHandle, &readFlags)) {
      FD_CLR(mHandle, &readFlags);
      res = ::read(mHandle, &buffer[bytesRead], numBytes - bytesRead);
      if (res < 0)
        throw SystemException(errno, "SerialConnection::read()::read()");
      bytesRead += res;
    }
    else
      throw IOException("SerialConnection::read(): timeout occured");
  }
}

void SerialConnection::write(const char* buffer, size_t numBytes) {
  if (!isOpen())
    open();
  double intPart;
  double fractPart = modf(mTimeout , &intPart);
  size_t bytesWritten = 0;
  while (bytesWritten < numBytes) {
    struct timeval waitd;
    waitd.tv_sec = intPart;
    waitd.tv_usec = fractPart * 1e6;
    fd_set writeFlags;
    FD_ZERO(&writeFlags);
    FD_SET(mHandle, &writeFlags);
    ssize_t res = select(mHandle + 1, (fd_set*)0, &writeFlags, (fd_set*)0,
      &waitd);
    if(res < 0)
      throw SystemException(errno, "SerialConnection::write()::select()");
    if (FD_ISSET(mHandle, &writeFlags)) {
      FD_CLR(mHandle, &writeFlags);
      res = ::write(mHandle, &buffer[bytesWritten], numBytes - bytesWritten);
      if (res < 0)
        throw SystemException(errno,
          "SerialConnection::write()::write()");
      bytesWritten += res;
    }
    else
      throw IOException("SerialConnection::write(): timeout occured");
  }
}
