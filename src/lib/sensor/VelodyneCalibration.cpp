#include "VelodyneCalibration.h"

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

VelodyneCalibration::VelodyneCalibration() {
}

VelodyneCalibration::VelodyneCalibration(const VelodyneCalibration &other) {
  for (uint32_t i = 0; i < mcu16LasersNbr; i++) {
    mCorr[i] = other.mCorr[i];
  }
}

VelodyneCalibration& VelodyneCalibration::operator =
  (const VelodyneCalibration &other) {
  for (uint32_t i = 0; i < mcu16LasersNbr; i++) {
    mCorr[i] = other.mCorr[i];
  }
  return *this;
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
  for (uint32_t i = 0; i < mcu16LasersNbr; i++) {
    stream << "id " << i << endl
           << "rotCorrection " << rad2deg(mCorr[i].mf64RotCorr) << endl
           << "vertCorrection " << rad2deg(mCorr[i].mf64VertCorr) << endl
           << "distCorrection " << mCorr[i].mf64DistCorr << endl
           << "vertOffsetCorrection " << mCorr[i].mf64VertOffsCorr << endl
           << "horizOffsetCorrection " << mCorr[i].mf64HorizOffsCorr << endl
           << endl;
  }
}

double VelodyneCalibration::deg2rad(double f64Deg) const {
  return f64Deg * M_PI / 180.0;
}

double VelodyneCalibration::rad2deg(double f64Rad) const {
  return f64Rad * 180.0 / M_PI;
}

double VelodyneCalibration::getRotCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getRotCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64RotCorr;
}

double VelodyneCalibration::getSinRotCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getSinRotCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64SinRotCorr;
}

double VelodyneCalibration::getCosRotCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getCosRotCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64CosRotCorr;
}

double VelodyneCalibration::getVertCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getVertCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64VertCorr;
}

double VelodyneCalibration::getSinVertCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getSinVertCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64SinVertCorr;
}

double VelodyneCalibration::getCosVertCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getCosVertCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64CosVertCorr;
}

double VelodyneCalibration::getDistCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getDistCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64DistCorr;
}

double VelodyneCalibration::getVertOffsCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getVertOffsCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64VertOffsCorr;
}

double VelodyneCalibration::getHorizOffsCorr(uint16_t u16LaserNbr) const
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getHorizOffsCorr: Out of bound");
  return mCorr[u16LaserNbr].mf64HorizOffsCorr;
}

void VelodyneCalibration::setRotCorr(uint16_t u16LaserNbr, double mf64Value)
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setRotCorr: Out of bound");
  mCorr[u16LaserNbr].mf64RotCorr = deg2rad(mf64Value);
  mCorr[u16LaserNbr].mf64SinRotCorr = sin(mCorr[u16LaserNbr].mf64RotCorr);
  mCorr[u16LaserNbr].mf64CosRotCorr = cos(mCorr[u16LaserNbr].mf64RotCorr);
}

void VelodyneCalibration::setVertCorr(uint16_t u16LaserNbr, double mf64Value)
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setVertCorr: Out of bound");
  mCorr[u16LaserNbr].mf64VertCorr = deg2rad(mf64Value);
  mCorr[u16LaserNbr].mf64SinVertCorr = sin(mCorr[u16LaserNbr].mf64VertCorr);
  mCorr[u16LaserNbr].mf64CosVertCorr = cos(mCorr[u16LaserNbr].mf64VertCorr);
}

void VelodyneCalibration::setDistCorr(uint16_t u16LaserNbr, double mf64Value)
  throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setDistCorr: Out of bound");
  mCorr[u16LaserNbr].mf64DistCorr = mf64Value;
}

void VelodyneCalibration::setVertOffsCorr(uint16_t u16LaserNbr,
  double mf64Value) throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setVertOffsCorr: Out of bound");
  mCorr[u16LaserNbr].mf64VertOffsCorr = mf64Value;
}

void VelodyneCalibration::setHorizOffsCorr(uint16_t u16LaserNbr,
  double mf64Value) throw(OutOfBoundException) {
  if (u16LaserNbr > mcu16LasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setHorizOffsCorr: Out of bound");
  mCorr[u16LaserNbr].mf64HorizOffsCorr = mf64Value;
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
