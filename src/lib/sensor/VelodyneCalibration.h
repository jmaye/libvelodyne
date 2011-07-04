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

/** \file VelodyneCalibration.h
    \brief This file defines the VelodyneCalibration class, which represents the
           calibration structure for the Velodyne
  */

#ifndef VELODYNECALIBRATION_H
#define VELODYNECALIBRATION_H

#include "exceptions/IOException.h"
#include "exceptions/OutOfBoundException.h"

#include <iosfwd>

/** The class VelodyneCalibration represents the calibration structure for the
    Velodyne.
    \brief Velodyne calibration
  */
class VelodyneCalibration {
  /** \name Stream methods
    @{
    */
  /// Read interface to std::ostream
  friend std::ostream& operator << (std::ostream& stream,
    const VelodyneCalibration& obj);
  /// Write interface to std::ostream
  friend std::istream& operator >> (std::istream& stream,
    VelodyneCalibration& obj);
  /** @}
    */

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodyneCalibration(const VelodyneCalibration& other);
  /// Assignment operator
  VelodyneCalibration& operator = (const VelodyneCalibration& other);
  /** @}
    */

  /** \name Stream methods
    @{
    */
  /// Read formatted data from istream
  void readFormatted(std::istream& stream) throw (IOException);
  /// Write formatted data to ostream
  void writeFormatted(std::ostream& stream) const;
  /** @}
    */

  /// Number of lasers in the Velodyne
  static const size_t mLasersNbr = 64;

  /// The VelodyneLaserCorrection struct defines the laser correction.
  /// \brief Laser correction
  struct VelodyneLaserCorrection {
    /// Rotation correction
    double mRotCorr;
    /// Sinus of rotation correction
    double mSinRotCorr;
    /// Cosinus of rotation correction
    double mCosRotCorr;
    /// Vertical correction
    double mVertCorr;
    /// Sinus of vertical correction
    double mSinVertCorr;
    /// Cosinus of vertical correction
    double mCosVertCorr;
    /// Distortion correction
    double mDistCorr;
    /// Vertical offset correction
    double mVertOffsCorr;
    /// Horizontal offset correction
    double mHorizOffsCorr;
    /// Default constructor
    VelodyneLaserCorrection() :
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
    VelodyneLaserCorrection(const VelodyneLaserCorrection& other) :
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
    VelodyneLaserCorrection& operator = (const VelodyneLaserCorrection& other) {
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

  /** \name Private members
    @{
    */
  /// Laser corrections
  VelodyneLaserCorrection mCorr[mLasersNbr];
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Default constructor
  VelodyneCalibration();
  /// Destructor
  ~VelodyneCalibration();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the rotation correction
  double getRotCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the sinus of rotation correction
  double getSinRotCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the cosinus of rotation correction
  double getCosRotCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the vertical correction
  double getVertCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the sinus of vertical correction
  double getSinVertCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the cosinus of vertical correction
  double getCosVertCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the distortion correction
  double getDistCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the vertical offset correction
  double getVertOffsCorr(size_t laserNbr) const throw(OutOfBoundException);
  /// Returns the horizontal offset correction
  double getHorizOffsCorr(size_t laserNbr) const throw(OutOfBoundException);

  /// Sets the rotation correction
  void setRotCorr(size_t laserNbr, double value) throw(OutOfBoundException);
  /// Sets the vertical correction
  void setVertCorr(size_t laserNbr, double value) throw(OutOfBoundException);
  /// Sets the distortion correction
  void setDistCorr(size_t laserNbr, double value) throw(OutOfBoundException);
  /// Sets the vertical offset correction
  void setVertOffsCorr(size_t laserNbr, double value)
    throw(OutOfBoundException);
  /// Sets the horizontal offset correction
  void setHorizOffsCorr(size_t laserNbr, double value)
    throw(OutOfBoundException);
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Converts degree to radian
  double deg2rad(double deg) const;
  /// Converts radian to degree
  double rad2deg(double rad) const;
  /** @}
    */

  /** \name Stream methods
    @{
    */
  /// Reads from stream
  void read(std::istream& stream);
  /// Writes to stream
  void write(std::ostream& stream) const;
  /** @}
    */

protected:

};

#endif // VELODYNECALIBRATION_H
