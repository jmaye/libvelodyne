#include "VelodyneScanCloud.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodyneScanCloud::VelodyneScanCloud() {
}

VelodyneScanCloud::VelodyneScanCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration) {
  mf64Timestamp = vdynePacket.mf64Timestamp;
  for (uint32_t i = 0; i < vdynePacket.mcu16DataChunkNbr; i++) {
    uint32_t u32IdxOffs = 0;
    if (vdynePacket.mData[i].mu16HeaderInfo == vdynePacket.mcu16LowerBank)
      u32IdxOffs = vdynePacket.mcu16LasersPerPacket;

    double f64Rotation =
      vdyneCalibration.deg2rad((double)vdynePacket.mData[i].mu16RotationalInfo /
      (double)vdynePacket.mcu16RotationResolution);

    for (uint32_t j = 0; j < vdynePacket.mcu16LasersPerPacket; j++) {
      uint32_t u32LaserIdx = u32IdxOffs + j;

      double f64Distance = vdyneCalibration.mCorr[u32LaserIdx].mf64DistCorr
        + (double)vdynePacket.mData[i].maLaserData[j].mu16Distance /
        (double)vdynePacket.mcu16DistanceResolution;

      if (f64Distance < mcf64MinDistance)
        break;

      f64Distance /= (double)mcu16MeterConversion;

      Scan scan;
      scan.mf64Range = f64Distance;
      scan.mf64Heading = normalizeAngle(-(f64Rotation -
        vdyneCalibration.mCorr[u32LaserIdx].mf64RotCorr));
      scan.mf64Pitch = vdyneCalibration.mCorr[u32LaserIdx].mf64VertCorr;
      mScanCloud.push_back(scan);
    }
  }
}

VelodyneScanCloud::VelodyneScanCloud(const VelodyneScanCloud &other) {
}

VelodyneScanCloud& VelodyneScanCloud::operator =
  (const VelodyneScanCloud &other) {
}

VelodyneScanCloud::~VelodyneScanCloud() {
}

void VelodyneScanCloud::read(istream &stream) {
}

void VelodyneScanCloud::write(ostream &stream) const {
}

void VelodyneScanCloud::read(ifstream &stream) {
}

void VelodyneScanCloud::write(ofstream &stream) const {
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

ostream& operator << (ostream &stream,
  const VelodyneScanCloud &obj) {
  obj.write(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodyneScanCloud &obj) {
  obj.read(stream);
  return stream;
}

ofstream& operator << (ofstream &stream,
  const VelodyneScanCloud &obj) {
  obj.write(stream);
  return stream;
}

ifstream& operator >> (ifstream &stream,
  VelodyneScanCloud &obj) {
  obj.read(stream);
  return stream;
}
