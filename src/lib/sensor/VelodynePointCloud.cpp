#include "VelodynePointCloud.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodynePointCloud::VelodynePointCloud() {
}

VelodynePointCloud::VelodynePointCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration) {
  for (uint32_t i = 0; i < vdynePacket.mcu16DataChunkNbr; i++) {
    uint32_t u32IdxOffs = 0;
    if (vdynePacket.mData[i].mu16HeaderInfo == 0xDDFF)
      u32IdxOffs = 32;
    for (uint32_t j = 0; j < vdynePacket.mcu16LaserNbr; j++) {
      uint32_t u32LaserIdx = u32IdxOffs + j;
      double f64Distance = vdyneCalibration.mCorr[u32LaserIdx].mf64DistCorr
        / 100.0 + (double)vdynePacket.mData[i].maLaserData[j].mu16Distance;

      if (f64Distance < mcf64MinDistance || f64Distance > mcf64MaxDistance)
        f64Distance = 0.0;

      double f64SinRotCorr =
        sin(deg2rad(vdyneCalibration.mCorr[u32LaserIdx].mf64RotCorr));
      double f64CosRotCorr =
        cos(deg2rad(vdyneCalibration.mCorr[u32LaserIdx].mf64RotCorr));
      double f64SinVert =
        sin(deg2rad(vdyneCalibration.mCorr[u32LaserIdx].mf64VertCorr));
      double f64CosVert =
        cos(deg2rad(vdyneCalibration.mCorr[u32LaserIdx].mf64VertCorr));

      double f64SinRot = sin(deg2rad(vdynePacket.mData[i].mu16RotationalInfo)) *
        f64CosRotCorr - cos(deg2rad(vdynePacket.mData[i].mu16RotationalInfo)) *
        f64SinRotCorr;
      double f64CosRot = cos(deg2rad(vdynePacket.mData[i].mu16RotationalInfo)) *
        f64CosRotCorr + sin(deg2rad(vdynePacket.mData[i].mu16RotationalInfo)) *
        f64SinRotCorr;

      double f64xyDist = f64Distance * f64CosVert - 
        vdyneCalibration.mCorr[u32LaserIdx].mf64VertOffsCorr / 100.0 * f64SinVert;

      Point3D point;
      point.mf64x = f64xyDist * f64SinRot -
        vdyneCalibration.mCorr[u32LaserIdx].mf64HorizOffsCorr /
        100.0 * f64CosRot;
      point.mf64y = f64xyDist * f64CosRot +
        vdyneCalibration.mCorr[u32LaserIdx].mf64HorizOffsCorr /
        100.0 * f64SinRot;
      point.mf64z = f64Distance * f64SinVert +
        vdyneCalibration.mCorr[u32LaserIdx].mf64VertOffsCorr /
        100.0 * f64CosVert;
      mPointCloud.push_back(point);
    }
  }
}

VelodynePointCloud::VelodynePointCloud(const VelodynePointCloud &other) {
}

VelodynePointCloud& VelodynePointCloud::operator =
  (const VelodynePointCloud &other) {
}

double VelodynePointCloud::deg2rad(double f64Deg) const {
  return f64Deg * M_PI / 180;
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
