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

/** \file VelodyneScanCloud.h
    \brief This file defines the VelodyneScanCloud class, which represents a
           Velodyne scan cloud
  */

#ifndef VELODYNESCANCLOUD_H
#define VELODYNESCANCLOUD_H

#include "sensor/VelodynePacket.h"
#include "sensor/VelodyneCalibration.h"

#include <vector>
#include <iosfwd>

/** The struct Scan represents a Velodyne scan.
    \brief Velodyne scan
  */
struct Scan {
  /// Range
  double mRange;
  /// Heading angle
  double mHeading;
  /// Pitch angle
  double mPitch;
  /// Intensity
  uint8_t mIntensity;
  /// Default constructor
  Scan() :
    mRange(0),
    mHeading(0),
    mPitch(0),
    mIntensity(0) {
  }
  /// Copy constructor
  Scan(const Scan& other) :
    mRange(other.mRange),
    mHeading(other.mHeading),
    mPitch(other.mPitch),
    mIntensity(other.mIntensity) {
  }
  /// Assignment operator
  Scan& operator = (const Scan& other) {
    if (this != &other) {
      mRange = other.mRange;
      mHeading = other.mHeading;
      mPitch = other.mPitch;
      mIntensity = other.mIntensity;
    }
    return *this;
  }
};

/** The class VelodyneScanCloud represents a Velodyne scan cloud.
    \brief Velodyne scan cloud
  */
class VelodyneScanCloud {
  /** \name Stream methods
    @{
    */
  /// Writes to ostream
  friend std::ostream& operator << (std::ostream& stream,
    const VelodyneScanCloud& obj);
  /// Reads from istream
  friend std::istream& operator >> (std::istream& stream,
    VelodyneScanCloud& obj);
  /** @}
    */

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodyneScanCloud(const VelodyneScanCloud& other);
  /// Assignment operator
  VelodyneScanCloud& operator = (const VelodyneScanCloud& other);
  /** @}
    */

  /** \name Constants
    @{
    */
  /// Minimum distance for representing points
  static const double mMinDistance = 0.9;
  /// Maximum distance for representing points
  static const double mMaxDistance = 120.0;
  /// Conversion in meters
  static const size_t mMeterConversion = 100;
  /** @}
    */

  /** \name Stream method
    @{
    */
  /// Reads from formatted input
  void readFormatted(std::istream& stream);
  /// Writes to formatted output
  void writeFormatted(std::ostream& stream) const;
  /** @}
    */

  /** \name Private methods
    @{
    */
  /// Normalize the angle positive
  double normalizeAnglePositive(double angle) const;
  /// Normalize the angle
  double normalizeAngle(double angle) const;
  /** @}
    */

  /** \name Private members
    @{
    */
  /// Timestamp of the cloud
  double mTimestamp;
  /// Points in the cloud
  std::vector<Scan> mScanCloudVector;
  /// Start angle of the cloud
  double mStartRotationAngle;
  /// End angle of the cloud
  double mEndRotationAngle;
  /** @}
    */

public:
  /** \name Constructors/Destructor
    @{
    */
  /// Default constructor
  VelodyneScanCloud();
  /// Constructs from packet
  VelodyneScanCloud(const VelodynePacket& vdynePacket,
    const VelodyneCalibration& vdyneCalibration,
    double minDistance = VelodyneScanCloud::mMinDistance,
    double maxDistance = VelodyneScanCloud::mMaxDistance);
  /// Destructor
  ~VelodyneScanCloud();

  /** \name Accessors
    @{
    */
  /// Returns the timestamp
  double getTimestamp() const;
  /// Returns the starting rotational angle
  double getStartRotationAngle() const;
  /// Returns the ending rotational angle
  double getEndRotationAngle() const;
  /// Returns starting iterator on the vector
  std::vector<Scan>::const_iterator getStartIterator() const;
  /// Returns end iterator on the cloud
  std::vector<Scan>::const_iterator getEndIterator() const;
  /// Returns the size of the cloud
  size_t getSize() const;
  /// Sets the timestamp
  void setTimestamp(double timestamp);
  /// Sets the starting rotational angle
  void setStartRotationAngle(double angle);
  /// Sets the ending rotational angle
  void setEndRotationAngle(double angle);
  /// Push a scan in the cloud
  void pushScan(const Scan& scan);
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Reads from istream
  void read(std::istream& stream);
  /// Writes to ostream
  void write(std::ostream& stream) const;
  /** @}
    */

protected:

};

#endif // VELODYNESCANCLOUD_H
