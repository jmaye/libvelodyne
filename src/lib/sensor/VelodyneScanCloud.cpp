#include "VelodyneScanCloud.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodyneScanCloud::VelodyneScanCloud() : mf64Timestamp(0),
                                         mf64StartRotationAngle(0),
                                         mf64EndRotationAngle(0) {
}

VelodyneScanCloud::VelodyneScanCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration) {
  mf64Timestamp = vdynePacket.getTimestamp();
  for (uint32_t i = 0; i < vdynePacket.mcu16DataChunkNbr; i++) {
    uint32_t u32IdxOffs = 0;
    const DataChunk &data = vdynePacket.getDataChunk(i);
    if (data.mu16HeaderInfo == vdynePacket.mcu16LowerBank)
      u32IdxOffs = data.mcu16LasersPerPacket;

    double f64Rotation =
      vdyneCalibration.deg2rad((double)data.mu16RotationalInfo /
      (double)vdynePacket.mcu16RotationResolution);

    if (i == 0)
      mf64StartRotationAngle = f64Rotation;
    else if (i == vdynePacket.mcu16DataChunkNbr -1)
      mf64EndRotationAngle = f64Rotation;

    for (uint32_t j = 0; j < data.mcu16LasersPerPacket; j++) {
      uint32_t u32LaserIdx = u32IdxOffs + j;

      double f64Distance = vdyneCalibration.getDistCorr(u32LaserIdx)
        + (double)data.maLaserData[j].mu16Distance /
        (double)vdynePacket.mcu16DistanceResolution;

      if (f64Distance < mcf64MinDistance)
        break;

      f64Distance /= (double)mcu16MeterConversion;

      Scan scan;
      scan.mf64Range = f64Distance;
      scan.mf64Heading = normalizeAngle(-(f64Rotation -
        vdyneCalibration.getRotCorr(u32LaserIdx)));
      scan.mf64Pitch = vdyneCalibration.getVertCorr(u32LaserIdx);
      scan.mu8Intensity = data.maLaserData[j].mu8Intensity;
      mScanCloudVector.push_back(scan);
    }
  }
}

VelodyneScanCloud::VelodyneScanCloud(const VelodyneScanCloud &other)
  : mf64Timestamp(other.mf64Timestamp),
    mScanCloudVector(other.mScanCloudVector) {
}

VelodyneScanCloud& VelodyneScanCloud::operator =
  (const VelodyneScanCloud &other) {
  if (this != &other) {
    mf64Timestamp = other.mf64Timestamp;
    mScanCloudVector = other.mScanCloudVector;
  }
  return *this;
}

VelodyneScanCloud::~VelodyneScanCloud() {
}

void VelodyneScanCloud::read(istream &stream) {
}

void VelodyneScanCloud::write(ostream &stream) const {
}

void VelodyneScanCloud::readFormatted(istream &stream) {
}

void VelodyneScanCloud::writeFormatted(ostream &stream) const {
}

double VelodyneScanCloud::normalizeAnglePositive(double f64Angle) const {
  return fmod(fmod(f64Angle, 2.0 * M_PI) + 2.0 * M_PI, 2.0 * M_PI);
}

double VelodyneScanCloud::normalizeAngle(double f64Angle) const {
  double f64Value = normalizeAnglePositive(f64Angle);
  if (f64Value > M_PI)
    f64Value -= 2.0 * M_PI;
  return f64Value;
}

double VelodyneScanCloud::getTimestamp() const {
  return mf64Timestamp;
}

vector<Scan>::const_iterator VelodyneScanCloud::getStartIterator() const {
  return mScanCloudVector.begin();
}

vector<Scan>::const_iterator VelodyneScanCloud::getEndIterator() const {
  return mScanCloudVector.end();
}

void VelodyneScanCloud::setTimestamp(double f64Timestamp) {
  mf64Timestamp = f64Timestamp;
}

void VelodyneScanCloud::pushScan(const Scan &scan) {
  mScanCloudVector.push_back(scan);
}

uint32_t VelodyneScanCloud::getSize() const {
  return mScanCloudVector.size();
}

double VelodyneScanCloud::getStartRotationAngle() const {
  return mf64StartRotationAngle;
}

double VelodyneScanCloud::getEndRotationAngle() const {
  return mf64EndRotationAngle;
}

void VelodyneScanCloud::setStartRotationAngle(double f64Angle) {
  mf64StartRotationAngle = f64Angle;
}

void VelodyneScanCloud::setEndRotationAngle(double f64Angle) {
  mf64EndRotationAngle = f64Angle;
}

ostream& operator << (ostream &stream,
  const VelodyneScanCloud &obj) {
  obj.writeFormatted(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodyneScanCloud &obj) {
  obj.readFormatted(stream);
  return stream;
}
