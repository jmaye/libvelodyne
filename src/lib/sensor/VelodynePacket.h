#ifndef VELODYNEPACKET_H
#define VELODYNEPACKET_H

#include "IOException.h"
#include "OutOfBoundException.h"

#include <iosfwd>

#include <stdint.h>

class UDPConnection;

struct LaserData {
  uint16_t mu16Distance;
  uint8_t mu8Intensity;
  LaserData() : mu16Distance(0),
                mu8Intensity(0) {
  }
  LaserData(const LaserData &other) : mu16Distance(other.mu16Distance),
                                      mu8Intensity(other.mu8Intensity) {
  }
  LaserData& operator = (const LaserData &other) {
    if (this != &other) {
      mu16Distance = other.mu16Distance;
      mu8Intensity = other.mu8Intensity;
    }
    return *this;
  }
};

struct DataChunk {
  static const uint16_t mcu16LasersPerPacket = 32;

  uint16_t mu16HeaderInfo;
  uint16_t mu16RotationalInfo;
  LaserData maLaserData[mcu16LasersPerPacket];
  DataChunk() : mu16HeaderInfo(0),
                mu16RotationalInfo(0) {
  }
  DataChunk(const DataChunk &other)
    : mu16HeaderInfo(other.mu16HeaderInfo),
      mu16RotationalInfo(other.mu16RotationalInfo) {
    for (uint32_t i = 0; i < mcu16LasersPerPacket; i++) {
      maLaserData[i] = other.maLaserData[i];
    }
  }
  DataChunk& operator = (const DataChunk &other) {
    if (this != &other) {
      mu16HeaderInfo = other.mu16HeaderInfo;
      mu16RotationalInfo = other.mu16RotationalInfo;
      for (uint32_t i = 0; i < mcu16LasersPerPacket; i++) {
        maLaserData[i] = other.maLaserData[i];
      }
    }
    return *this;
  }
};

class VelodynePacket {
public:
  static const uint16_t mcu16UpperBank = 0xeeff;
  static const uint16_t mcu16LowerBank = 0xddff;
  static const uint16_t mcu16RotationResolution = 100;
  static const uint16_t mcu16DistanceResolution = 5;
  static const uint16_t mcu16TemperatureResolution = 256;
  static const uint32_t mcu32TimeResolution = 1000000;
  static const uint16_t mcu16PacketSize = 1206;
  static const uint16_t mcu16DataChunkNbr = 12;

private:
  friend std::ostream& operator << (std::ostream &stream,
    const VelodynePacket &obj);
  friend std::istream& operator >> (std::istream &stream,
    VelodynePacket &obj);
  friend std::ofstream& operator << (std::ofstream &stream,
    const VelodynePacket &obj);
  friend std::ifstream& operator >> (std::ifstream &stream,
    VelodynePacket &obj);
  friend UDPConnection& operator >> (UDPConnection &stream,
    VelodynePacket &obj) throw(IOException);

  VelodynePacket(const VelodynePacket &other);
  VelodynePacket& operator = (const VelodynePacket &other);

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;
  virtual void read(UDPConnection &stream) throw(IOException);
  virtual void read(uint8_t au8Packet[]);

  double seconds() const;

  double mf64Timestamp;
  DataChunk mData[mcu16DataChunkNbr];
  uint16_t mu16SpinCount;
  uint32_t mu32Reserved;

  uint8_t mau8Packet[mcu16PacketSize];

public:
  VelodynePacket();
  ~VelodynePacket();

  double getTimestamp() const;
  uint16_t getSpinCount() const;
  uint32_t getReserved() const;
  const DataChunk& getDataChunk(uint16_t u16DataChunkIdx) const
    throw(OutOfBoundException);
  const uint8_t* getRawPacket() const;

  void setTimestamp(double f64Timestamp);
  void setSpinCount(uint16_t u16SpinCount);
  void setReserved(uint32_t u32Reserved);
  void setDataChunk(const DataChunk &data, uint16_t u16DataChunkIdx)
    throw(OutOfBoundException);
  void setRawPacket(const uint8_t *au8Data);

protected:

};

#endif // VELODYNEPACKET_H
