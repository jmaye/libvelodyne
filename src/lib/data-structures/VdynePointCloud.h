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

/** \file VdynePointCloud.h
    \brief This file defines the VdynePointCloud class, which represents a
           Velodyne point cloud
  */

#ifndef VDYNEPOINTCLOUD_H
#define VDYNEPOINTCLOUD_H

#include <cstdint>

#include <vector>

#include "base/Serializable.h"
#include "base/BinaryStreamReader.h"
#include "base/BinaryStreamWriter.h"

/** The class VdynePointCloud represents a Velodyne point cloud.
    \brief Velodyne point cloud
  */
class VdynePointCloud :
  public Serializable {
public:
  /** \name Types definitions
    @{
    */
  /// The struct Point3D represents a 3D point.
  struct Point3D :
    public Serializable {
    /// X coordinate
    float mX;
    /// Y coordinate
    float mY;
    /// Z coordinate
    float mZ;
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
        Serializable(),
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
    /// Reads from standard input
    virtual void read(std::istream& /*stream*/) {
    }
    /// Writes to standard output
    virtual void write(std::ostream& stream) const {
      stream << mX << " " << mY<< " " << mZ << " " << mIntensity;
    }
    /// Reads from a file
    virtual void read(std::ifstream& /*stream*/) {
    }
    /// Writes to a file
    virtual void write(std::ofstream& /*stream*/) const {
    }
    /// Writes binary into a output stream
    void writeBinary(std::ostream& stream) const {
      BinaryStreamWriter<std::ostream> binaryStream(stream);
      binaryStream << mX << mY << mZ << mIntensity;
    }
    /// Reads binary from an input stream
    void readBinary(std::istream& stream) {
      BinaryStreamReader<std::istream> binaryStream(stream);
      binaryStream >> mX >> mY >> mZ >> mIntensity;
    }
  };
  /// Container type
  typedef std::vector<Point3D> Container;
  /// Constant iterator type
  typedef Container::const_iterator ConstPointIterator;
  /// Iterator type
  typedef Container::iterator PointIterator;
  /** @}
    */

  /** \name Constructors/Destructor
    @{
    */
  /// Default constructor
  VdynePointCloud();
  /// Copy constructor
  VdynePointCloud(const VdynePointCloud& other);
  /// Assignment operator
  VdynePointCloud& operator = (const VdynePointCloud& other);
  // Destructor
  ~VdynePointCloud();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the timestamp
  int64_t getTimestamp() const;
  /// Sets the timestamp
  void setTimestamp(int64_t timestamp);
  /// Returns the starting rotational angle
  float getStartRotationAngle() const;
  /// Sets the starting rotational angle
  void setStartRotationAngle(float angle);
  /// Returns the ending rotational angle
  float getEndRotationAngle() const;
  /// Sets the ending rotational angle
  void setEndRotationAngle(float angle);
  /// Returns the container
  const Container& getPoints() const;
  /// Returns iterator at start of the container
  ConstPointIterator getPointBegin() const;
  /// Returns iterator at start of the container
  PointIterator getPointBegin();
  /// Returns iterator at end of the container
  ConstPointIterator getPointEnd() const;
  /// Returns iterator at end of the container
  PointIterator getPointEnd();
  /// Returns the size of the cloud
  size_t getSize() const;
  /// Inserts a point into the point cloud
  void insertPoint(const Point3D& point);
  /// Clear the point cloud
  void clear();
  /** @}
    */

  /** \name Methods
      @{
    */
  /// Writes into a output stream
  void writeBinary(std::ostream& stream) const;
  /// Reads from an input stream
  void readBinary(std::istream& stream);
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
  /// Timestamp of the cloud
  int64_t mTimestamp;
  /// Points in the cloud
  Container mPoints;
  /// Start angle of the cloud
  float mStartRotationAngle;
  /// End angle of the cloud
  float mEndRotationAngle;
  /** @}
    */

};

#endif // VDYNEPOINTCLOUD_H
