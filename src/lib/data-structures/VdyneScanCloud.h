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

/** \file VdyneScanCloud.h
    \brief This file defines the VdyneScanCloud class, which represents a
           Velodyne scan cloud
  */

#ifndef VDYNESCANCLOUD_H
#define VDYNESCANCLOUD_H

#include <stdint.h>

#include <vector>

#include "base/Serializable.h"
#include "base/BinaryStreamReader.h"
#include "base/BinaryStreamWriter.h"

/** The class VdyneScanCloud represents a Velodyne scan cloud.
    \brief Velodyne scan cloud
  */
class VdyneScanCloud :
  public Serializable {
public:
  /** \name Types definitions
    @{
    */
  /// The struct Scan represents a Velodyne scan.
  struct Scan :
    public Serializable {
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
    /// Reads from standard input
    virtual void read(std::istream& stream) {
    }
    /// Writes to standard output
    virtual void write(std::ostream& stream) const {
      stream << mRange << " " << mHeading<< " " << mPitch << " "
         << mIntensity;
    }
    /// Reads from a file
    virtual void read(std::ifstream& stream) {
    }
    /// Writes to a file
    virtual void write(std::ofstream& stream) const {
    }
    /// Writes binary into a output stream
    void writeBinary(std::ostream& stream) const {
      BinaryStreamWriter<std::ostream> binaryStream(stream);
      binaryStream << mRange << mHeading << mPitch << mIntensity;
    }
    /// Reads binary from an input stream
    void readBinary(std::istream& stream) {
      BinaryStreamReader<std::istream> binaryStream(stream);
      binaryStream >> mRange >> mHeading >> mPitch >> mIntensity;
    }
  };
  /// Container type
  typedef std::vector<Scan> Container;
  /// Constant iterator type
  typedef Container::const_iterator ConstScanIterator;
  /// Iterator type
  typedef Container::iterator ScanIterator;
  /** @}
    */

  /** \name Constructors/Destructor
    @{
    */
  /// Default constructor
  VdyneScanCloud();
  /// Copy constructor
  VdyneScanCloud(const VdyneScanCloud& other);
  /// Assignment operator
  VdyneScanCloud& operator = (const VdyneScanCloud& other);
  /// Destructor
  ~VdyneScanCloud();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the timestamp
  double getTimestamp() const;
  /// Sets the timestamp
  void setTimestamp(double timestamp);
  /// Returns the starting rotational angle
  double getStartRotationAngle() const;
  /// Sets the starting rotational angle
  void setStartRotationAngle(double angle);
  /// Returns the ending rotational angle
  double getEndRotationAngle() const;
  /// Sets the ending rotational angle
  void setEndRotationAngle(double angle);
  /// Returns the container
  const Container& getScans() const;
  /// Returns iterator at start of the container
  ConstScanIterator getScanBegin() const;
  /// Returns iterator at start of the container
  ScanIterator getScanBegin();
  /// Returns iterator at end of the container
  ConstScanIterator getScanEnd() const;
  /// Returns iterator at end of the container
  ScanIterator getScanEnd();
  /// Returns the size of the cloud
  size_t getSize() const;
  /// Inserts a scan into the scan cloud
  void insertScan(const Scan& scan);
  /// Clear the scan cloud
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
  double mTimestamp;
  /// Scans in the cloud
  Container mScans;
  /// Start angle of the cloud
  double mStartRotationAngle;
  /// End angle of the cloud
  double mEndRotationAngle;
  /** @}
    */

};

#endif // VDYNESCANCLOUD_H
