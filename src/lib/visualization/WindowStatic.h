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

/** \file WindowStatic.h
    \brief This file defines the WindowStatic class, which displays the Velodyne
           point cloud from recorded data
  */

#ifndef WINDOWSTATIC_H
#define WINDOWSTATIC_H

#include "sensor/VelodynePointCloud.h"
#include "sensor/VelodyneCalibration.h"

#include <GL/gl.h>

#include <vector>

#include <stdint.h>

/** The class WindowStatic displays the Velodyne point cloud statically.
    \brief Velodyne static visualization
  */
class WindowStatic {
  WindowStatic(const WindowStatic &other);
  WindowStatic& operator = (const WindowStatic &other);

  void drawBackground(float f32Red, float f32Green, float f32Blue) const;
  void drawAxes(float f32Length) const;

  static void displayCallback();
  static void reshapeCallback(int i32Width, int i32Height);
  static void motionCallback(int i32X, int i32Y);
  static void mouseCallback(int i32Button, int i32State, int i32X, int i32Y);
  static void keyboardCallback(unsigned char u8Key, int i32X, int i32Y);

  static const int32_t mci32InitX = 100;
  static const int32_t mci32InitY = 100;
  static const int32_t mci32GlutWheelUp = 3;
  static const int32_t mci32GlutWheelDown = 4;

  int mi32ID;
  int mi32Width;
  int mi32Height;

  Point3D mTranslation;
  Point3D mRotation;

  double mf64Scale;

  bool mbShowAxes;

  int mi32MouseButton;
  int mi32MouseState;
  int mi32StartX;
  int mi32StartY;

  std::vector<Point3D> mPointCloudVector;

  GLuint mListIndex;

public:
  WindowStatic();
  WindowStatic(int argc, char **argv, int i32Width = 640, int i32Height = 480);
  ~WindowStatic();

  void show() const;
  void redraw() const;
  void setTranslation(double f64X, double f64Y, double f64Z);
  void setRotation(double f64Yaw, double f64Pitch, double f64Roll);
  void setScale(double f64Scale);
  void setVisibility(bool bShowAxes);
  void drawScene() const;
  void addPointCloud(const VelodynePointCloud &pointCloud);
  void createGlList();

protected:

};

#endif // WINDOWSTATIC_H
