#include "VelodyneCalibration.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodyneCalibration::VelodyneCalibration() {
}

VelodyneCalibration::VelodyneCalibration(const VelodyneCalibration &other) {
}

VelodyneCalibration& VelodyneCalibration::operator =
  (const VelodyneCalibration &other) {
}

VelodyneCalibration::~VelodyneCalibration() {
}

void VelodyneCalibration::read(istream &stream) {
}

void VelodyneCalibration::write(ostream &stream) const {
  for (uint32_t i = 0; i < mcu16LasersNbr; i++) {
    stream << "Laser " << i << ": "
                       << mCorr[i].mf64RotCorr << " "
                       << mCorr[i].mf64VertCorr << " "
                       << mCorr[i].mf64DistCorr << " "
                       << mCorr[i].mf64VertOffsCorr << " "
                       << mCorr[i].mf64HorizOffsCorr << endl;
  }
}

void VelodyneCalibration::read(ifstream &stream) throw(IOException) {
  for (uint32_t i = 0; i < mcu16LasersNbr; i++) {
    string strKey;
    double f64Value;
    stream >> strKey;
    if (strKey.compare("id") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    if ((uint32_t)f64Value != i)
      throw IOException("VelodyneCalibration::read: Unexcepted id");
    stream >> strKey;
    if (strKey.compare("rotCorrection") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    mCorr[i].mf64RotCorr = deg2rad(f64Value);
    mCorr[i].mf64SinRotCorr = sin(mCorr[i].mf64RotCorr);
    mCorr[i].mf64CosRotCorr = cos(mCorr[i].mf64RotCorr);
    stream >> strKey;
    if (strKey.compare("vertCorrection") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    mCorr[i].mf64VertCorr = deg2rad(f64Value);
    mCorr[i].mf64SinVertCorr = sin(mCorr[i].mf64VertCorr);
    mCorr[i].mf64CosVertCorr = cos(mCorr[i].mf64VertCorr);
    stream >> strKey;
    if (strKey.compare("distCorrection") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    mCorr[i].mf64DistCorr = f64Value;
    stream >> strKey;
    if (strKey.compare("vertOffsetCorrection") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    mCorr[i].mf64VertOffsCorr = f64Value;
    stream >> strKey;
    if (strKey.compare("horizOffsetCorrection") != 0)
      throw IOException("VelodyneCalibration::read: Unexcepted key");
    stream >> f64Value;
    mCorr[i].mf64HorizOffsCorr = f64Value;
  }
}

void VelodyneCalibration::write(ofstream &stream) const {
}

double VelodyneCalibration::deg2rad(double f64Deg) const {
  return f64Deg * M_PI / 180.0;
}

ostream& operator << (ostream &stream,
  const VelodyneCalibration &obj) {
  obj.write(stream);
  return stream;
}

istream& operator >> (istream &stream,
  VelodyneCalibration &obj) {
  obj.read(stream);
  return stream;
}

ofstream& operator << (ofstream &stream,
  const VelodyneCalibration &obj) {
  obj.write(stream);
  return stream;
}

ifstream& operator >> (ifstream &stream,
  VelodyneCalibration &obj) throw(IOException) {
  obj.read(stream);
  return stream;
}
