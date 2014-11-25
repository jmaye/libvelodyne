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

/** \file Converter.h
    \brief This file defines utilities to convert Velodyne data packets to point
           clouds or scan clouds.
  */

#ifndef CONVERTER_H
#define CONVERTER_H

#include <cmath>

#include "sensor/DataPacket.h"
#include "sensor/Calibration.h"
#include "data-structures/VdynePointCloud.h"
#include "data-structures/VdyneScanCloud.h"

/** The Converter namespace contains utilities to convert Velodyne data packets
     to point clouds or scan clouds.
    \brief Velodyne data packets converter
  */
namespace Converter {
  /** \name Constants
    @{
    */
  /// Minimum distance for representing points
  static const float mMinDistance = 0.9;
  /// Maximum distance for representing points
  static const float mMaxDistance = 120.0;
  /// Conversion in meters
  static const size_t mMeterConversion = 100;
  /** @}
    */

  /** \name Methods
    @{
    */
  /// The toPointCloud function converts a data packet into a point cloud
  void toPointCloud(const DataPacket& dataPacket, const Calibration&
    calibration, VdynePointCloud& pointCloud, float minDistance =
    Converter::mMinDistance, float maxDistance = Converter::mMaxDistance);
  /// The toScanCloud function converts a data packet into a scan cloud
  void toScanCloud(const DataPacket& dataPacket, const Calibration&
    calibration, VdyneScanCloud& scanCloud, float minDistance =
    Converter::mMinDistance, float maxDistance = Converter::mMaxDistance);
  /// Normalize an angle positive
  inline float normalizeAnglePositive(float angle) {
    return std::fmod(std::fmod(angle, 2.0 * M_PI) + 2.0 * M_PI, 2.0 * M_PI);
  }
  /// Normalize an angle
  float normalizeAngle(float angle);
  /** @}
    */

};


#endif // CONVERTER_H
