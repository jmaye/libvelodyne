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

/** \file Calibration.h
    \brief This file defines the Calibration class, which represents the
           calibration structure for the Velodyne
  */

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <cmath>

#include "base/Serializable.h"

#include "exceptions/OutOfBoundException.h"

/** The class Calibration represents the calibration structure for the
    Velodyne.
    \brief Velodyne calibration
  */
class Calibration :
  public Serializable {
public:
  /** \name Types definitions
    @{
    */
  /// The LaserCorrection struct defines the laser correction.
  struct LaserCorrection {
    /// Rotation correction
    float mRotCorr;
    /// Sinus of rotation correction
    float mSinRotCorr;
    /// Cosinus of rotation correction
    float mCosRotCorr;
    /// Vertical correction
    float mVertCorr;
    /// Sinus of vertical correction
    float mSinVertCorr;
    /// Cosinus of vertical correction
    float mCosVertCorr;
    /// Distortion correction
    float mDistCorr;
    /// Vertical offset correction
    float mVertOffsCorr;
    /// Horizontal offset correction
    float mHorizOffsCorr;
    /// Default constructor
    LaserCorrection() :
        mRotCorr(0),
        mSinRotCorr(0),
        mCosRotCorr(0),
        mVertCorr(0),
        mSinVertCorr(0),
        mCosVertCorr(0),
        mDistCorr(0),
        mVertOffsCorr(0),
        mHorizOffsCorr(0) {
    }
    /// Copy constructor
    LaserCorrection(const LaserCorrection& other) :
        mRotCorr(other.mRotCorr),
        mSinRotCorr(other.mSinRotCorr),
        mCosRotCorr(other.mCosRotCorr),
        mVertCorr(other.mVertCorr),
        mSinVertCorr(other.mSinVertCorr),
        mCosVertCorr(other.mCosVertCorr),
        mDistCorr(other.mDistCorr),
        mVertOffsCorr(other.mVertOffsCorr),
        mHorizOffsCorr(other.mHorizOffsCorr) {
    }
    // Assignment operator
    LaserCorrection& operator = (const LaserCorrection& other) {
      if (this != &other) {
        mRotCorr = other.mRotCorr;
        mSinRotCorr = other.mSinRotCorr;
        mCosRotCorr =other.mCosRotCorr;
        mVertCorr = other.mVertCorr;
        mSinVertCorr = other.mSinVertCorr;
        mCosVertCorr = other.mCosVertCorr;
        mDistCorr =other.mDistCorr;
        mVertOffsCorr = other.mVertOffsCorr;
        mHorizOffsCorr = other.mHorizOffsCorr;
      }
      return *this;
    }
  };
  /** @}
    */

  /** \name Constructors/destructor
    @{
    */
  /// Default constructor
  Calibration(size_t numLasers = 64);
  /// Copy constructor
  Calibration(const Calibration& other);
  /// Assignment operator
  Calibration& operator = (const Calibration& other);
  /// Destructor
  virtual ~Calibration();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the number of lasers
  size_t getNumLasers() const {
    return mNumLasers;
  }
  /// Returns the rotation correction
  float getRotCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getRotCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mRotCorr;
  }
  /// Sets the rotation correction
  void setRotCorr(size_t laserNbr, float value);
  /// Returns the sinus of rotation correction
  float getSinRotCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getSinRotCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mSinRotCorr;
  }
  /// Returns the cosinus of rotation correction
  float getCosRotCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getCosRotCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mCosRotCorr;
  }
  /// Returns the vertical correction
  float getVertCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getVertCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mVertCorr;
  }
  /// Sets the vertical correction
  void setVertCorr(size_t laserNbr, float value);
  /// Returns the sinus of vertical correction
  float getSinVertCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getSinVertCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mSinVertCorr;
  }
  /// Returns the cosinus of vertical correction
  float getCosVertCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getCosVertCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mCosVertCorr;
  }
  /// Returns the distortion correction
  float getDistCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getDistCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mDistCorr;
  }
  /// Sets the distortion correction
  void setDistCorr(size_t laserNbr, float value) {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::setDistCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    mCorr[laserNbr].mDistCorr = value;
  }
  /// Returns the vertical offset correction
  float getVertOffsCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getVertOffsCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mVertOffsCorr;
  }
  /// Sets the vertical offset correction
  void setVertOffsCorr(size_t laserNbr, float value) {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::setVertOffsCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    mCorr[laserNbr].mVertOffsCorr = value;
  }
  /// Returns the horizontal offset correction
  float getHorizOffsCorr(size_t laserNbr) const {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::getHorizOffsCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    return mCorr[laserNbr].mHorizOffsCorr;
  }
  /// Sets the horizontal offset correction
  void setHorizOffsCorr(size_t laserNbr, float value) {
#ifndef NDEBUG
    if (laserNbr > mNumLasers)
      throw OutOfBoundException<size_t>(laserNbr,
        "Calibration::setHorizOffsCorr(): Out of bound",
        __FILE__, __LINE__);
#endif
    mCorr[laserNbr].mHorizOffsCorr = value;
  }
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Converts degree to radian
  static float deg2rad(float deg) {
    return deg * M_PI / 180.0;
  }
  /// Converts radian to degree
  static float rad2deg(float rad) {
    return rad * 180.0 / M_PI;
  }
  /** @}
    */

protected:
  /** \name Stream methods
    @{
    */
  /// Reads from standard input
  virtual void read(std::istream& stream);
  /// Writes to standard output
  virtual void write(std::ostream& stream) const;
  /// Reads from a file
  virtual void read(std::ifstream& stream);
  /// Writes to a file
  virtual void write(std::ofstream& stream) const;
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Number of lasers in the Velodyne
  size_t mNumLasers;
  /// Laser corrections
  LaserCorrection* mCorr;
  /** @}
    */

};

#endif // CALIBRATION_H
