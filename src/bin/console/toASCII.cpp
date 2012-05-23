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

/** \file toASCII.cpp
    \brief This file is a testing binary for writing Velodyne data packets to
           ASCII.
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
      << " <logFile> <calibrationFile> <asciiFile>" << std::endl;
    return -1;
  }
  std::ifstream logFile(argv[1]);
  Calibration calibration;
  std::ifstream calibFile(argv[2]);
  calibFile >> calibration;
  std::ofstream asciiFile(argv[3]);
  while (logFile.eof() != true) {
    DataPacket dataPacket;
    dataPacket.readBinary(logFile);
    VdynePointCloud pointCloud;
    Converter::toPointCloud(dataPacket, calibration, pointCloud);
    asciiFile << pointCloud;
  }
  return 0;
}
