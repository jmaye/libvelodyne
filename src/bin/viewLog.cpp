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

/** \file viewLog.cpp
    \brief This file is a testing binary for viewing a log file.
  */

#include "sensor/VelodyneCalibration.h"
#include "sensor/VelodynePacket.h"
#include "sensor/VelodynePointCloud.h"
#include "visualization/WindowStatic.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <logFile> <calibrationFile>"
      << std::endl;
    return -1;
  }
  WindowStatic window(argc, argv);
  std::ifstream logFile(argv[1], std::ios::in | std::ios::binary);
  VelodyneCalibration vdyneCalibration;
  std::ifstream calibFile(argv[2]);
  calibFile >> vdyneCalibration;
  while (logFile.eof() != true) {
    VelodynePacket vdynePacket;
    vdynePacket.read(logFile);
    VelodynePointCloud pointCloud(vdynePacket, vdyneCalibration);
    window.addPointCloud(pointCloud);
  }
  window.createGlList();
  window.setTranslation(0, -10, -60);
  window.setRotation(0, 30, 0);
  window.setVisibility(true);
  window.show();
  return 0;
}
