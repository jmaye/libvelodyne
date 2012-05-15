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

/** \file SensorBrowseControl.h
    \brief This file defines a Qt control for visualizing Velodyne data from
           a log file.
  */

#ifndef SENSORBROWSECONTROL_H
#define SENSORBROWSECONTROL_H

#include <QtGui/QColor>
#include <QtGui/QAction>
#include <QtCore/QTimer>
#include <QtCore/QString>

#include "visualization/Control.h"
#include "visualization/View3d.h"
#include "visualization/Scene3d.h"
#include "base/Singleton.h"
#include "sensor/Calibration.h"
#include "sensor/DataPacket.h"
#include "data-structures/VdynePointCloud.h"

class Ui_SensorBrowseControl;

/** The SensorBrowseControl class represents a control for visualizing Velodyne
    data from a log file.
    \brief Sensor control for Velodyne from log file
  */
class SensorBrowseControl :
  public Control,
  public Singleton<SensorBrowseControl> {

Q_OBJECT

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  SensorBrowseControl(const SensorBrowseControl& other);
  /// Assignment operator
  SensorBrowseControl& operator = (const SensorBrowseControl& other);
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs control from parameters
  SensorBrowseControl(bool showPoints = true);
  /// Destructor
  ~SensorBrowseControl();
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
  /// Sets the log filename
  void setLogFilename(const QString& filename);
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
  /// Open log file
  bool open(const char* filename);
  /// Close log file
  void close();
  /// Rewind log file
  bool rewind();
  /// Get progress of log file
  double getProgress() const;
  /// Read packet from log file
  bool readPacket(DataPacket& packet);
  /// Read point cloud
  bool readPointCloud();
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Qt user interface
  Ui_SensorBrowseControl* mUi;
  /// Palette
  Palette mPalette;
  /// Log menu
  QMenu* mLogMenu;
  /// Log start
  QAction* mLogStartAction;
  /// Log play
  QAction* mLogPlayAction;
  /// Log skip
  QAction* mLogSkipAction;
  /// Timer
  QTimer mTimer;
  /// Velodyne calibration
  Calibration mCalibration;
  /// Log file
  std::ifstream mLogFile;
  /// Log file size
  size_t mLogFilesize;
  /// Log file position
  size_t mLogFilepos;
  /// Min distance of the points
  double mMinDistance;
  /// Max distance of the points
  double mMaxDistance;
  /// Velodyne point cloud
  VdynePointCloud mPointCloud;
  /** @}
    */

protected slots:
  /** \name Qt slots
    @{
    */
  /// Calibration browse clicked
  void calibrationBrowseClicked();
  /// Log browse clicked
  void logBrowseClicked();
  /// Log play toggled
  void logPlayToggled();
  /// Log play triggered
  void logPlayTriggered();
  /// Log start clicked
  void logStartClicked();
  /// Log skip clicked
  void logSkipClicked();
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

signals:
  /** \name Qt signals
    @{
    */
  /// Start Velodyne spinning
  void start();
  /// Stop Velodyne spinning
  void stop();
  /** @}
    */

};

#endif // SENSORBROWSECONTROL_H
