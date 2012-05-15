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

/** \file browseView.cpp
    \brief This file is a testing binary for viewing Velodyne data from a log
           file.
  */

#include <QtGui/QApplication>

#include "visualization/MainWindow.h"
#include "visualization/View3d.h"
#include "visualization/View3dControl.h"
#include "visualization/SensorBrowseControl.h"
#include "visualization/VelodyneControl.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.setWindowTitle("Velodyne Browse View");
  mainWindow.addView(QString("Velodyne Data"), View3d::getInstance());
  mainWindow.addControl(QString("View3d"), View3dControl::getInstance());
  mainWindow.addControl(QString("Sensor"), SensorBrowseControl::getInstance());
  mainWindow.addControl(QString("Velodyne"), VelodyneControl::getInstance());
  QObject::connect(&SensorBrowseControl::getInstance(),
    SIGNAL(start()),
    &VelodyneControl::getInstance(),
    SLOT(start()));
  QObject::connect(&SensorBrowseControl::getInstance(),
    SIGNAL(stop()),
    &VelodyneControl::getInstance(),
    SLOT(stop()));
  mainWindow.show();
  const int ret =  app.exec();
   QObject::disconnect(&SensorBrowseControl::getInstance(),
    SIGNAL(start()),
    &VelodyneControl::getInstance(),
    SLOT(start()));
  QObject::disconnect(&SensorBrowseControl::getInstance(),
    SIGNAL(stop()),
    &VelodyneControl::getInstance(),
    SLOT(stop()));
  return ret;
}
