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

#include "sensor/VelodynePacket.h"

#include "com/UDPConnection.h"

#include <iostream>
#include <fstream>

#include <sys/time.h>
#include <cstring>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodynePacket::VelodynePacket() :
  mTimestamp(0),
  mSpinCount(0),
  mReserved(0) {
  memset(mau8Packet, 0, mPacketSize * sizeof(uint8_t));
}

VelodynePacket::VelodynePacket(const VelodynePacket& other) :
  mTimestamp(other.mTimestamp),
  mSpinCount(other.mSpinCount),
  mReserved(other.mReserved) {
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    mData[i] = other.mData[i];
  }
  memcpy(mau8Packet, other.mau8Packet, mPacketSize * sizeof(uint8_t));
}

VelodynePacket& VelodynePacket::operator = (const VelodynePacket &other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mSpinCount = other.mSpinCount;
    mReserved = other.mReserved;
    for (size_t i = 0; i < mDataChunkNbr; i++) {
      mData[i] = other.mData[i];
    }
    memcpy(mau8Packet, other.mau8Packet, mPacketSize * sizeof(uint8_t));
  }
  return *this;
}

VelodynePacket::~VelodynePacket() {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double VelodynePacket::getTimestamp() const {
  return mTimestamp;
}

uint16_t VelodynePacket::getSpinCount() const {
  return mSpinCount;
}

uint32_t VelodynePacket::getReserved() const {
  return mReserved;
}

const DataChunk& VelodynePacket::getDataChunk(size_t dataChunkIdx) const
  throw (OutOfBoundException) {
  if (dataChunkIdx >= mDataChunkNbr)
    throw OutOfBoundException("VelodynePacket::getDataChunk(): Out of bound");
  return mData[dataChunkIdx];
}

const uint8_t* VelodynePacket::getRawPacket() const {
  return mau8Packet;
}

void VelodynePacket::setTimestamp(double timestamp) {
  mTimestamp = timestamp;
}

void VelodynePacket::setSpinCount(uint16_t spinCount) {
  mSpinCount = spinCount;
}

void VelodynePacket::setReserved(uint32_t reserved) {
  mReserved = reserved;
}

void VelodynePacket::setDataChunk(const DataChunk& data, size_t dataChunkIdx)
  throw (OutOfBoundException) {
  if (dataChunkIdx >= mDataChunkNbr)
    throw OutOfBoundException("VelodynePacket::getDataChunk(): Out of bound");
  mData[dataChunkIdx] = data;
}

void VelodynePacket::setRawPacket(const uint8_t* au8Data) {
  memcpy(mau8Packet, au8Data, mPacketSize * sizeof(uint8_t));
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

double VelodynePacket::seconds() const {
  struct timeval now;
  gettimeofday(&now, 0L);
  return (double)((double)now.tv_sec + ((double)now.tv_usec /
    (double)mTimeResolution));
}

void VelodynePacket::read(UDPConnection& stream) {
  stream.readBuffer(mau8Packet, mPacketSize);
  mTimestamp = seconds();
  read(mau8Packet);
}

void VelodynePacket::read(std::istream& stream) {
  stream.read((char*)&mTimestamp, sizeof(double));
  stream.read((char*)mau8Packet, mPacketSize);
  read(mau8Packet);
}

void VelodynePacket::write(std::ostream& stream) const {
  write(mau8Packet);
  stream.write((char*)&mTimestamp, sizeof(double));
  stream.write((char*)mau8Packet, mPacketSize);
}

void VelodynePacket::readFormatted(std::istream& stream) {
}

void VelodynePacket::writeFormatted(std::ostream& stream) const {
  stream << std::fixed << "Time: " << mTimestamp << std::endl;
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    stream << "Row: ";
    if (mData[i].mHeaderInfo == mUpperBank)
      stream << "up";
    else
      stream << "low";
    stream << std::endl;
    stream << "Angle: " << (double)mData[i].mRotationalInfo /
      (double)mRotationResolution << std::endl;
    for (size_t j = 0; j < DataChunk::mLasersPerPacket; j++) {
      stream  << (double)mData[i].maLaserData[j].mDistance /
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

void VelodynePacket::read(uint8_t au8Packet[]) {
  size_t index = 0;
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    mData[i].mHeaderInfo = *(uint16_t*)&au8Packet[index];
    index += sizeof(uint16_t);
    mData[i].mRotationalInfo = *(uint16_t*)&au8Packet[index];
    index += sizeof(uint16_t);
    for (uint32_t j = 0; j < DataChunk::mLasersPerPacket; j++) {
      mData[i].maLaserData[j].mDistance = *(uint16_t*)&au8Packet[index];
      index += sizeof(uint16_t);
      mData[i].maLaserData[j].mIntensity = *(uint8_t*)&au8Packet[index];
      index += sizeof(uint8_t);
    }
  }
  mSpinCount = *(uint16_t*)&au8Packet[index];
  index += sizeof(uint16_t);
  mReserved = *(uint32_t*)&au8Packet[index];
}

void VelodynePacket::write(uint8_t au8Packet[]) const {
  size_t index = 0;
  for (size_t i = 0; i < mDataChunkNbr; i++) {
    *(uint16_t*)&au8Packet[index] = mData[i].mHeaderInfo;
    index += sizeof(uint16_t);
    *(uint16_t*)&au8Packet[index] = mData[i].mRotationalInfo;
    index += sizeof(uint16_t);
    for (uint32_t j = 0; j < DataChunk::mLasersPerPacket; j++) {
      *(uint16_t*)&au8Packet[index] = mData[i].maLaserData[j].mDistance;
      index += sizeof(uint16_t);
      *(uint8_t*)&au8Packet[index] = mData[i].maLaserData[j].mIntensity;
      index += sizeof(uint8_t);
    }
  }
  *(uint16_t*)&au8Packet[index] = mSpinCount;
  index += sizeof(uint16_t);
  *(uint32_t*)&au8Packet[index] = mReserved;
}

std::ostream& operator << (std::ostream& stream, const VelodynePacket& obj) {
  obj.writeFormatted(stream);
  return stream;
}

std::istream& operator >> (std::istream& stream, VelodynePacket &obj) {
  obj.readFormatted(stream);
  return stream;
}

UDPConnection& operator >> (UDPConnection& stream, VelodynePacket& obj) {
  obj.read(stream);
  return stream;
}
