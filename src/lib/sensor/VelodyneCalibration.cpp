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

#include "sensor/VelodyneCalibration.h"

#include <iostream>
#include <fstream>

#include <cmath>

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodyneCalibration::VelodyneCalibration() {
}

VelodyneCalibration::VelodyneCalibration(const VelodyneCalibration& other) {
  for (size_t i = 0; i < mLasersNbr; i++) {
    mCorr[i] = other.mCorr[i];
  }
}

VelodyneCalibration& VelodyneCalibration::operator =
  (const VelodyneCalibration& other) {
  for (size_t i = 0; i < mLasersNbr; i++) {
    mCorr[i] = other.mCorr[i];
  }
  return *this;
}

VelodyneCalibration::~VelodyneCalibration() {
}

/******************************************************************************/
/* Stream methods                                                             */
/******************************************************************************/

void VelodyneCalibration::read(std::istream& stream) {
}

void VelodyneCalibration::write(std::ostream& stream) const {
}

void VelodyneCalibration::readFormatted(std::istream& stream)
  throw (IOException) {
  for (size_t i = 0; i < mLasersNbr; i++) {
    std::string strKey;
    double value;
    stream >> strKey;
    if (strKey.compare("id") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    if ((size_t)value != i)
      throw IOException("VelodyneCalibration::read(): Unexcepted id");
    stream >> strKey;
    if (strKey.compare("rotCorrection") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mRotCorr = deg2rad(value);
    mCorr[i].mSinRotCorr = sin(mCorr[i].mRotCorr);
    mCorr[i].mCosRotCorr = cos(mCorr[i].mRotCorr);
    stream >> strKey;
    if (strKey.compare("vertCorrection") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mVertCorr = deg2rad(value);
    mCorr[i].mSinVertCorr = sin(mCorr[i].mVertCorr);
    mCorr[i].mCosVertCorr = cos(mCorr[i].mVertCorr);
    stream >> strKey;
    if (strKey.compare("distCorrection") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mDistCorr = value;
    stream >> strKey;
    if (strKey.compare("vertOffsetCorrection") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mVertOffsCorr = value;
    stream >> strKey;
    if (strKey.compare("horizOffsetCorrection") != 0)
      throw IOException("VelodyneCalibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mHorizOffsCorr = value;
  }
}

void VelodyneCalibration::writeFormatted(std::ostream& stream) const {
  for (size_t i = 0; i < mLasersNbr; i++) {
    stream << "id " << i << std::endl
           << "rotCorrection " << rad2deg(mCorr[i].mRotCorr) << std::endl
           << "vertCorrection " << rad2deg(mCorr[i].mVertCorr) << std::endl
           << "distCorrection " << mCorr[i].mDistCorr << std::endl
           << "vertOffsetCorrection " << mCorr[i].mVertOffsCorr << std::endl
           << "horizOffsetCorrection " << mCorr[i].mHorizOffsCorr << std::endl
           << std::endl;
  }
}

double VelodyneCalibration::deg2rad(double deg) const {
  return deg * M_PI / 180.0;
}

double VelodyneCalibration::rad2deg(double rad) const {
  return rad * 180.0 / M_PI;
}

double VelodyneCalibration::getRotCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getRotCorr(): Out of bound");
  return mCorr[laserNbr].mRotCorr;
}

double VelodyneCalibration::getSinRotCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getSinRotCorr(): Out of bound");
  return mCorr[laserNbr].mSinRotCorr;
}

double VelodyneCalibration::getCosRotCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getCosRotCorr(): Out of bound");
  return mCorr[laserNbr].mCosRotCorr;
}

double VelodyneCalibration::getVertCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getVertCorr(): Out of bound");
  return mCorr[laserNbr].mVertCorr;
}

double VelodyneCalibration::getSinVertCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getSinVertCorr(): Out of bound");
  return mCorr[laserNbr].mSinVertCorr;
}

double VelodyneCalibration::getCosVertCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getCosVertCorr(): Out of bound");
  return mCorr[laserNbr].mCosVertCorr;
}

double VelodyneCalibration::getDistCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getDistCorr(): Out of bound");
  return mCorr[laserNbr].mDistCorr;
}

double VelodyneCalibration::getVertOffsCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getVertOffsCorr(): Out of bound");
  return mCorr[laserNbr].mVertOffsCorr;
}

double VelodyneCalibration::getHorizOffsCorr(size_t laserNbr) const
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::getHorizOffsCorr(): Out of bound");
  return mCorr[laserNbr].mHorizOffsCorr;
}

void VelodyneCalibration::setRotCorr(size_t laserNbr, double value)
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setRotCorr(): Out of bound");
  mCorr[laserNbr].mRotCorr = deg2rad(value);
  mCorr[laserNbr].mSinRotCorr = sin(mCorr[laserNbr].mRotCorr);
  mCorr[laserNbr].mCosRotCorr = cos(mCorr[laserNbr].mRotCorr);
}

void VelodyneCalibration::setVertCorr(size_t laserNbr, double value)
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setVertCorr(): Out of bound");
  mCorr[laserNbr].mVertCorr = deg2rad(value);
  mCorr[laserNbr].mSinVertCorr = sin(mCorr[laserNbr].mVertCorr);
  mCorr[laserNbr].mCosVertCorr = cos(mCorr[laserNbr].mVertCorr);
}

void VelodyneCalibration::setDistCorr(size_t laserNbr, double value)
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setDistCorr(): Out of bound");
  mCorr[laserNbr].mDistCorr = value;
}

void VelodyneCalibration::setVertOffsCorr(size_t laserNbr, double value)
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setVertOffsCorr(): Out of bound");
  mCorr[laserNbr].mVertOffsCorr = value;
}

void VelodyneCalibration::setHorizOffsCorr(size_t laserNbr, double value)
  throw (OutOfBoundException) {
  if (laserNbr > mLasersNbr)
    throw OutOfBoundException("VelodyneCalibration::setHorizOffsCorr(): Out of bound");
  mCorr[laserNbr].mHorizOffsCorr = value;
}

std::ostream& operator << (std::ostream& stream, const VelodyneCalibration&
  obj) {
  obj.writeFormatted(stream);
  return stream;
}

std::istream& operator >> (std::istream& stream, VelodyneCalibration& obj) {
  obj.readFormatted(stream);
  return stream;
}
