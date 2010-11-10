#include "SerialConnection.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

using namespace std;

SerialConnection::SerialConnection(string devicePathStr, uint32_t u32Baudrate,
  uint32_t u32Databits, uint32_t u32Stopbits, SerialParity parity,
  double f64Timeout)
  : mDevicePathStr(devicePathStr),
    mu32Baudrate(u32Baudrate),
    mu32Databits(u32Databits),
    mu32Stopbits(u32Stopbits),
    mSerialParity(parity),
    mf64Timeout(f64Timeout),
    mi32Handle(0) {
}

SerialConnection::~SerialConnection() {
  close();
}

SerialConnection::SerialConnection(const SerialConnection &other) {
}

SerialConnection& SerialConnection::operator = (const SerialConnection &other) {
  return *this;
}

string SerialConnection::getDevicePathStr() const {
  return mDevicePathStr;
}

void SerialConnection::setTimeout(double f64Time) {
  mf64Timeout = f64Time;
}

double SerialConnection::getTimeout() const {
  return mf64Timeout;
}

uint32_t SerialConnection::getBaudrate() const {
  return mu32Baudrate;
}

uint32_t SerialConnection::getDatabits() const {
  return mu32Databits;
}

uint32_t SerialConnection::getStopbits() const {
  return mu32Stopbits;
}

SerialParity SerialConnection::getParity() const {
  return mSerialParity;
}

void SerialConnection::setBaudrate(uint32_t u32Baudrate) throw(IOException) {
  mu32Baudrate = u32Baudrate;
  close();
  open();
}

void SerialConnection::setDatabits(uint32_t u32Databits) throw(IOException) {
  mu32Databits = u32Databits;
  close();
  open();
}

void SerialConnection::setStopbits(uint32_t u32Stopbits) throw(IOException) {
  mu32Stopbits = u32Stopbits;
  close();
  open();
}

void SerialConnection::setParity(SerialParity parity) throw(IOException) {
  mSerialParity = parity;
  close();
  open();
}

void SerialConnection::open() throw(IOException) {
  mi32Handle = ::open(mDevicePathStr.c_str(), O_RDWR | O_NDELAY);
  if (mi32Handle == -1)
    throw IOException("SerialConnection::open: Port opening failed");
  struct termios tios;
  memset(&tios, 0, sizeof(struct termios));
  switch (mu32Baudrate) {
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
      throw IOException("SerialConnection::open: Invalid baudrate");
  }
  switch (mu32Databits) {
    case 5 : tios.c_cflag |= CS5;
             break;
    case 6 : tios.c_cflag |= CS6;
             break;
    case 7 : tios.c_cflag |= CS7;
             break;
    case 8 : tios.c_cflag |= CS8;
             break;
    default:
      throw IOException("SerialConnection::open: Invalid databits");
  }
  switch (mu32Stopbits) {
    case 1 : break;
    case 2 : tios.c_cflag |= CSTOPB;
             break;
    default:
      throw IOException("SerialConnection::open: Invalid stopbits");
  }
  switch (mSerialParity) {
    case none: break;
    case even: tios.c_cflag |= PARENB;
               break;
    case odd : tios.c_cflag |= PARENB | PARODD;
               break;
    default  :
      throw IOException("SerialConnection::open: Invalid parity");
  }
  tios.c_cflag |= CLOCAL;
  tios.c_iflag = IGNPAR;
  if (tcflush(mi32Handle, TCIOFLUSH))
    throw IOException("SerialConnection::open: flush failed");
  if (tcsetattr(mi32Handle, TCSANOW, &tios))
    throw IOException("SerialConnection::open: setting attributes failed");
}

void SerialConnection::close() throw(IOException) {
  if (mi32Handle != 0) {
    if (tcdrain(mi32Handle))
      throw IOException("SerialConnection::close: Port draining failed");
    if (tcflush(mi32Handle, TCIOFLUSH))
      throw IOException("SerialConnection::close: Port flusing failed");
    if (::close(mi32Handle))
      throw IOException("SerialConnection::close: Port closing failed");
    mi32Handle = 0;
  }
}

bool SerialConnection::isOpen() const {
  return (mi32Handle != 0);
}

void SerialConnection::readBuffer(uint8_t *au8Buffer, uint32_t u32NbBytes)
  throw(IOException) {
  if (isOpen() == false)
    open();
  double f64IntPart;
  double f64FractPart = modf(mf64Timeout , &f64IntPart);
  uint32_t u32NumRead = 0;
  while (u32NumRead < u32NbBytes) {
    struct timeval waitd;
    waitd.tv_sec = f64IntPart;
    waitd.tv_usec = f64FractPart * 1000000;
    fd_set readFlags;
    FD_ZERO(&readFlags);
    FD_SET(mi32Handle, &readFlags);
    int32_t i32Res = select(mi32Handle + 1, &readFlags, (fd_set*)0,
      (fd_set*)0, &waitd);
    if(i32Res < 0)
      throw IOException("SerialConnection::readBuffer: Read select failed");
    if (FD_ISSET(mi32Handle, &readFlags)) {
      FD_CLR(mi32Handle, &readFlags);
      i32Res = read(mi32Handle, &au8Buffer[u32NumRead], u32NbBytes -
        u32NumRead);
      if ((i32Res < 0) && (errno != EWOULDBLOCK))
        throw IOException("SerialConnection::readBuffer: Read failed");
      if (i32Res > 0) {
        u32NumRead += i32Res;
      }
    }
    else
      throw IOException("SerialConnection::readBuffer: Read timeout");
  }
}

void SerialConnection::writeBuffer(uint8_t *au8Buffer, uint32_t u32NbBytes)
  throw(IOException) {
  if (isOpen() == false)
    open();
  double f64IntPart;
  double f64FractPart = modf(mf64Timeout , &f64IntPart);
  uint32_t u32NumWrite = 0;
  while (u32NumWrite < u32NbBytes) {
    struct timeval waitd;
    waitd.tv_sec = f64IntPart;
    waitd.tv_usec = f64FractPart * 1000000;
    fd_set writeFlags;
    FD_ZERO(&writeFlags);
    FD_SET(mi32Handle, &writeFlags);
    int32_t i32Res = select(mi32Handle + 1, (fd_set*)0, &writeFlags,
      (fd_set*)0, &waitd);
    if(i32Res < 0)
      throw IOException("writeBuffer: Write select failed");
    if (FD_ISSET(mi32Handle, &writeFlags)) {
      FD_CLR(mi32Handle, &writeFlags);
      i32Res = write(mi32Handle, &au8Buffer[u32NumWrite], u32NbBytes -
        u32NumWrite);
      if ((i32Res < 0) && (errno != EWOULDBLOCK))
        throw IOException("SerialConnection::writeBuffer: Write failed");
      if (i32Res > 0) {
        u32NumWrite += i32Res;
      }
    }
    else
      throw IOException("SerialConnection::writeBuffer: Write timeout");
  }
}
