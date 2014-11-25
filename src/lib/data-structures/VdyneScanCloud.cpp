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

#include "data-structures/VdyneScanCloud.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VdyneScanCloud& VdyneScanCloud::operator = (const VdyneScanCloud& other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mScans = other.mScans;
    mStartRotationAngle = other.mStartRotationAngle;
    mEndRotationAngle = other.mEndRotationAngle;
  }
  return *this;
}

/******************************************************************************/
/* Streaming operations                                                       */
/******************************************************************************/

void VdyneScanCloud::read(std::istream& /*stream*/) {
}

void VdyneScanCloud::write(std::ostream& stream) const {
  stream << "Timestamp: " << mTimestamp << std::endl
    << "Start rotation: " << mStartRotationAngle << std::endl
    << "End rotation: " << mEndRotationAngle << std::endl;
  for (auto it = getScanBegin(); it != getScanEnd(); ++it)
    stream << *it << std::endl;
}

void VdyneScanCloud::read(std::ifstream& /*stream*/) {
}

void VdyneScanCloud::write(std::ofstream& /*stream*/) const {
}

void VdyneScanCloud::writeBinary(std::ostream& stream) const {
  BinaryStreamWriter<std::ostream> binaryStream(stream);
  const size_t numScans = mScans.size();
  binaryStream << mTimestamp << mStartRotationAngle << mEndRotationAngle
    << numScans;
  for (auto it = getScanBegin(); it != getScanEnd(); ++it)
    it->writeBinary(stream);
}

void VdyneScanCloud::readBinary(std::istream& stream) {
  BinaryStreamReader<std::istream> binaryStream(stream);
  size_t numScans;
  binaryStream >> mTimestamp >> mStartRotationAngle >> mEndRotationAngle
    >> numScans;
  for (size_t i = 0; i < numScans; ++i) {
    Scan scan;
    scan.readBinary(stream);
    mScans.push_back(scan);
  }
}
