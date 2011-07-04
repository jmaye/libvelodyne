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

/** \file VelodynePacket.h
    \brief This file defines the VelodynePacket class, which represents a
           Velodyne packet
  */

#ifndef VELODYNEPACKET_H
#define VELODYNEPACKET_H

#include "exceptions/IOException.h"
#include "exceptions/OutOfBoundException.h"

#include <iosfwd>

#include <stdint.h>

class UDPConnection;

/** The struct LaserData represents the actual laser data.
    \brief Laser data
  */
struct LaserData {
  /// Distance measure
  uint16_t mDistance;
  /// Intensity of the measure
  uint8_t mIntensity;
  /// Default constructor
  LaserData() :
    mDistance(0),
    mIntensity(0) {
  }
  /// Copy constructor
  LaserData(const LaserData& other) :
    mDistance(other.mDistance),
    mIntensity(other.mIntensity) {
  }
  /// Assignment operator
  LaserData& operator = (const LaserData& other) {
    if (this != &other) {
      mDistance = other.mDistance;
      mIntensity = other.mIntensity;
    }
    return *this;
  }
};

/** The struct DataChunk represents a data chunk
    \brief Data chunk
  */
struct DataChunk {
  /// Number of lasers per packet
  static const size_t mLasersPerPacket = 32;
  /// Header info
  uint16_t mHeaderInfo;
  /// Rotational info
  uint16_t mRotationalInfo;
  /// Actual laser data
  LaserData maLaserData[mLasersPerPacket];
  /// Default constructor
  DataChunk() :
    mHeaderInfo(0),
    mRotationalInfo(0) {
  }
  /// Copy constructor
  DataChunk(const DataChunk& other) :
    mHeaderInfo(other.mHeaderInfo),
    mRotationalInfo(other.mRotationalInfo) {
    for (size_t i = 0; i < mLasersPerPacket; i++) {
      maLaserData[i] = other.maLaserData[i];
    }
  }
  /// Assignment operator
  DataChunk& operator = (const DataChunk& other) {
    if (this != &other) {
      mHeaderInfo = other.mHeaderInfo;
      mRotationalInfo = other.mRotationalInfo;
      for (size_t i = 0; i < mLasersPerPacket; i++) {
        maLaserData[i] = other.maLaserData[i];
      }
    }
    return *this;
  }
};

/** The class VelodynePacket represents a Velodyne packet.
    \brief Velodyne packet
  */
class VelodynePacket {
public:
  /** \name Constants
    @{
    */
  /// Upper bank encoding
  static const size_t mUpperBank = 0xeeff;
  /// Lower bank encoding
  static const size_t mLowerBank = 0xddff;
  /// Rotation resolution
  static const size_t mRotationResolution = 100;
  /// Distance resolution
  static const size_t mDistanceResolution = 5;
  /// Temperature resolution
  static const size_t mTemperatureResolution = 256;
  /// Time resolution
  static const size_t mTimeResolution = 1000000;
  /// Packet size
  static const size_t mPacketSize = 1206;
  /// Number of data chunks
  static const size_t mDataChunkNbr = 12;
  /** @}
    */

private:
  /** \name Stream methods
    @{
    */
  /// Writes to std::ostream
  friend std::ostream& operator << (std::ostream& stream,
    const VelodynePacket &obj);
  /// Reads from std::istream
  friend std::istream& operator >> (std::istream& stream, VelodynePacket& obj);
  /** @}
    */

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodynePacket(const VelodynePacket& other);
  /// Assignment operator
  VelodynePacket& operator = (const VelodynePacket& other);
  /** @}
    */

  /** \name Stream methods
    @{
    */
  /// Reads formatted from istream
  void readFormatted(std::istream &stream);
  /// Writes formatted to ostream
  void writeFormatted(std::ostream &stream) const;
  /// Read to buffer
  void read(uint8_t au8Packet[]);
  /// Write to buffer
  void write(uint8_t au8Packet[]) const;
  /** @}
    */

  /** \name Private methods
    @{
    */
  /// Returns the time in seconds
  double seconds() const;
  /** @}
    */

  /** \name Private members
    @{
    */
  double mTimestamp;
  DataChunk mData[mDataChunkNbr];
  uint16_t mSpinCount;
  uint32_t mReserved;
  mutable uint8_t mau8Packet[mPacketSize];
  /** @}
    */

public:
  /** \name Constructors/Destructor
    @{
    */
  /// Default constructor
  VelodynePacket();
  /// Destructor
  ~VelodynePacket();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the timestamp of the acquisition
  double getTimestamp() const;
  /// Returns the spin count
  uint16_t getSpinCount() const;
  /// Returns the reserved stuff
  uint32_t getReserved() const;
  //// Returns a data chunk
  const DataChunk& getDataChunk(size_t dataChunkIdx) const
    throw(OutOfBoundException);
  /// Returns the raw packet
  const uint8_t* getRawPacket() const;
  /// Sets the timestamp
  void setTimestamp(double timestamp);
  /// Sets the spin count
  void setSpinCount(uint16_t spinCount);
  /// Sets the reserved stuff
  void setReserved(uint32_t reserved);
  /// Sets the data chunk
  void setDataChunk(const DataChunk& data, size_t dataChunkIdx)
    throw(OutOfBoundException);
  /// Sets the raw packet
  void setRawPacket(const uint8_t* au8Data);
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Reads from UDP
  void read(UDPConnection& stream);
  /// Reads from istream
  void read(std::istream& stream);
  /// Writews to ostream
  void write(std::ostream& stream) const;
  /** @}
    */

protected:

};

#endif // VELODYNEPACKET_H
