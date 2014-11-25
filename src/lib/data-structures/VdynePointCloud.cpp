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

#include "data-structures/VdynePointCloud.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VdynePointCloud& VdynePointCloud::operator = (const VdynePointCloud& other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mPoints = other.mPoints;
    mStartRotationAngle = other.mStartRotationAngle;
    mEndRotationAngle = other.mEndRotationAngle;
  }
  return *this;
}

/******************************************************************************/
/* Streaming operations                                                       */
/******************************************************************************/

void VdynePointCloud::read(std::istream& /*stream*/) {
}

void VdynePointCloud::write(std::ostream& stream) const {
  stream << "Timestamp: " << mTimestamp << std::endl
    << "Start rotation: " << mStartRotationAngle << std::endl
    << "End rotation: " << mEndRotationAngle << std::endl;
  for (auto it = getPointBegin(); it != getPointEnd(); ++it)
    stream << *it << std::endl;
}

void VdynePointCloud::read(std::ifstream& /*stream*/) {
}

void VdynePointCloud::write(std::ofstream& /*stream*/) const {
}

void VdynePointCloud::writeBinary(std::ostream& stream) const {
  BinaryStreamWriter<std::ostream> binaryStream(stream);
  const size_t numPoints = mPoints.size();
  binaryStream << mTimestamp << mStartRotationAngle << mEndRotationAngle
    << numPoints;
  for (auto it = getPointBegin(); it != getPointEnd(); ++it)
    it->writeBinary(stream);
}

void VdynePointCloud::readBinary(std::istream& stream) {
  BinaryStreamReader<std::istream> binaryStream(stream);
  size_t numPoints;
  binaryStream >> mTimestamp >> mStartRotationAngle >> mEndRotationAngle
    >> numPoints;
  for (size_t i = 0; i < numPoints; ++i) {
    Point3D point;
    point.readBinary(stream);
    mPoints.push_back(point);
  }
}
