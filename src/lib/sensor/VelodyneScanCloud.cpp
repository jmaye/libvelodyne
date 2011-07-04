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

#include "sensor/VelodyneScanCloud.h"

#include <iostream>
#include <fstream>

#include <cmath>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodyneScanCloud::VelodyneScanCloud() :
  mTimestamp(0),
  mStartRotationAngle(0),
  mEndRotationAngle(0) {
}

VelodyneScanCloud::VelodyneScanCloud(const VelodynePacket& vdynePacket,
  const VelodyneCalibration& vdyneCalibration, double minDistance,
  double maxDistance) {
  mTimestamp = vdynePacket.getTimestamp();
  for (size_t i = 0; i < vdynePacket.mDataChunkNbr; i++) {
    size_t idxOffs = 0;
    const DataChunk& data = vdynePacket.getDataChunk(i);
    if (data.mHeaderInfo == vdynePacket.mLowerBank)
      idxOffs = data.mLasersPerPacket;

    double rotation =
      vdyneCalibration.deg2rad((double)data.mRotationalInfo /
      (double)vdynePacket.mRotationResolution);

    if (i == 0)
      mStartRotationAngle = rotation;
    else if (i == vdynePacket.mDataChunkNbr -1)
      mEndRotationAngle = rotation;

    for (size_t j = 0; j < data.mLasersPerPacket; j++) {
      size_t laserIdx = idxOffs + j;

      double distance = (vdyneCalibration.getDistCorr(laserIdx)
        + (double)data.maLaserData[j].mDistance /
        (double)vdynePacket.mDistanceResolution) /
        (double)mMeterConversion;

      if ((distance < mMinDistance) || (distance > mMaxDistance))
        continue;

      Scan scan;
      scan.mRange = distance;
      scan.mHeading = normalizeAngle(-(rotation -
        vdyneCalibration.getRotCorr(laserIdx)));
      scan.mPitch = vdyneCalibration.getVertCorr(laserIdx);
      scan.mIntensity = data.maLaserData[j].mIntensity;
      mScanCloudVector.push_back(scan);
    }
  }
}

VelodyneScanCloud::VelodyneScanCloud(const VelodyneScanCloud& other) :
  mTimestamp(other.mTimestamp),
  mScanCloudVector(other.mScanCloudVector) {
}

VelodyneScanCloud& VelodyneScanCloud::operator =
  (const VelodyneScanCloud &other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mScanCloudVector = other.mScanCloudVector;
  }
  return *this;
}

VelodyneScanCloud::~VelodyneScanCloud() {
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void VelodyneScanCloud::read(std::istream& stream) {
}

void VelodyneScanCloud::write(std::ostream& stream) const {
}

void VelodyneScanCloud::readFormatted(std::istream& stream) {
}

void VelodyneScanCloud::writeFormatted(std::ostream& stream) const {
}

std::ostream& operator << (std::ostream& stream, const VelodyneScanCloud& obj) {
  obj.writeFormatted(stream);
  return stream;
}

std::istream& operator >> (std::istream& stream, VelodyneScanCloud& obj) {
  obj.readFormatted(stream);
  return stream;
}

double VelodyneScanCloud::normalizeAnglePositive(double angle) const {
  return fmod(fmod(angle, 2.0 * M_PI) + 2.0 * M_PI, 2.0 * M_PI);
}

double VelodyneScanCloud::normalizeAngle(double angle) const {
  double value = normalizeAnglePositive(angle);
  if (value > M_PI)
    value -= 2.0 * M_PI;
  return value;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double VelodyneScanCloud::getTimestamp() const {
  return mTimestamp;
}

std::vector<Scan>::const_iterator VelodyneScanCloud::getStartIterator() const {
  return mScanCloudVector.begin();
}

std::vector<Scan>::const_iterator VelodyneScanCloud::getEndIterator() const {
  return mScanCloudVector.end();
}

void VelodyneScanCloud::setTimestamp(double timestamp) {
  mTimestamp = timestamp;
}

void VelodyneScanCloud::pushScan(const Scan &scan) {
  mScanCloudVector.push_back(scan);
}

size_t VelodyneScanCloud::getSize() const {
  return mScanCloudVector.size();
}

double VelodyneScanCloud::getStartRotationAngle() const {
  return mStartRotationAngle;
}

double VelodyneScanCloud::getEndRotationAngle() const {
  return mEndRotationAngle;
}

void VelodyneScanCloud::setStartRotationAngle(double angle) {
  mStartRotationAngle = angle;
}

void VelodyneScanCloud::setEndRotationAngle(double angle) {
  mEndRotationAngle = angle;
}
