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

/** \file DataPacket.h
    \brief This file defines the DataPacket class, which represents a
           Velodyne data packet
  */

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <stdint.h>

#include "base/Serializable.h"

class UDPConnectionServer;
class BinaryReader;
class BinaryWriter;

/** The class DataPacket represents a Velodyne data packet.
    \brief Velodyne data packet
  */
class DataPacket :
  public Serializable {
public:
  /** \name Types definitions
    @{
    */
  /// The struct LaserData represents the actual laser data.
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
  /// The struct DataChunk represents a data chunk.
  struct DataChunk {
    /// Number of lasers per packet
    static const size_t mLasersPerPacket = 32;
    /// Header info
    uint16_t mHeaderInfo;
    /// Rotational info
    uint16_t mRotationalInfo;
    /// Actual laser data
    LaserData mLaserData[mLasersPerPacket];
    /// Default constructor
    DataChunk() :
        mHeaderInfo(0),
        mRotationalInfo(0) {
    }
    /// Copy constructor
    DataChunk(const DataChunk& other) :
        mHeaderInfo(other.mHeaderInfo),
        mRotationalInfo(other.mRotationalInfo) {
      for (size_t i = 0; i < mLasersPerPacket; ++i)
        mLaserData[i] = other.mLaserData[i];
    }
    /// Assignment operator
    DataChunk& operator = (const DataChunk& other) {
      if (this != &other) {
        mHeaderInfo = other.mHeaderInfo;
        mRotationalInfo = other.mRotationalInfo;
        for (size_t i = 0; i < mLasersPerPacket; ++i)
          mLaserData[i] = other.mLaserData[i];
      }
      return *this;
    }
  };
  /** @}
    */

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
  /// Packet size
  static const size_t mPacketSize = 1206;
  /// Number of data chunks
  static const size_t mDataChunkNbr = 12;
  /** @}
    */

  /** \name Constructors/Destructor
    @{
    */
  /// Default constructor
  DataPacket();
  /// Copy constructor
  DataPacket(const DataPacket& other);
  /// Assignment operator
  DataPacket& operator = (const DataPacket& other);
  /// Destructor
  ~DataPacket();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the timestamp of the acquisition
  double getTimestamp() const;
  /// Sets the timestamp
  void setTimestamp(double timestamp);
  /// Returns the spin count
  uint16_t getSpinCount() const;
  /// Sets the spin count
  void setSpinCount(uint16_t spinCount);
  /// Returns the reserved stuff
  uint32_t getReserved() const;
  /// Sets the reserved stuff
  void setReserved(uint32_t reserved);
  /// Returns a data chunk
  const DataChunk& getDataChunk(size_t dataChunkIdx) const;
  /// Sets the data chunk
  void setDataChunk(const DataChunk& data, size_t dataChunkIdx);
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Binary read from UDP
  void readBinary(UDPConnectionServer& connection);
  /// Binary write into a output stream
  void writeBinary(std::ostream& stream) const;
  /// Binary read from an input stream
  void readBinary(std::istream& stream);
  /** @}
    */

protected:
  /** \name Stream methods
    @{
    */
  /// Reads from standard input
  virtual void read(std::istream& stream);
  /// Writes to standard output
  virtual void write(std::ostream& stream) const;
  /// Reads from a file
  virtual void read(std::ifstream& stream);
  /// Writes to a file
  virtual void write(std::ofstream& stream) const;
  /** @}
    */

  /** \name Protected methods
    @{
    */
  /// Read raw packet
  void readRawPacket(BinaryReader& stream);
  /// Write raw packet
  void writeRawPacket(BinaryWriter& stream) const;
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Timestamp of the packet
  double mTimestamp;
  /// Data in the packet
  DataChunk mData[mDataChunkNbr];
  /// Spin count
  uint16_t mSpinCount;
  /// Reserved field
  uint32_t mReserved;
  /// Raw data
  uint8_t mRawPacket[mPacketSize];
  /** @}
    */

};

#endif // DATAPACKET_H
