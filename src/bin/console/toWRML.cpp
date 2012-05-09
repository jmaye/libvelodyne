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

/** \file toWRML.cpp
    \brief This file is a testing binary for writing Velodyne data packet to
           WRML.
  */

#include <iostream>
#include <fstream>

#include "sensor/Calibration.h"
#include "sensor/DataPacket.h"
#include "sensor/Converter.h"
#include "data-structures/VdynePointCloud.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
      << " <logFile> <calibrationFile> <wrmlFile>" << std::endl;
    return -1;
  }
  std::ifstream logFile(argv[1], std::ios::in | std::ios::binary);
  Calibration calibration;
  std::ifstream calibFile(argv[2]);
  calibFile >> calibration;
  std::ofstream wrmlFile(argv[3]);
  wrmlFile << "#VRML V2.0 utf8" << std::endl
           << "Shape {" << std::endl
           << "   geometry PointSet {" << std::endl
           << "      coord Coordinate {" << std::endl
           << "         point [" <<std:: endl;
  while (logFile.eof() != true) {
    DataPacket dataPacket;
    dataPacket.readBinary(logFile);
    VdynePointCloud pointCloud;
    Converter::toPointCloud(dataPacket, calibration, pointCloud);
    wrmlFile << pointCloud;
  }
  wrmlFile << "         ]" << std::endl
           << "      }" << std::endl
           << "   }" << std::endl
           << "}" << std::endl;
  return 0;
}
