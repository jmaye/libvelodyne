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
/* Statics initialization                                                     */
/******************************************************************************/

const double PositionPacket::mGyroScaleFactor = 0.09766;
const double PositionPacket::mTempScaleFactor = 0.1453;
const double PositionPacket::mTempOffset = 25;
const double PositionPacket::mAccelScaleFactor = 0.001221;

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
  stream << "Gyro 1: " << getGyro1() << std::endl;
  stream << "Gyro 2: " << getGyro2() << std::endl;
  stream << "Gyro 3: " << getGyro3() << std::endl;
  stream << "Temp 1: " << getTemp1() << std::endl;
  stream << "Temp 2: " << getTemp2() << std::endl;
  stream << "Temp 3: " << getTemp3() << std::endl;
  stream << "Accel 1 X: " << getAccel1X() << std::endl;
  stream << "Accel 1 Y: " << getAccel1Y() << std::endl;
  stream << "Accel 2 X: " << getAccel2X() << std::endl;
  stream << "Accel 2 Y: " << getAccel2Y() << std::endl;
  stream << "Accel 3 X: " << getAccel3X() << std::endl;
  stream << "Accel 3 Y: " << getAccel3Y() << std::endl;
  stream << "GPS timestamp: " << getGPSTimestamp() << std::endl;
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

double PositionPacket::getGyro1() const {
  if ((mGyro1 & 0x0fff) & 0x0800)
    return (int16_t)((mGyro1 & 0x0fff) | 0xf000) * mGyroScaleFactor;
  else
    return (mGyro1 & 0x0fff) * mGyroScaleFactor;
}

double PositionPacket::getGyro2() const {
  if ((mGyro2 & 0x0fff) & 0x0800)
    return (int16_t)((mGyro2 & 0x0fff) | 0xf000) * mGyroScaleFactor;
  else
    return (mGyro2 & 0x0fff) * mGyroScaleFactor;
}

double PositionPacket::getGyro3() const {
  if ((mGyro3 & 0x0fff) & 0x0800)
    return (int16_t)((mGyro3 & 0x0fff) | 0xf000) * mGyroScaleFactor;
  else
    return (mGyro3 & 0x0fff) * mGyroScaleFactor;
}

double PositionPacket::getTemp1() const {
  if ((mTemp1 & 0x0fff) & 0x0800)
    return (int16_t)((mTemp1 & 0x0fff) | 0xf000) * mTempScaleFactor +
      mTempOffset;
  else
    return (mTemp1 & 0x0fff) * mTempScaleFactor + mTempOffset;
}

double PositionPacket::getTemp2() const {
  if ((mTemp2 & 0x0fff) & 0x0800)
    return (int16_t)((mTemp2 & 0x0fff) | 0xf000) * mTempScaleFactor +
      mTempOffset;
  else
    return (mTemp2 & 0x0fff) * mTempScaleFactor + mTempOffset;
}

double PositionPacket::getTemp3() const {
  if ((mTemp3 & 0x0fff) & 0x0800)
    return (int16_t)((mTemp3 & 0x0fff) | 0xf000) * mTempScaleFactor +
      mTempOffset;
  else
    return (mTemp3 & 0x0fff) * mTempScaleFactor + mTempOffset;
}

double PositionPacket::getAccel1X() const {
  if ((mAccel1X & 0x0fff) & 0x0800)
    return (int16_t)((mAccel1X & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel1X & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getAccel2X() const {
  if ((mAccel2X & 0x0fff) & 0x0800)
    return (int16_t)((mAccel2X & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel2X & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getAccel3X() const {
  if ((mAccel3X & 0x0fff) & 0x0800)
    return (int16_t)((mAccel3X & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel3X & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getAccel1Y() const {
  if ((mAccel1Y & 0x0fff) & 0x0800)
    return (int16_t)((mAccel1Y & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel1Y & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getAccel2Y() const {
  if ((mAccel2Y & 0x0fff) & 0x0800)
    return (int16_t)((mAccel2Y & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel2Y & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getAccel3Y() const {
  if ((mAccel3Y & 0x0fff) & 0x0800)
    return (int16_t)((mAccel3Y & 0x0fff) | 0xf000) * mAccelScaleFactor;
  else
    return (mAccel3Y & 0x0fff) * mAccelScaleFactor;
}

double PositionPacket::getGPSTimestamp() const {
  return mGPSTimestamp / 1000000.0;
}

const uint8_t* PositionPacket::getNMEASentence() const {
  return mNMEASentence;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void PositionPacket::readRawPacket(BinaryReader& stream) {
  stream.read(reinterpret_cast<char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream >> mGyro1 >> mTemp1 >> mAccel1X >> mAccel1Y >> mGyro2 >> mTemp2 >>
    mAccel2X >> mAccel2Y >> mGyro3 >> mTemp3 >> mAccel3X >> mAccel3Y;
  stream.read(reinterpret_cast<char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream >> mGPSTimestamp;
  stream.read(reinterpret_cast<char*>(&mNotUsed3), sizeof(mNotUsed3));
  stream.read(reinterpret_cast<char*>(&mNMEASentence), sizeof(mNMEASentence));
  stream.read(reinterpret_cast<char*>(&mNotUsed4), sizeof(mNotUsed4));
}

void PositionPacket::writeRawPacket(BinaryWriter& stream) const {
  stream.write(reinterpret_cast<const char*>(&mNotUsed1), sizeof(mNotUsed1));
  stream << mGyro1 << mTemp1 << mAccel1X << mAccel1Y << mGyro2 << mTemp2 <<
    mAccel2X << mAccel2Y << mGyro3 << mTemp3 << mAccel3X << mAccel3Y;
  stream.write(reinterpret_cast<const char*>(&mNotUsed2), sizeof(mNotUsed2));
  stream << mGPSTimestamp;
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
  BinaryStreamWriter<std::ostream> binaryStream(stream);
  binaryStream << mTimestamp;
  writeRawPacket(binaryStream);
}

void PositionPacket::readBinary(std::istream& stream) {
  BinaryStreamReader<std::istream> binaryStream(stream);
  binaryStream >> mTimestamp;
  readRawPacket(binaryStream);
}
