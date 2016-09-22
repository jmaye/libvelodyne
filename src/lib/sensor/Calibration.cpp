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

#include <string>

#include "exceptions/IOException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

Calibration::Calibration(size_t numLasers) :
    mNumLasers(numLasers) {
  mCorr = new LaserCorrection[numLasers];
}

Calibration::Calibration(const Calibration& other) :
    Serializable(),
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
    delete [] mCorr;
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

void Calibration::read(std::istream& /*stream*/) {
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
    float value;
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

  const std::string fieldSep("\t\t");
  stream.precision(8);
  stream.width(0);
  stream.unsetf(std::ios::fixed |std::ios::scientific);

  for (size_t i = 0; i < mNumLasers; ++i) {
    stream << "id" << fieldSep << i << std::endl
           << "rotCorrection" << fieldSep << rad2deg(mCorr[i].mRotCorr) << std::endl
           << "vertCorrection" << fieldSep << rad2deg(mCorr[i].mVertCorr) << std::endl
           << "distCorrection" << fieldSep << mCorr[i].mDistCorr << std::endl
           << "vertOffsetCorrection" << fieldSep << mCorr[i].mVertOffsCorr << std::endl
           << "horizOffsetCorrection" << fieldSep << mCorr[i].mHorizOffsCorr << std::endl
           ;
  }
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

void Calibration::setRotCorr(size_t laserNbr, float value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setRotCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mRotCorr = value;
  mCorr[laserNbr].mSinRotCorr = sin(mCorr[laserNbr].mRotCorr);
  mCorr[laserNbr].mCosRotCorr = cos(mCorr[laserNbr].mRotCorr);
}

void Calibration::setVertCorr(size_t laserNbr, float value) {
  if (laserNbr > mNumLasers)
    throw OutOfBoundException<size_t>(laserNbr,
      "Calibration::setVertCorr(): Out of bound",
      __FILE__, __LINE__);
  mCorr[laserNbr].mVertCorr = value;
  mCorr[laserNbr].mSinVertCorr = sin(mCorr[laserNbr].mVertCorr);
  mCorr[laserNbr].mCosVertCorr = cos(mCorr[laserNbr].mVertCorr);
}
