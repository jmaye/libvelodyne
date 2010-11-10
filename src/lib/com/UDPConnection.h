#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H

#include "IOException.h"

#include <stdint.h>

class UDPConnection {
  UDPConnection(const UDPConnection &other);
  UDPConnection& operator = (const UDPConnection &other);

  uint16_t mu16Port;
  double mf64Timeout;
  int32_t mi32Socket;

public:
  UDPConnection(uint16_t u16Port = 2368, double f64Timeout = 2.5);
  ~UDPConnection();

  double getTimeout() const;
  uint16_t getPort() const;
  void setTimeout(double f64Time);
  void open() throw(IOException);
  void close() throw(IOException);
  bool isOpen() const;
  void readBuffer(uint8_t *au8Buffer, uint32_t u32NbBytes) throw(IOException);

protected:

};

#endif // UDPCONNECTION_H
