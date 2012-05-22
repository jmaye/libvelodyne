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

#include "sensor/PositionPacket.h"

#include "com/UDPConnectionServer.h"
#include "base/Timestamp.h"
#include "base/BinaryBufferReader.h"
#include "base/BinaryStreamReader.h"
#include "base/BinaryStreamWriter.h"
#include "exceptions/OutOfBoundException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

PositionPacket::PositionPacket() :
    mTimestamp(0) {
}

PositionPacket::PositionPacket(const PositionPacket& other) :
    mTimestamp(other.mTimestamp) {
}

PositionPacket& PositionPacket::operator = (const PositionPacket &other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
  }
  return *this;
}

PositionPacket::~PositionPacket() {
}

/******************************************************************************/
/* Streaming operations                                                       */
/******************************************************************************/

void PositionPacket::read(std::ifstream& stream) {
}

void PositionPacket::write(std::ofstream& stream) const {
}

void PositionPacket::read(std::istream& stream) {
}

void PositionPacket::write(std::ostream& stream) const {
  stream << std::fixed << "Time: " << mTimestamp << std::endl;
  stream << mGyro1 * mGyroScaleFactor << std::endl;
  stream << mTemp1 * mTempScaleFactor + mTempOffset << std::endl;
  stream << mAccel1X * mAccelScaleFactor << std::endl;
  stream << mAccel1Y * mAccelScaleFactor << std::endl;
  stream << mGyro2 * mGyroScaleFactor << std::endl;
  stream << mTemp2 * mTempScaleFactor + mTempOffset << std::endl;
  stream << mAccel2X * mAccelScaleFactor << std::endl;
  stream << mAccel2Y * mAccelScaleFactor << std::endl;
  stream << mGyro3 * mGyroScaleFactor << std::endl;
  stream << mTemp3 * mTempScaleFactor + mTempOffset << std::endl;
  stream << mAccel3X * mAccelScaleFactor << std::endl;
  stream << mAccel3Y * mAccelScaleFactor << std::endl;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double PositionPacket::getTimestamp() const {
  return mTimestamp;
}

void PositionPacket::setTimestamp(double timestamp) {
  mTimestamp = timestamp;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void PositionPacket::readRawPacket(BinaryReader& stream) {
  stream.read(reinterpret_cast<char*>(&mEthernetHeader),
    sizeof(mEthernetHeader));
  stream.read(reinterpret_cast<char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream >> mGyro1 >> mTemp1 >> mAccel1X >> mAccel1Y >> mGyro2 >> mTemp2 >>
    mAccel2X >> mAccel2Y >> mGyro3 >> mTemp3 >> mAccel3X >> mAccel3Y;
  stream.read(reinterpret_cast<char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream >> mTimestamp;
  stream.read(reinterpret_cast<char*>(&mNotUsed3), sizeof(mNotUsed3));
  stream.read(reinterpret_cast<char*>(&mNMEASentence), sizeof(mNMEASentence));
  stream.read(reinterpret_cast<char*>(&mNotUsed4), sizeof(mNotUsed4));
}

void PositionPacket::writeRawPacket(BinaryWriter& stream) const {
  stream.write(reinterpret_cast<const char*>(&mEthernetHeader),
    sizeof(mEthernetHeader));
  stream.write(reinterpret_cast<const char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream << mGyro1 << mTemp1 << mAccel1X << mAccel1Y << mGyro2 << mTemp2 <<
    mAccel2X << mAccel2Y << mGyro3 << mTemp3 << mAccel3X << mAccel3Y;
  stream.write(reinterpret_cast<const char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream << mTimestamp;
  stream.write(reinterpret_cast<const char*>(&mNotUsed3), sizeof(mNotUsed3));
  stream.write(reinterpret_cast<const char*>(&mNMEASentence),
    sizeof(mNMEASentence));
  stream.write(reinterpret_cast<const char*>(&mNotUsed4), sizeof(mNotUsed4));
}

void PositionPacket::readBinary(UDPConnectionServer& stream) {
  stream.read(reinterpret_cast<char*>(mRawPacket), mPacketSize);
  mTimestamp = Timestamp::now();
  BinaryBufferReader binaryStream(reinterpret_cast<char*>(mRawPacket),
    mPacketSize);
  readRawPacket(binaryStream);
}

void PositionPacket::writeBinary(std::ostream& stream) const {
  BinaryStreamWriter binaryStream(stream);
  stream << mTimestamp;
  writeRawPacket(binaryStream);
}

void PositionPacket::readBinary(std::istream& stream) {
  BinaryStreamReader binaryStream(stream);
  stream >> mTimestamp;
  readRawPacket(binaryStream);
}
