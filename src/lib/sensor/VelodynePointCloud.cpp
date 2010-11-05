#include "VelodynePointCloud.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodynePointCloud::VelodynePointCloud() {
}

VelodynePointCloud::VelodynePointCloud(const VelodynePacket &vdynePacket,
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

      double f64SinRot = sin(f64Rotation) *
        vdyneCalibration.mCorr[u32LaserIdx].mf64CosRotCorr -
        cos(f64Rotation) * vdyneCalibration.mCorr[u32LaserIdx].mf64SinRotCorr;
      double f64CosRot = cos(f64Rotation) *
        vdyneCalibration.mCorr[u32LaserIdx].mf64CosRotCorr +
        sin(f64Rotation) * vdyneCalibration.mCorr[u32LaserIdx].mf64SinRotCorr;

      f64Distance /= (double)mcu16MeterConversion;
      double f64HorizOffsCorr =
        vdyneCalibration.mCorr[u32LaserIdx].mf64HorizOffsCorr /
        (double)mcu16MeterConversion;
      double f64VertOffsCorr =
        vdyneCalibration.mCorr[u32LaserIdx].mf64VertOffsCorr /
        (double)mcu16MeterConversion;

      double f64xyDist = f64Distance *
        vdyneCalibration.mCorr[u32LaserIdx].mf64CosVertCorr -
        f64VertOffsCorr * vdyneCalibration.mCorr[u32LaserIdx].mf64SinVertCorr;

      Point3D point;
      point.mf64x = f64xyDist * f64SinRot - f64HorizOffsCorr * f64CosRot;
      point.mf64y = f64xyDist * f64CosRot + f64HorizOffsCorr * f64SinRot;
      point.mf64z = f64Distance *
        vdyneCalibration.mCorr[u32LaserIdx].mf64SinVertCorr + f64VertOffsCorr *
        vdyneCalibration.mCorr[u32LaserIdx].mf64CosVertCorr;
      mPointCloud.push_back(point);
    }
  }
}

VelodynePointCloud::VelodynePointCloud(const VelodynePointCloud &other) {
}

VelodynePointCloud& VelodynePointCloud::operator =
  (const VelodynePointCloud &other) {
}

VelodynePointCloud::~VelodynePointCloud() {
}

void VelodynePointCloud::read(istream &stream) {
}

void VelodynePointCloud::write(ostream &stream) const {
}

void VelodynePointCloud::read(ifstream &stream) {
}

void VelodynePointCloud::write(ofstream &stream) const {
  for (uint32_t i = 0; i < mPointCloud.size(); i++)
    stream << mPointCloud[i].mf64x << " "
           << mPointCloud[i].mf64y << " "
           << mPointCloud[i].mf64z << endl;
}

ostream& operator << (ostream &stream,
  const VelodynePointCloud &obj) {
  obj.write(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodynePointCloud &obj) {
  obj.read(stream);
  return stream;
}

ofstream& operator << (ofstream &stream,
  const VelodynePointCloud &obj) {
  obj.write(stream);
  return stream;
}

ifstream& operator >> (ifstream &stream,
  VelodynePointCloud &obj) {
  obj.read(stream);
  return stream;
}
