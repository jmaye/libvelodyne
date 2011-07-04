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

#include "sensor/VelodynePointCloud.h"

#include <iostream>
#include <fstream>

#include <cmath>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodynePointCloud::VelodynePointCloud() :
  mTimestamp(0),
  mStartRotationAngle(0),
  mEndRotationAngle(0) {
}

VelodynePointCloud::VelodynePointCloud(const VelodynePacket& vdynePacket,
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

      double sinRot = sin(rotation) *
        vdyneCalibration.getCosRotCorr(laserIdx) -
        cos(rotation) * vdyneCalibration.getSinRotCorr(laserIdx);
      double cosRot = cos(rotation) *
        vdyneCalibration.getCosRotCorr(laserIdx) +
        sin(rotation) * vdyneCalibration.getSinRotCorr(laserIdx);

      double horizOffsCorr =
        vdyneCalibration.getHorizOffsCorr(laserIdx) /
        (double)mMeterConversion;
      double vertOffsCorr =
        vdyneCalibration.getVertOffsCorr(laserIdx) /
        (double)mMeterConversion;

      double xyDist = distance *
        vdyneCalibration.getCosVertCorr(laserIdx) -
        vertOffsCorr * vdyneCalibration.getSinVertCorr(laserIdx);

      Point3D point;
      point.mX = xyDist * sinRot - horizOffsCorr * cosRot;
      point.mY = xyDist * cosRot + horizOffsCorr * sinRot;
      point.mZ = distance *
        vdyneCalibration.getSinVertCorr(laserIdx) + vertOffsCorr *
        vdyneCalibration.getCosVertCorr(laserIdx);
      point.mIntensity = data.maLaserData[j].mIntensity;
      mPointCloudVector.push_back(point);
    }
  }
}

VelodynePointCloud::VelodynePointCloud(const VelodynePointCloud& other) :
  mTimestamp(other.mTimestamp),
  mPointCloudVector(other.mPointCloudVector) {
}

VelodynePointCloud& VelodynePointCloud::operator =
  (const VelodynePointCloud& other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mPointCloudVector = other.mPointCloudVector;
  }
  return *this;
}

VelodynePointCloud::~VelodynePointCloud() {
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void VelodynePointCloud::read(std::istream& stream) {
}

void VelodynePointCloud::write(std::ostream& stream) const {
}

void VelodynePointCloud::readFormatted(std::istream& stream) {
}

void VelodynePointCloud::writeFormatted(std::ostream& stream) const {
  stream << "Timestamp: " << mTimestamp << std::endl;
  for (size_t i = 0; i < mPointCloudVector.size(); i++)
    stream << mPointCloudVector[i].mX << " "
           << mPointCloudVector[i].mY << " "
           << mPointCloudVector[i].mZ << std::endl;
}

std::ostream& operator << (std::ostream& stream, const VelodynePointCloud&
  obj) {
  obj.writeFormatted(stream);
  return stream;
}

std::istream& operator >> (std::istream& stream, VelodynePointCloud& obj) {
  obj.readFormatted(stream);
  return stream;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double VelodynePointCloud::getTimestamp() const {
  return mTimestamp;
}

std::vector<Point3D>::const_iterator VelodynePointCloud::getStartIterator()
  const {
  return mPointCloudVector.begin();
}

std::vector<Point3D>::const_iterator VelodynePointCloud::getEndIterator()
  const {
  return mPointCloudVector.end();
}

void VelodynePointCloud::setTimestamp(double timestamp) {
  mTimestamp = timestamp;
}

void VelodynePointCloud::pushPoint(const Point3D& point) {
  mPointCloudVector.push_back(point);
}

size_t VelodynePointCloud::getSize() const {
  return mPointCloudVector.size();
}

double VelodynePointCloud::getStartRotationAngle() const {
  return mStartRotationAngle;
}

double VelodynePointCloud::getEndRotationAngle() const {
  return mEndRotationAngle;
}

void VelodynePointCloud::setStartRotationAngle(double angle) {
  mStartRotationAngle = angle;
}

void VelodynePointCloud::setEndRotationAngle(double angle) {
  mEndRotationAngle = angle;
}
