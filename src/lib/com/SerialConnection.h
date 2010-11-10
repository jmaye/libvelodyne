#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include "IOException.h"

#include <string>

#include <stdint.h>

enum SerialParity {
  none = 0,
  odd = 1,
  even = 2
};

class SerialConnection {
  SerialConnection(const SerialConnection &other);
  SerialConnection& operator = (const SerialConnection &other);

  std::string mDevicePathStr;
  uint32_t mu32Baudrate;
  uint32_t mu32Databits;
  uint32_t mu32Stopbits;
  SerialParity mSerialParity;
  double mf64Timeout;
  int32_t mi32Handle;

public:
  SerialConnection(std::string devicePathStr = "/dev/ttyUSB0",
    uint32_t u32Baudrate = 9600, uint32_t u32Databits = 8,
    uint32_t u32Stopbits = 1, SerialParity parity = none,
    double f64Timeout = 2.5);
  ~SerialConnection();

  std::string getDevicePathStr() const;
  uint32_t getBaudrate() const;
  uint32_t getDatabits() const;
  uint32_t getStopbits() const;
  SerialParity getParity() const;
  double getTimeout() const;
  void setTimeout(double f64Time);
  void setBaudrate(uint32_t u32Baudrate) throw(IOException);
  void setDatabits(uint32_t u32Databits) throw(IOException);
  void setStopbits(uint32_t u32Stopbits) throw(IOException);
  void setParity(SerialParity parity) throw(IOException);
  void open() throw(IOException);
  void close() throw(IOException);
  bool isOpen() const;
  void readBuffer(uint8_t *au8Buffer, uint32_t u32NbBytes) throw(IOException);
  void writeBuffer(const uint8_t *au8Buffer, uint32_t u32NbBytes)
    throw(IOException);

protected:

};

#endif // SERIALCONNECTION_H
