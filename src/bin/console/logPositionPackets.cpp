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

/** \file logDataPackets.cpp
    \brief This file is a testing binary for logging Velodyne position packets.
  */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include "com/UDPConnectionServer.h"
#include "sensor/PositionPacket.h"
#include "exceptions/IOException.h"
#include "exceptions/SystemException.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <LogFile> <PktNbr>" << std::endl;
    return -1;
  }
  UDPConnectionServer com(8308);
  const size_t numPackets = atoi(argv[2]);
  size_t packetCount = 0;
  while (packetCount < numPackets) {
    PositionPacket posPacket;
    try {
      posPacket.readBinary(com);
    }
    catch (IOException& e) {
      std::cerr << e.what() << std::endl;
      continue;
    }
    catch (SystemException& e) {
      std::cerr << e.what() << std::endl;
      continue;
    }
    std::ofstream logFile(argv[1], std::ios::app);
    posPacket.writeBinary(logFile);
    packetCount++;
  }
  return 0;
}
