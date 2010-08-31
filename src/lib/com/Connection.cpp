#include "Connection.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <math.h>

using namespace std;

Connection::Connection(uint32_t u32Port, double f64Timeout) : mu32Port(),
                                            mf64Timeout(f64Timeout),
                                            mi32Socket(0),
                                            mu16PacketPos(0) {
}

Connection::~Connection() {
  close();
}

double Connection::getTimeout() const {
  return mf64Timeout;
}

uint32_t Connection::getPort() const {
  return mu32Port;
}

void Connection::setTimeout(double f64Time) {
  mf64Timeout = f64Time;
}

void Connection::open() throw(IOException) {
  mi32Socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (mi32Socket == -1)
    throw IOException("Connection::open: Socket creation failed");

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port   = htons(mu32Port);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(mi32Socket, (struct sockaddr*)&local, sizeof(local)) == -1)
    throw IOException("Connection::open: Socket binding failed");
}

void Connection::close() throw(IOException) {
  if (mi32Socket != 0) {
    int32_t i32Res = ::close(mi32Socket);
    if (i32Res == -1)
      throw IOException("Connection::close: Socket closing failed");
  }
  mi32Socket = 0;
}

bool Connection::isOpen() const {
  return (mi32Socket != 0);
}

const VelodynePacket& Connection::receivePacket() throw(IOException) {
  if(mi32Socket == 0)
    open();
  mu16PacketPos = 0;
  do {
    mu16PacketPos += readBytes();
  }
  while (mu16PacketPos != mcu16PacketSize);
  return *(VelodynePacket*)&mau8Packet[0];
}

int32_t Connection::readBytes() const throw(IOException) {
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
    throw IOException("Connection::readBytes: Read select failed");
  if (FD_ISSET(mi32Socket, &readFlags)) {
    FD_CLR(mi32Socket, &readFlags);
    uint32_t u32StructSize = sizeof(struct sockaddr_in);
    struct sockaddr server;
    int32_t i32Ret = recvfrom(mi32Socket, (uint8_t*)&mau8Packet[mu16PacketPos],
      mcu16PacketSize - mu16PacketPos, 0, &server, &u32StructSize);
    if (i32Ret <= 0)
      throw IOException("Connection::readBytes: Read failed");
    else
      return i32Ret;
  }
  else
    throw IOException("Connection::readBytes: Read timeout");
}
