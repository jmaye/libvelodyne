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

/** \file logDataPacketsBuffer.cpp
    \brief This file is a testing binary for logging Velodyne data packets
           buffer.
  */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include "sensor/AcquisitionThread.h"
#include "sensor/DataPacket.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <LogFile> <PktNbr>" << std::endl;
    return -1;
  }
  UDPConnectionServer connection(2368);
  AcquisitionThread<DataPacket> acqThread(connection);
  acqThread.start();
  const size_t numPackets = atoi(argv[2]);
  size_t packetCount = 0;
  while (packetCount < numPackets)
    if (!acqThread.getBuffer().isEmpty()) {
      std::ofstream logFile (argv[1], std::ios::app);
      acqThread.getBuffer().dequeue()->writeBinary(logFile);
      packetCount++;
    }
  return 0;
}
