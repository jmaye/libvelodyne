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

VdyneScanCloud::VdyneScanCloud() :
    mTimestamp(0),
    mStartRotationAngle(0),
    mEndRotationAngle(0) {
}

VdyneScanCloud::VdyneScanCloud(const VdyneScanCloud& other) :
    Serializable(),
    mTimestamp(other.mTimestamp),
    mScans(other.mScans),
    mStartRotationAngle(other.mStartRotationAngle),
    mEndRotationAngle(other.mEndRotationAngle) {
}

VdyneScanCloud& VdyneScanCloud::operator = (const VdyneScanCloud& other) {
  if (this != &other) {
    mTimestamp = other.mTimestamp;
    mScans = other.mScans;
    mStartRotationAngle = other.mStartRotationAngle;
    mEndRotationAngle = other.mEndRotationAngle;
  }
  return *this;
}

VdyneScanCloud::~VdyneScanCloud() {
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

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

double VdyneScanCloud::getTimestamp() const {
  return mTimestamp;
}

void VdyneScanCloud::setTimestamp(double timestamp) {
  mTimestamp = timestamp;
}

double VdyneScanCloud::getStartRotationAngle() const {
  return mStartRotationAngle;
}

void VdyneScanCloud::setStartRotationAngle(double angle) {
  mStartRotationAngle = angle;
}

double VdyneScanCloud::getEndRotationAngle() const {
  return mEndRotationAngle;
}

void VdyneScanCloud::setEndRotationAngle(double angle) {
  mEndRotationAngle = angle;
}

VdyneScanCloud::ConstScanIterator VdyneScanCloud::getScanBegin() const {
  return mScans.begin();
}

VdyneScanCloud::ScanIterator VdyneScanCloud::getScanBegin() {
  return mScans.begin();
}

VdyneScanCloud::ConstScanIterator VdyneScanCloud::getScanEnd() const {
  return mScans.end();
}

VdyneScanCloud::ScanIterator VdyneScanCloud::getScanEnd() {
  return mScans.end();
}

const VdyneScanCloud::Container& VdyneScanCloud::getScans() const {
  return mScans;
}

size_t VdyneScanCloud::getSize() const {
  return mScans.size();
}

void VdyneScanCloud::insertScan(const Scan& point) {
  mScans.push_back(point);
}

void VdyneScanCloud::clear() {
  mScans.clear();
}
