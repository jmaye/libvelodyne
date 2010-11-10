#include "UDPConnection.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

using namespace std;

UDPConnection::UDPConnection(uint16_t u16Port, double f64Timeout)
 : mu16Port(u16Port),
   mf64Timeout(f64Timeout),
   mi32Socket(0) {
}

UDPConnection::~UDPConnection() {
  close();
}

UDPConnection::UDPConnection(const UDPConnection &other) {
}

UDPConnection& UDPConnection::operator = (const UDPConnection &other) {
  return *this;
}

uint16_t UDPConnection::getPort() const {
  return mu16Port;
}

void UDPConnection::setTimeout(double f64Time) {
  mf64Timeout = f64Time;
}

double UDPConnection::getTimeout() const {
  return mf64Timeout;
}

void UDPConnection::open() throw(IOException) {
  mi32Socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (mi32Socket == -1)
    throw IOException("UDPConnection::open: Socket creation failed");

  struct sockaddr_in server;
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(mu16Port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(mi32Socket, (const sockaddr*)&server, sizeof(struct sockaddr)) == -1)
    throw IOException("UDPConnection::open: Socket binding failed");
}

void UDPConnection::close() throw(IOException) {
  if (mi32Socket != 0) {
    int32_t i32Res = ::close(mi32Socket);
    if (i32Res == -1)
      throw IOException("UDPConnection::close: Socket closing failed");
  }
  mi32Socket = 0;
}

bool UDPConnection::isOpen() const {
  return (mi32Socket != 0);
}

void UDPConnection::readBuffer(uint8_t *au8Buffer, uint32_t u32NbBytes)
  throw(IOException) {
  if (isOpen() == false)
    open();
  double f64IntPart;
  double f64FractPart = modf(mf64Timeout , &f64IntPart);
  struct timeval waitd;
  waitd.tv_sec = f64IntPart;
  waitd.tv_usec = f64FractPart * 1000000;
  fd_set readFlags;
  FD_ZERO(&readFlags);
  FD_SET(mi32Socket, &readFlags);
  int32_t i32Res = select(mi32Socket + 1, &readFlags, (fd_set*)0,
    (fd_set*)0, &waitd);
  if(i32Res < 0)
    throw IOException("UDPConnection::readBuffer: Read select failed");
  if (FD_ISSET(mi32Socket, &readFlags)) {
    FD_CLR(mi32Socket, &readFlags);
    struct sockaddr_in client;
    socklen_t size;
    if (recvfrom(mi32Socket, au8Buffer, u32NbBytes, 0,
      (sockaddr*)&client, &size) != u32NbBytes)
      throw IOException("UDPConnection::readBuffer: Read failed");
  }
  else
    throw IOException("UDPConnection::readBuffer: Read timeout");
}
