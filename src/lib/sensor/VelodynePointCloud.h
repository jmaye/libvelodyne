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

/** \file VelodynePointCloud.h
    \brief This file defines the VelodynePointCloud class, which represents a
           Velodyne point cloud
  */

#ifndef VELODYNEPOINTCLOUD_H
#define VELODYNEPOINTCLOUD_H

#include "sensor/VelodynePacket.h"
#include "sensor/VelodyneCalibration.h"

#include <vector>
#include <iosfwd>

/** The struct Point3D represents a 3D point.
    \brief 3d point
  */
struct Point3D {
  /// X coordinate
  double mX;
  /// Y coordinate
  double mY;
  /// Z coordinate
  double mZ;
  /// Intensity
  uint8_t mIntensity;
  /// Default constructor
  Point3D() :
    mX(0),
    mY(0),
    mZ(0),
    mIntensity(0) {
  }
  /// Copy constructor
  Point3D(const Point3D& other) :
    mX(other.mX),
    mY(other.mY),
    mZ(other.mZ),
    mIntensity(other.mIntensity) {
  }
  /// Assignment operator
  Point3D& operator = (const Point3D& other) {
    if (this != &other) {
      mX = other.mX;
      mY = other.mY;
      mZ = other.mZ;
      mIntensity = other.mIntensity;
    }
    return *this;
  }
};

/** The class VelodynePointCloud3D represents a Velodyne point cloud.
    \brief Velodyne point cloud
  */
class VelodynePointCloud {
public:
  /** \name Types definitions
    @{
    */
  /// Point vector type
  typedef std::vector<Point3D> Point3DVector;
  /// Point vector iterator
  typedef Point3DVector::const_iterator Point3DVectorConstIterator;
  /** @}
    */

private:
  /** \name Stream methods
    @{
    */
  /// Writes to ostream
  friend std::ostream& operator << (std::ostream& stream,
    const VelodynePointCloud &obj);
  /// Reads from istream
  friend std::istream& operator >> (std::istream& stream,
    VelodynePointCloud& obj);
  /** @}
    */

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodynePointCloud(const VelodynePointCloud& other);
  /// Assignment operator
  VelodynePointCloud& operator = (const VelodynePointCloud& other);
  /** @}
    */

  /** \name Constants
    @{
    */
  /// Minimum distance for representing points
  static const double mMinDistance = 1.5;
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

  /** \name Private members
    @{
    */
  /// Timestamp of the cloud
  double mTimestamp;
  /// Points in the cloud
  Point3DVector mPointCloudVector;
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
  VelodynePointCloud();
  /// Constructs from packet
  VelodynePointCloud(const VelodynePacket& vdynePacket,
    const VelodyneCalibration& vdyneCalibration,
    double minDistance = VelodynePointCloud::mMinDistance,
    double maxDistance = VelodynePointCloud::mMaxDistance);
  // Destructor
  ~VelodynePointCloud();
  /** @}
    */

  /** \name Accessors
    @{
    */
  // Returns the timestamp
  double getTimestamp() const;
  // Returns the starting rotational angle
  double getStartRotationAngle() const;
  // Returns the ending rotational angle
  double getEndRotationAngle() const;
  // Returns starting iterator on the vector
  Point3DVectorConstIterator getStartIterator() const;
  // Returns end iterator on the cloud
  Point3DVectorConstIterator getEndIterator() const;
  // Returns the size of the cloud
  size_t getSize() const;
  // Sets the timestamp
  void setTimestamp(double timestamp);
  // Sets the starting rotational angle
  void setStartRotationAngle(double angle);
  // Sets the ending rotational angle
  void setEndRotationAngle(double angle);
  // Push a point in the cloud
  void pushPoint(const Point3D& point);
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

#endif // VELODYNEPOINTCLOUD_H
