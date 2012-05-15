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

/** \file VelodyneControl.h
    \brief This file defines a Qt control for the Velodyne
  */

#ifndef VELODYNECONTROL_H
#define VELODYNECONTROL_H

#include <QtCore/QTimer>

#include "visualization/Control.h"
#include "base/Singleton.h"
#include "visualization/View3d.h"
#include "visualization/Scene3d.h"

class Ui_VelodyneControl;

/** The VelodyneControl class represents a Qt control for the Velodyne.
    \brief Qt control for the Velodyne
  */
class VelodyneControl :
  public Control,
  public Singleton<VelodyneControl> {

Q_OBJECT

  /** \name Private constructors
    @{
    */
  /// Copy constructor
  VelodyneControl(const VelodyneControl& other);
  /// Assignment operator
  VelodyneControl& operator = (const VelodyneControl& other);
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs the control with parameters
  VelodyneControl(bool showVelodyne = true);
  /// Destructor
  ~VelodyneControl();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Displays the Velodyne
  void setShowVelodyne(bool showVelodyne);
  /** @}
    */

protected:
  /** \name Protected methods
    @{
    */
  /// Render the Velodyne
  void renderVelodyne();
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Qt user interface
  Ui_VelodyneControl* mUi;
  /// GL display list for the Velodyne base
  GLint mVelodyneBaseDisplayList;
  /// GL display list for the Velodyne spinning part
  GLint mVelodyneSpinDisplayList;
  /// Velodyne angle
  double mAngle;
  /// Rotate timer
  QTimer mRotateTimer;
  /** @}
    */

protected slots:
  /** \name Qt slots
    @{
    */
  /// Show the Velodyne
  void showVelodyneToggled(bool checked);
  /// Render the Velodyne
  void render(View3d& view, Scene3d& scene);
  /// Clients should create display lists
  void createDisplayLists();
  /// Timeout of the timer
  void timerTimeout();
  /// Start spinning
  void start();
  /// Stop spinning
  void stop();
  /** @}
    */

};

#endif // VELODYNECONTROL_H
