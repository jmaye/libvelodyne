#ifndef VELODYNEPACKET_H
#define VELODYNEPACKET_H

#include "IOException.h"

#include <iosfwd>

#include <stdint.h>

class UDPConnection;

class VelodynePacket {
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

  static const uint16_t mcu16PacketSize = 1206;
  static const uint16_t mcu16LaserNbr = 32;
  static const uint16_t mcu16DataChunkNbr = 12;

  struct LaserData {
    uint16_t mu16Distance;
    uint8_t mu8Intensity;
  };

  struct DataChunk {
    uint16_t mu16HeaderInfo;
    uint16_t mu16RotationalInfo;
    LaserData maLaserData[mcu16LaserNbr];
  };

  double mf64Timestamp;
  DataChunk mData[mcu16DataChunkNbr];
  uint16_t mu16SpinCount;
  uint32_t mu32Reserved;

  uint8_t mau8Packet[mcu16PacketSize];

public:
  VelodynePacket();
  ~VelodynePacket();

protected:

};

#endif // VELODYNEPACKET_H
