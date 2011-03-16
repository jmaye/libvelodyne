#include "VelodynePacket.h"

#include "UDPConnection.h"

#include <iostream>
#include <fstream>

#include <sys/time.h>
#include <string.h>

using namespace std;

VelodynePacket::VelodynePacket() : mf64Timestamp(0),
                                   mu16SpinCount(0),
                                   mu32Reserved(0) {
  memset(mau8Packet, 0, mcu16PacketSize * sizeof(uint8_t));
}

VelodynePacket::VelodynePacket(const VelodynePacket &other)
  : mf64Timestamp(other.mf64Timestamp),
    mu16SpinCount(other.mu16SpinCount),
    mu32Reserved(other.mu32Reserved) {
  for (uint32_t i = 0; i < mcu16DataChunkNbr; i++) {
    mData[i] = other.mData[i];
  }
  memcpy(mau8Packet, other.mau8Packet, mcu16PacketSize * sizeof(uint8_t));
}

VelodynePacket& VelodynePacket::operator = (const VelodynePacket &other) {
  if (this != &other) {
    mf64Timestamp = other.mf64Timestamp;
    mu16SpinCount = other.mu16SpinCount;
    mu32Reserved = other.mu32Reserved;
    for (uint32_t i = 0; i < mcu16DataChunkNbr; i++) {
      mData[i] = other.mData[i];
    }
    memcpy(mau8Packet, other.mau8Packet, mcu16PacketSize * sizeof(uint8_t));
  }
  return *this;
}

VelodynePacket::~VelodynePacket() {
}

double VelodynePacket::seconds() const {
  struct timeval now;
  gettimeofday(&now, 0L);
  return (double)((double)now.tv_sec + ((double)now.tv_usec /
    (double)mcu32TimeResolution));
}

void VelodynePacket::read(UDPConnection &stream) throw(IOException) {
  stream.readBuffer(mau8Packet, mcu16PacketSize);
  mf64Timestamp = seconds();
  read(mau8Packet);
}

void VelodynePacket::read(istream &stream) {
  stream.read((char*)&mf64Timestamp, sizeof(double));
  stream.read((char*)mau8Packet, mcu16PacketSize);
  read(mau8Packet);
}

void VelodynePacket::write(ostream &stream) const {
  stream.write((char*)&mf64Timestamp, sizeof(double));
  stream.write((char*)mau8Packet, mcu16PacketSize);
}

void VelodynePacket::readFormatted(istream &stream) {
}

void VelodynePacket::writeFormatted(ostream &stream) const {
  stream << fixed << "Time: " << mf64Timestamp << endl;
  for (uint32_t i = 0; i < mcu16DataChunkNbr; i++) {
    stream << "Row: ";
    if (mData[i].mu16HeaderInfo == mcu16UpperBank)
      stream << "up";
    else
      stream << "low";
    stream << endl;
    stream << "Angle: " <<  (double)mData[i].mu16RotationalInfo /
      (double)mcu16RotationResolution << endl;
    for (uint32_t j = 0; j < DataChunk::mcu16LasersPerPacket; j++) {
      stream  << (double)mData[i].maLaserData[j].mu16Distance /
        (double)mcu16DistanceResolution << " ";
    }
    stream << endl;
  }

  if (*(char*)&mu32Reserved == 'V') {
    stream << "Spin count: " << mu16SpinCount << endl;
    stream << "Version: " << ((char*)&mu32Reserved)[1]
           << ((char*)&mu32Reserved)[2]
           << ((char*)&mu32Reserved)[3] << endl;
  }
  else {
    uint8_t u8IntTemp = ((uint8_t*)&mu16SpinCount)[1];
    double f64FracTemp = (double)((uint8_t*)&mu16SpinCount)[0] /
      (double)mcu16TemperatureResolution;
    double f64Temperature = (double)u8IntTemp + f64FracTemp;
    stream << "Temperature: " << f64Temperature << endl;
  }
}

void VelodynePacket::read(uint8_t au8Packet[]) {
  uint32_t u32Index = 0;
  for (uint32_t i = 0; i < mcu16DataChunkNbr; i++) {
    mData[i].mu16HeaderInfo = *(uint16_t*)&au8Packet[u32Index];
    u32Index += sizeof(uint16_t);
    mData[i].mu16RotationalInfo = *(uint16_t*)&au8Packet[u32Index];
    u32Index += sizeof(uint16_t);
    for (uint32_t j = 0; j < DataChunk::mcu16LasersPerPacket; j++) {
      mData[i].maLaserData[j].mu16Distance = *(uint16_t*)&au8Packet[u32Index];
      u32Index += sizeof(uint16_t);
      mData[i].maLaserData[j].mu8Intensity = *(uint8_t*)&au8Packet[u32Index];
      u32Index += sizeof(uint8_t);
    }
  }
  mu16SpinCount = *(uint16_t*)&au8Packet[u32Index];
  u32Index += sizeof(uint16_t);
  mu32Reserved = *(uint32_t*)&au8Packet[u32Index];
}

double VelodynePacket::getTimestamp() const {
  return mf64Timestamp;
}

uint16_t VelodynePacket::getSpinCount() const {
  return mu16SpinCount;
}

uint32_t VelodynePacket::getReserved() const {
  return mu32Reserved;
}

const DataChunk& VelodynePacket::getDataChunk(uint16_t u16DataChunkIdx) const
  throw(OutOfBoundException) {
  if (u16DataChunkIdx >= mcu16DataChunkNbr)
    throw OutOfBoundException("VelodynePacket::getDataChunk: Out of bound");
  return mData[u16DataChunkIdx];
}

const uint8_t* VelodynePacket::getRawPacket() const {
  return mau8Packet;
}

void VelodynePacket::setTimestamp(double f64Timestamp) {
  mf64Timestamp = f64Timestamp;
}

void VelodynePacket::setSpinCount(uint16_t u16SpinCount) {
  mu16SpinCount = u16SpinCount;
}

void VelodynePacket::setReserved(uint32_t u32Reserved) {
  mu32Reserved = u32Reserved;
}

void VelodynePacket::setDataChunk(const DataChunk &data,
  uint16_t u16DataChunkIdx) throw(OutOfBoundException) {
  if (u16DataChunkIdx >= mcu16DataChunkNbr)
    throw OutOfBoundException("VelodynePacket::getDataChunk: Out of bound");
  mData[u16DataChunkIdx] = data;
}

void VelodynePacket::setRawPacket(const uint8_t *au8Data) {
  memcpy(mau8Packet, au8Data, mcu16PacketSize * sizeof(uint8_t));
}

ostream& operator << (ostream &stream,
  const VelodynePacket &obj) {
  obj.writeFormatted(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodynePacket &obj) {
  obj.readFormatted(stream);
  return stream;
}

UDPConnection& operator >> (UDPConnection &stream,
  VelodynePacket &obj) throw(IOException) {
  obj.read(stream);
  return stream;
}
