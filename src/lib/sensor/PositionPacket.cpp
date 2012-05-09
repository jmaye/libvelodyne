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

void PositionPacket::readRawPacket(std::istream& stream) {
  stream.read(reinterpret_cast<char*>(&mEthernetHeader),
    sizeof(mEthernetHeader));
  stream.read(reinterpret_cast<char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream.read(reinterpret_cast<char*>(&mGyro1), sizeof(mGyro1));
  stream.read(reinterpret_cast<char*>(&mTemp1), sizeof(mTemp1));
  stream.read(reinterpret_cast<char*>(&mAccel1X), sizeof(mAccel1X));
  stream.read(reinterpret_cast<char*>(&mAccel1Y), sizeof(mAccel1Y));
  stream.read(reinterpret_cast<char*>(&mGyro2), sizeof(mGyro2));
  stream.read(reinterpret_cast<char*>(&mTemp2), sizeof(mTemp2));
  stream.read(reinterpret_cast<char*>(&mAccel2X), sizeof(mAccel2X));
  stream.read(reinterpret_cast<char*>(&mAccel2Y), sizeof(mAccel2Y));
  stream.read(reinterpret_cast<char*>(&mGyro3), sizeof(mGyro2));
  stream.read(reinterpret_cast<char*>(&mTemp3), sizeof(mTemp2));
  stream.read(reinterpret_cast<char*>(&mAccel3X), sizeof(mAccel2X));
  stream.read(reinterpret_cast<char*>(&mAccel3Y), sizeof(mAccel2Y));
  stream.read(reinterpret_cast<char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream.read(reinterpret_cast<char*>(&mGPSTimestamp), sizeof(mGPSTimestamp));
  stream.read(reinterpret_cast<char*>(&mNotUsed3), sizeof(mNotUsed3));
  stream.read(reinterpret_cast<char*>(&mNMEASentence), sizeof(mNMEASentence));
  stream.read(reinterpret_cast<char*>(&mNotUsed4), sizeof(mNotUsed4));
}

void PositionPacket::writeRawPacket(std::ostream& stream) const {
  stream.write(reinterpret_cast<const char*>(&mEthernetHeader),
    sizeof(mEthernetHeader));
  stream.write(reinterpret_cast<const char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream.write(reinterpret_cast<const char*>(&mGyro1), sizeof(mGyro1));
  stream.write(reinterpret_cast<const char*>(&mTemp1), sizeof(mTemp1));
  stream.write(reinterpret_cast<const char*>(&mAccel1X), sizeof(mAccel1X));
  stream.write(reinterpret_cast<const char*>(&mAccel1Y), sizeof(mAccel1Y));
  stream.write(reinterpret_cast<const char*>(&mGyro2), sizeof(mGyro2));
  stream.write(reinterpret_cast<const char*>(&mTemp2), sizeof(mTemp2));
  stream.write(reinterpret_cast<const char*>(&mAccel2X), sizeof(mAccel2X));
  stream.write(reinterpret_cast<const char*>(&mAccel2Y), sizeof(mAccel2Y));
  stream.write(reinterpret_cast<const char*>(&mGyro3), sizeof(mGyro2));
  stream.write(reinterpret_cast<const char*>(&mTemp3), sizeof(mTemp2));
  stream.write(reinterpret_cast<const char*>(&mAccel3X), sizeof(mAccel2X));
  stream.write(reinterpret_cast<const char*>(&mAccel3Y), sizeof(mAccel2Y));
  stream.write(reinterpret_cast<const char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream.write(reinterpret_cast<const char*>(&mGPSTimestamp),
    sizeof(mGPSTimestamp));
  stream.write(reinterpret_cast<const char*>(&mNotUsed3), sizeof(mNotUsed3));
  stream.write(reinterpret_cast<const char*>(&mNMEASentence),
    sizeof(mNMEASentence));
  stream.write(reinterpret_cast<const char*>(&mNotUsed4), sizeof(mNotUsed4));
}

void PositionPacket::readBinary(UDPConnectionServer& stream) {
  stream.readBuffer(reinterpret_cast<char*>(mRawPacket), mPacketSize);
  mTimestamp = Timestamp::now();
  std::istringstream is(std::string(reinterpret_cast<const char*>(mRawPacket),
    mPacketSize));
  readRawPacket(is);
}

void PositionPacket::writeBinary(std::ostream& stream) const {
  stream.write(reinterpret_cast<const char*>(&mTimestamp), sizeof(mTimestamp));
  writeRawPacket(stream);
}

void PositionPacket::readBinary(std::istream& stream) {
  stream.read(reinterpret_cast<char*>(&mTimestamp), sizeof(mTimestamp));
  readRawPacket(stream);
}
