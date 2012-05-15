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

/** \file liveView.cpp
    \brief This file is a testing binary for a live view of Velodyne data.
  */

#include <QtGui/QApplication>

#include "visualization/MainWindow.h"
#include "visualization/View3d.h"
#include "visualization/View3dControl.h"
#include "visualization/SensorLiveControl.h"
#include "visualization/VelodyneControl.h"
#include "sensor/AcquisitionThread.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  UDPConnectionServer connection(2368);
  AcquisitionThread<DataPacket> acqThread(connection);
  SensorLiveControl sensorLiveControl(acqThread.getBuffer());
  mainWindow.setWindowTitle("Velodyne Live View");
  mainWindow.addView(QString("Velodyne Data"), View3d::getInstance());
  mainWindow.addControl(QString("View3d"), View3dControl::getInstance());
  mainWindow.addControl(QString("Sensor"), sensorLiveControl);
  mainWindow.addControl(QString("Velodyne"), VelodyneControl::getInstance());
  acqThread.start();
  QObject::connect(&sensorLiveControl,
    SIGNAL(start()),
    &VelodyneControl::getInstance(),
    SLOT(start()));
  QObject::connect(&sensorLiveControl,
    SIGNAL(stop()),
    &VelodyneControl::getInstance(),
    SLOT(stop()));
  mainWindow.show();
  const int ret = app.exec();
  QObject::disconnect(&sensorLiveControl,
    SIGNAL(start()),
    &VelodyneControl::getInstance(),
    SLOT(start()));
  QObject::disconnect(&sensorLiveControl,
    SIGNAL(stop()),
    &VelodyneControl::getInstance(),
    SLOT(stop()));
  return ret;
}
