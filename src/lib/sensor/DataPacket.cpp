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

#include "sensor/DataPacket.h"

#include <cstring>
#include <chrono>

#include "com/UDPConnectionServer.h"
#include "base/BinaryBufferReader.h"
#include "base/BinaryStreamReader.h"
#include "base/BinaryStreamWriter.h"
#include "exceptions/OutOfBoundException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

DataPacket::DataPacket() :
    mTimestamp(0),
    mSpinCount(0),
    mReserved(0) {
}

DataPacket::DataPacket(const DataPacket& other) :
    Serializable(),
    mTimestamp(other.mTimestamp),
    mSpinCount(other.mSpinCount),
    mReserved(other.mReserved) {
  for (size_t i = 0; i < mDataChunkNbr; ++i)
    mData[i] = other.mData[i];
}

DataPacket& DataPacket::operator = (const DataPacket &other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mSpinCount = other.mSpinCount;
    mReserved = other.mReserved;
    for (size_t i = 0; i < mDataChunkNbr; i++)
      mData[i] = other.mData[i];
  }
  return *this;
}

DataPacket::~DataPacket() {
}

/******************************************************************************/
/* Streaming operations                                                       */
/******************************************************************************/

void DataPacket::read(std::ifstream& /*stream*/) {
}

void DataPacket::write(std::ofstream& /*stream*/) const {
}

void DataPacket::read(std::istream& /*stream*/) {
}

void DataPacket::write(std::ostream& stream) const {
  stream << std::fixed << "Time: " << mTimestamp << std::endl;
  for (size_t i = 0; i < mDataChunkNbr; ++i) {
    stream << "Row: ";
    if (mData[i].mHeaderInfo == mUpperBank)
      stream << "up";
    else
      stream << "low";
    stream << std::endl;
    stream << "Angle: " << (float)mData[i].mRotationalInfo /
      (float)mRotationResolution << std::endl;
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j) {
      stream  << (float)mData[i].mLaserData[j].mDistance /
        (float)mDistanceResolution << " ";
    }
    stream << std::endl;
  }
  if (*(char*)&mReserved == 'V') {
    stream << "Spin count: " << mSpinCount << std::endl;
    stream << "Version: " << ((char*)&mReserved)[1]
           << ((char*)&mReserved)[2]
           << ((char*)&mReserved)[3] << std::endl;
  }
  else {
    uint8_t intTemp = ((uint8_t*)&mSpinCount)[1];
    float fracTemp = (float)((uint8_t*)&mSpinCount)[0] /
      (float)mTemperatureResolution;
    float temperature = (float)intTemp + fracTemp;
    stream << "Temperature: " << temperature << std::endl;
  }
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

int64_t DataPacket::getTimestamp() const {
  return mTimestamp;
}

uint16_t DataPacket::getSpinCount() const {
  return mSpinCount;
}

uint32_t DataPacket::getReserved() const {
  return mReserved;
}

const DataPacket::DataChunk& DataPacket::getDataChunk(size_t dataChunkIdx)
    const {
  if (dataChunkIdx >= mDataChunkNbr)
    throw OutOfBoundException<size_t>(dataChunkIdx,
      "DataPacket::getDataChunk(): Out of bound",
      __FILE__, __LINE__);
  return mData[dataChunkIdx];
}

void DataPacket::setTimestamp(int64_t timestamp) {
  mTimestamp = timestamp;
}

void DataPacket::setSpinCount(uint16_t spinCount) {
  mSpinCount = spinCount;
}

void DataPacket::setReserved(uint32_t reserved) {
  mReserved = reserved;
}

void DataPacket::setDataChunk(const DataChunk& data, size_t dataChunkIdx) {
  if (dataChunkIdx >= mDataChunkNbr)
    throw OutOfBoundException<size_t>(dataChunkIdx,
      "DataPacket::getDataChunk(): Out of bound",
      __FILE__, __LINE__);
  mData[dataChunkIdx] = data;
}

uint32_t DataPacket::getGPSTimestamp() const {
  uint32_t gpsTimestamp;
  memcpy(reinterpret_cast<uint8_t*>(&gpsTimestamp),
    reinterpret_cast<const uint8_t*>(&mSpinCount), 2);
  memcpy(reinterpret_cast<uint8_t*>(
    &(reinterpret_cast<uint8_t*>(&gpsTimestamp)[2])),
    reinterpret_cast<const uint8_t*>(&mReserved), 2);
  return gpsTimestamp;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void DataPacket::readRawPacket(BinaryReader& stream) {
  for (size_t i = 0; i < mDataChunkNbr; ++i) {
    stream >> mData[i].mHeaderInfo >> mData[i].mRotationalInfo;
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j)
      stream >> mData[i].mLaserData[j].mDistance
        >> mData[i].mLaserData[j].mIntensity;
  }
  stream >> mSpinCount >> mReserved;
}

void DataPacket::writeRawPacket(BinaryWriter& stream) const {
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    stream << mData[i].mHeaderInfo << mData[i].mRotationalInfo;
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j)
      stream << mData[i].mLaserData[j].mDistance
        << mData[i].mLaserData[j].mIntensity;
  }
  stream << mSpinCount << mReserved;
}

void DataPacket::readBinary(UDPConnectionServer& connection) {
  connection.read(reinterpret_cast<char*>(mRawPacket), mPacketSize);
  auto time = std::chrono::high_resolution_clock::now();
  mTimestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
    time.time_since_epoch()).count();
  BinaryBufferReader binaryStream(reinterpret_cast<char*>(mRawPacket),
    mPacketSize);
  readRawPacket(binaryStream);
}

void DataPacket::writeBinary(std::ostream& stream) const {
  BinaryStreamWriter<std::ostream> binaryStream(stream);
  binaryStream << mTimestamp;
  writeRawPacket(binaryStream);
}

void DataPacket::readBinary(std::istream& stream) {
  BinaryStreamReader<std::istream> binaryStream(stream);
  binaryStream >> mTimestamp;
  readRawPacket(binaryStream);
}
