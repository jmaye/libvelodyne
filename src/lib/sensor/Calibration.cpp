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

#include "sensor/Calibration.h"

#include <cmath>

#include <string>

#include "exceptions/OutOfBoundException.h"
#include "exceptions/IOException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

Calibration::Calibration(size_t numLasers) :
    mNumLasers(numLasers) {
  mCorr = new LaserCorrection[numLasers];
}

Calibration::Calibration(const Calibration& other) :
    mNumLasers(other.mNumLasers) {
  mCorr = new LaserCorrection[mNumLasers];
  for (size_t i = 0; i < mNumLasers; ++i)
    mCorr[i] = other.mCorr[i];
}

Calibration& Calibration::operator = (const Calibration& other) {
  if (this != &other) {
    mNumLasers = other.mNumLasers;
    mCorr = new LaserCorrection[mNumLasers];
    for (size_t i = 0; i < mNumLasers; ++i)
      mCorr[i] = other.mCorr[i];
  }
  return *this;
}

Calibration::~Calibration() {
  if (mCorr)
    delete mCorr;
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void Calibration::read(std::istream& stream) {
}

void Calibration::write(std::ostream& stream) const {
  for (size_t i = 0; i < mNumLasers; ++i) {
    stream << "id " << i << std::endl
           << "rotCorrection " << rad2deg(mCorr[i].mRotCorr) << std::endl
           << "vertCorrection " << rad2deg(mCorr[i].mVertCorr) << std::endl
           << "distCorrection " << mCorr[i].mDistCorr << std::endl
           << "vertOffsetCorrection " << mCorr[i].mVertOffsCorr << std::endl
           << "horizOffsetCorrection " << mCorr[i].mHorizOffsCorr << std::endl
           << std::endl;
  }
}

void Calibration::read(std::ifstream& stream) {
  if (!stream.is_open())
    throw IOException("Calibration::read(): could not open file");
  for (size_t i = 0; i < mNumLasers; ++i) {
    std::string strKey;
    double value;
    stream >> strKey;
    if (strKey.compare("id") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    if ((size_t)value != i)
      throw IOException("Calibration::read(): Unexcepted id");
    stream >> strKey;
    if (strKey.compare("rotCorrection") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mRotCorr = deg2rad(value);
    mCorr[i].mSinRotCorr = sin(mCorr[i].mRotCorr);
    mCorr[i].mCosRotCorr = cos(mCorr[i].mRotCorr);
    stream >> strKey;
    if (strKey.compare("vertCorrection") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mVertCorr = deg2rad(value);
    mCorr[i].mSinVertCorr = sin(mCorr[i].mVertCorr);
    mCorr[i].mCosVertCorr = cos(mCorr[i].mVertCorr);
    stream >> strKey;
    if (strKey.compare("distCorrection") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mDistCorr = value;
    stream >> strKey;
    if (strKey.compare("vertOffsetCorrection") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mVertOffsCorr = value;
    stream >> strKey;
    if (strKey.compare("horizOffsetCorrection") != 0)
      throw IOException("Calibration::read(): Unexcepted key");
    stream >> value;
    mCorr[i].mHorizOffsCorr = value;
  }
}

void Calibration::write(std::ofstream& stream) const {
  if (!stream.is_open())
    throw IOException("Calibration::write(): could not open file");
  for (size_t i = 0; i < mNumLasers; ++i) {
    stream << "id " << i << std::endl
           << "rotCorrection " << rad2deg(mCorr[i].mRotCorr) << std::endl
           << "vertCorrection " << rad2deg(mCorr[i].mVertCorr) << std::endl
           << "distCorrection " << mCorr[i].mDistCorr << std::endl
           << "vertOffsetCorrection " << mCorr[i].mVertOffsCorr << std::endl
           << "horizOffsetCorrection " << mCorr[i].mHorizOffsCorr << std::endl
           << std::endl;
  }
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

size_t Calibration::getNumLasers() const {
  return mNumLasers;
}

double Calibration::getRotCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getRotCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mRotCorr;
}

double Calibration::getSinRotCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getSinRotCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mSinRotCorr;
}

double Calibration::getCosRotCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getCosRotCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mCosRotCorr;
}

double Calibration::getVertCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getVertCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mVertCorr;
}

double Calibration::getSinVertCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getSinVertCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mSinVertCorr;
}

double Calibration::getCosVertCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getCosVertCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mCosVertCorr;
}

double Calibration::getDistCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getDistCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mDistCorr;
}

double Calibration::getVertOffsCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getVertOffsCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mVertOffsCorr;
}

double Calibration::getHorizOffsCorr(size_t laserNbr) const {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::getHorizOffsCorr(): Out of bound",
      __FILE__, __LINE__);
  return mCorr[laserNbr].mHorizOffsCorr;
}

void Calibration::setRotCorr(size_t laserNbr, double value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setRotCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mRotCorr = deg2rad(value);
  mCorr[laserNbr].mSinRotCorr = sin(mCorr[laserNbr].mRotCorr);
  mCorr[laserNbr].mCosRotCorr = cos(mCorr[laserNbr].mRotCorr);
}

void Calibration::setVertCorr(size_t laserNbr, double value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setVertCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mVertCorr = deg2rad(value);
  mCorr[laserNbr].mSinVertCorr = sin(mCorr[laserNbr].mVertCorr);
  mCorr[laserNbr].mCosVertCorr = cos(mCorr[laserNbr].mVertCorr);
}

void Calibration::setDistCorr(size_t laserNbr, double value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setDistCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mDistCorr = value;
}

void Calibration::setVertOffsCorr(size_t laserNbr, double value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setVertOffsCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mVertOffsCorr = value;
}

void Calibration::setHorizOffsCorr(size_t laserNbr, double value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setHorizOffsCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mHorizOffsCorr = value;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

double Calibration::deg2rad(double deg) {
  return deg * M_PI / 180.0;
}

double Calibration::rad2deg(double rad) {
  return rad * 180.0 / M_PI;
}
