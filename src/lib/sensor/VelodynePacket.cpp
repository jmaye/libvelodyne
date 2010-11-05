#include "VelodynePacket.h"

#include "UDPConnection.h"

#include <iostream>
#include <fstream>

#include <sys/time.h>

using namespace std;

VelodynePacket::VelodynePacket() {
}

VelodynePacket::VelodynePacket(const VelodynePacket &other) {
}

VelodynePacket& VelodynePacket::operator = (const VelodynePacket &other) {
}

VelodynePacket::~VelodynePacket() {
}

double VelodynePacket::seconds() const {
  struct timeval now;
  gettimeofday(&now, 0L);
  return (double)((double)now.tv_sec + ((double)now.tv_usec /
    (double)mcu32TimeResolution));
}

void VelodynePacket::read(istream &stream) {
}

void VelodynePacket::write(ostream &stream) const {
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
    for (uint32_t j = 0; j < mcu16LasersPerPacket; j++) {
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

void VelodynePacket::read(ifstream &stream) {
  stream.read((char*)&mf64Timestamp, sizeof(double));
  stream.read((char*)mau8Packet, mcu16PacketSize);
  read(mau8Packet);
}

void VelodynePacket::write(ofstream &stream) const {
  stream.write((char*)&mf64Timestamp, sizeof(double));
  stream.write((char*)mau8Packet, mcu16PacketSize);
}

void VelodynePacket::read(UDPConnection &stream) throw(IOException) {
  stream.readBuffer(mau8Packet, mcu16PacketSize);
  mf64Timestamp = seconds();
  read(mau8Packet);
}

void VelodynePacket::read(uint8_t au8Packet[]) {
  uint32_t u32Index = 0;
  for (uint32_t i = 0; i < mcu16DataChunkNbr; i++) {
    mData[i].mu16HeaderInfo = *(uint16_t*)&au8Packet[u32Index];
    u32Index += sizeof(uint16_t);
    mData[i].mu16RotationalInfo = *(uint16_t*)&au8Packet[u32Index];
    u32Index += sizeof(uint16_t);
    for (uint32_t j = 0; j < mcu16LasersPerPacket; j++) {
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

ostream& operator << (ostream &stream,
  const VelodynePacket &obj) {
  obj.write(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodynePacket &obj) {
  obj.read(stream);
  return stream;
}

ofstream& operator << (ofstream &stream,
  const VelodynePacket &obj) {
  obj.write(stream);
  return stream;
}

ifstream& operator >> (ifstream &stream,
  VelodynePacket &obj) {
  obj.read(stream);
  return stream;
}

UDPConnection& operator >> (UDPConnection &stream,
  VelodynePacket &obj) throw(IOException) {
  obj.read(stream);
  return stream;
}
