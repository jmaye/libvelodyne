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

#include "com/UDPConnectionServer.h"
#include "base/Timestamp.h"
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

void DataPacket::read(std::ifstream& stream) {
}

void DataPacket::write(std::ofstream& stream) const {
}

void DataPacket::read(std::istream& stream) {
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
    stream << "Angle: " << (double)mData[i].mRotationalInfo /
      (double)mRotationResolution << std::endl;
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j) {
      stream  << (double)mData[i].mLaserData[j].mDistance /
        (double)mDistanceResolution << " ";
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
    double fracTemp = (double)((uint8_t*)&mSpinCount)[0] /
      (double)mTemperatureResolution;
    double temperature = (double)intTemp + fracTemp;
    stream << "Temperature: " << temperature << std::endl;
  }
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double DataPacket::getTimestamp() const {
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

void DataPacket::setTimestamp(double timestamp) {
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

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void DataPacket::readRawPacket(std::istream& stream) {
  for (size_t i = 0; i < mDataChunkNbr; ++i) {
    stream.read(reinterpret_cast<char*>(&mData[i].mHeaderInfo),
      sizeof(mData[i].mHeaderInfo));
    stream.read(reinterpret_cast<char*>(&mData[i].mRotationalInfo),
      sizeof(mData[i].mRotationalInfo));
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j) {
      stream.read(reinterpret_cast<char*>(&mData[i].mLaserData[j].mDistance),
        sizeof(mData[i].mLaserData[j].mDistance));
      stream.read(reinterpret_cast<char*>(&mData[i].mLaserData[j].mIntensity),
        sizeof(mData[i].mLaserData[j].mIntensity));
    }
  }
  stream.read(reinterpret_cast<char*>(&mSpinCount), sizeof(mSpinCount));
  stream.read(reinterpret_cast<char*>(&mReserved), sizeof(mReserved));
}

void DataPacket::writeRawPacket(std::ostream& stream) const {
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    stream.write(reinterpret_cast<const char*>(&mData[i].mHeaderInfo),
      sizeof(mData[i].mHeaderInfo));
    stream.write(reinterpret_cast<const char*>(&mData[i].mRotationalInfo),
      sizeof(mData[i].mRotationalInfo));
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; ++j) {
      stream.write(reinterpret_cast<const char*>(
        &mData[i].mLaserData[j].mDistance),
        sizeof(mData[i].mLaserData[j].mDistance));
      stream.write(reinterpret_cast<const char*>(
        &mData[i].mLaserData[j].mIntensity),
        sizeof(mData[i].mLaserData[j].mIntensity));
    }
  }
  stream.write(reinterpret_cast<const char*>(&mSpinCount), sizeof(mSpinCount));
  stream.write(reinterpret_cast<const char*>(&mReserved), sizeof(mReserved));
}

void DataPacket::readBinary(UDPConnectionServer& connection) {
  connection.readBuffer(reinterpret_cast<char*>(mRawPacket), mPacketSize);
  mTimestamp = Timestamp::now();
  std::istringstream is(std::string(reinterpret_cast<const char*>(mRawPacket),
    mPacketSize));
  readRawPacket(is);
}

void DataPacket::writeBinary(std::ostream& stream) const {
  stream.write(reinterpret_cast<const char*>(&mTimestamp), sizeof(mTimestamp));
  writeRawPacket(stream);
}

void DataPacket::readBinary(std::istream& stream) {
  stream.read(reinterpret_cast<char*>(&mTimestamp), sizeof(mTimestamp));
  readRawPacket(stream);
}
