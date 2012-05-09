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

/** \file SensorLiveControl.h
    \brief This file defines a Qt control for visualizing live Velodyne data.
  */

#ifndef SENSORLIVECONTROL_H
#define SENSORLIVECONTROL_H

#include <boost/shared_ptr.hpp>

#include <QtGui/QColor>
#include <QtCore/QTimer>
#include <QtCore/QString>

#include "visualization/Control.h"
#include "visualization/View3d.h"
#include "visualization/Scene3d.h"
#include "base/Singleton.h"
#include "sensor/Calibration.h"
#include "sensor/DataPacket.h"
#include "sensor/AcquisitionThread.h"
#include "data-structures/VdynePointCloud.h"

class Ui_SensorLiveControl;

/** The SensorLiveControl class represents a control for visualizing live
    Velodyne data.
    \brief Sensor control for live Velodyne data
  */
class SensorLiveControl :
  public Control,
  public Singleton<SensorLiveControl> {

Q_OBJECT

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  SensorLiveControl(const SensorLiveControl& other);
  /// Assignment operator
  SensorLiveControl& operator = (const SensorLiveControl& other);
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs control from parameters
  SensorLiveControl(AcquisitionThread<DataPacket>::Buffer& acqBuffer,
    bool showPoints = true);
  /// Destructor
  ~SensorLiveControl();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Sets the points color
  void setPointColor(const QColor& color);
  /// Sets the points size
  void setPointSize(double size);
  /// Shows the points
  void setShowPoints(bool showPoints);
  /// Smoothes the points
  void setSmoothPoints(bool smoothPoints);
  /// Sets the calibration filename
  void setCalibrationFilename(const QString& filename);
  /// Sets the min distance from the sensor
  void setMinDistance(double minDistance);
  /// Sets the max distance from the sensor
  void setMaxDistance(double maxDistance);
  /** @}
    */

protected:
  /** \name Protected methods
    @{
    */
  /// Render the points
  void renderPoints(double size, bool smooth);
  /// Read packet from the queue
  bool readPacket(boost::shared_ptr<DataPacket>& packet);
  /// Read point cloud
  bool readPointCloud();
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Qt user interface
  Ui_SensorLiveControl* mUi;
  /// Palette
  Palette mPalette;
  /// Timer
  QTimer mTimer;
  /// Velodyne calibration
  Calibration mCalibration;
  /// Min distance of the points
  double mMinDistance;
  /// Max distance of the points
  double mMaxDistance;
  /// Velodyne point cloud used for display
  VdynePointCloud mPointCloud;
  /// Velodyne point cloud used for acquiring
  VdynePointCloud mAcqPointCloud;
  /// Acquisition thread
  AcquisitionThread<DataPacket>::Buffer& mAcqBuffer;
  /** @}
    */

protected slots:
  /** \name Qt slots
    @{
    */
  /// Calibration browse clicked
  void calibrationBrowseClicked();
  /// Timeout of the timer
  void timerTimeout();
  /// Color changed
  void colorChanged(const QString& role, const QColor& color);
  /// Point size changed
  void pointSizeChanged(double pointSize);
  /// Show points toggled
  void showPointsToggled(bool checked);
  /// Smooth points toggled
  void smoothPointsToggled(bool checked);
  /// Min distance changed
  void minDistanceChanged(double minDistance);
  /// Max distance changed
  void maxDistanceChanged(double maxDistance);
  /// Render the scene
  void render(View3d& view, Scene3d& scene);
  /** @}
    */

};

#endif // SENSORLIVECONTROL_H
